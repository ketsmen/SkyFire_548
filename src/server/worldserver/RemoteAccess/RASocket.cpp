/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/** \file
    \ingroup Skyfired
*/

#include "AccountMgr.h"
#include "Common.h"
#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "RASocket.h"
#include "SRP6.h"
#include "Util.h"
#include "World.h"
#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/error_code.hpp>
#include <chrono>
#include <cstring>
#include <thread>
#include <utility>

namespace
{
    bool IsWouldBlock(boost::system::error_code const& error)
    {
        return error == boost::asio::error::would_block || error == boost::asio::error::try_again;
    }
}

RASocket::RASocket(std::shared_ptr<boost::asio::io_context> ioContext, std::unique_ptr<RASocketHandle> socket, std::string const& remoteAddress) :
    _ioContext(std::move(ioContext)), _socket(std::move(socket)), _remoteAddress(remoteAddress), _minLevel(3), _commandExecuting(false), _commandComplete(false)
{
    _minLevel = uint8(sConfigMgr->GetIntDefault("RA.MinLevel", 3));
}

void RASocket::start()
{
    std::thread([this]
    {
        svc();
        close();
        delete this;
    }).detach();
}

void RASocket::close()
{
    SF_LOG_INFO("commands.ra", "Closing connection");

    if (is_open())
    {
        boost::system::error_code ignored;
        _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
        _socket->close(ignored);
    }

    while (_commandExecuting)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}

bool RASocket::is_open() const
{
    return _socket && _socket->is_open();
}

int RASocket::send_data(char const* data, size_t length)
{
    if (!is_open())
        return -1;

    boost::system::error_code error;
    boost::asio::write(*_socket, boost::asio::buffer(data, length), error);

    return error ? -1 : 0;
}

int RASocket::send(const std::string& line)
{
    return send_data(line.c_str(), line.length());
}

int RASocket::recv_line(std::string& out_line)
{
    out_line.clear();
    char byte;
    for (;;)
    {
        if (!is_open())
            return -1;

        boost::system::error_code error;
        size_t n = _socket->read_some(boost::asio::buffer(&byte, sizeof(byte)), error);

        if (error || n == 0)
            return -1;

        ASSERT(n == sizeof(byte));

        if (byte == '\n')
            break;
        else if (byte == '\r') /* Ignore CR */
            continue;
        else
            out_line += byte;
    }

    return 0;
}

int RASocket::process_command(const std::string& command)
{
    if (command.length() == 0)
        return 0;

    SF_LOG_INFO("commands.ra", "Received command: %s", command.c_str());

    // handle quit, exit and logout commands to terminate connection
    if (command == "quit" || command == "exit" || command == "logout") {
        (void)send("Bye\r\n");
        return -1;
    }

    _commandExecuting = true;
    {
        std::lock_guard<std::mutex> guard(_commandLock);
        _commandComplete = false;
        std::queue<std::string> empty;
        std::swap(_commandOutput, empty);
    }

    CliCommandHolder* cmd = new CliCommandHolder(this, command.c_str(), &RASocket::zprint, &RASocket::commandFinished);
    sWorld->QueueCliCommand(cmd);

    // wait for result
    for (;;)
    {
        std::string output;

        {
            std::unique_lock<std::mutex> lock(_commandLock);
            _commandCondition.wait(lock, [this] { return !_commandOutput.empty() || _commandComplete; });

            if (!_commandOutput.empty())
            {
                output = _commandOutput.front();
                _commandOutput.pop();
            }
            else if (_commandComplete)
                break;
        }

        if (!output.empty() && send(output) == -1)
            return -1;
    }

    return 0;
}

int RASocket::check_access_level(const std::string& user)
{
    std::string safeUser = user;

    AccountMgr::normalizeString(safeUser);

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ACCESS);
    stmt->setString(0, safeUser);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (!result)
    {
        SF_LOG_INFO("commands.ra", "User %s does not exist in database", user.c_str());
        return -1;
    }

    Field* fields = result->Fetch();

    if (fields[1].GetUInt8() < _minLevel)
    {
        SF_LOG_INFO("commands.ra", "User %s has no privilege to login", user.c_str());
        return -1;
    }
    else if (fields[2].GetInt32() != -1)
    {
        SF_LOG_INFO("commands.ra", "User %s has to be assigned on all realms (with RealmID = '-1')", user.c_str());
        return -1;
    }

    return 0;
}

int RASocket::check_password(const std::string& user, const std::string& pass)
{
    std::string safe_user = user;
    AccountMgr::normalizeString(safe_user);

    std::string safe_pass = pass;
    AccountMgr::normalizeString(safe_pass);

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_CHECK_PASSWORD_BY_NAME);

    stmt->setString(0, safe_user);
    if (PreparedQueryResult result = LoginDatabase.Query(stmt))
    {
        SkyFire::Crypto::SRP6::Salt salt = (*result)[0].GetBinary<SkyFire::Crypto::SRP6::SALT_LENGTH>();
        SkyFire::Crypto::SRP6::Verifier verifier = (*result)[1].GetBinary<SkyFire::Crypto::SRP6::VERIFIER_LENGTH>();

        if (SkyFire::Crypto::SRP6::CheckLogin(safe_user, safe_pass, salt, verifier))
            return 0;
    }

    SF_LOG_INFO("commands.ra", "Wrong password for user: %s", user.c_str());
    return -1;
}

int RASocket::authenticate()
{
    if (send(std::string("Username: ")) == -1)
        return -1;

    std::string user;
    if (recv_line(user) == -1)
        return -1;

    if (send(std::string("Password: ")) == -1)
        return -1;

    std::string pass;
    if (recv_line(pass) == -1)
        return -1;

    SF_LOG_INFO("commands.ra", "Login attempt for user: %s", user.c_str());

    if (check_access_level(user) == -1)
        return -1;

    if (check_password(user, pass) == -1)
        return -1;

    SF_LOG_INFO("commands.ra", "User login: %s", user.c_str());

    return 0;
}


int RASocket::subnegotiate()
{
    char buf[1024];

    // Wait a maximum of 1000ms for negotiation packet - not all telnet clients may send it
    if (!is_open())
        return -1;

    boost::system::error_code error;
    _socket->non_blocking(true, error);
    if (error)
        return -1;

    size_t n = 0;
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);

    for (;;)
    {
        error.clear();
        n = _socket->read_some(boost::asio::buffer(buf, sizeof(buf) - 1), error);

        if (!error)
            break;

        if (!IsWouldBlock(error))
        {
            _socket->non_blocking(false, error);
            return -1;
        }

        if (std::chrono::steady_clock::now() >= deadline)
        {
            _socket->non_blocking(false, error);
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    _socket->non_blocking(false, error);
    if (error)
        return -1;

    if (n <= 0)
        return int(n);

    if (n >= 1024)
    {
        SF_LOG_DEBUG("commands.ra", "RASocket::subnegotiate: allocated buffer 1024 bytes was too small for negotiation packet, size: %u", uint32(n));
        return -1;
    }

    buf[n] = '\0';

#ifdef _DEBUG
    for (uint8 i = 0; i < n; )
    {
        uint8 iac = buf[i];
        if (iac == 0xFF)   // "Interpret as Command" (IAC)
        {
            uint8 command = buf[++i];
            std::stringstream ss;
            switch (command)
            {
                case 0xFB:        // WILL
                    ss << "WILL ";
                    break;
                case 0xFC:        // WON'T
                    ss << "WON'T ";
                    break;
                case 0xFD:        // DO
                    ss << "DO ";
                    break;
                case 0xFE:        // DON'T
                    ss << "DON'T ";
                    break;
                default:
                    return -1;      // not allowed
            }

            uint8 param = buf[++i];
            ss << uint32(param);
            SF_LOG_DEBUG("commands.ra", ss.str().c_str());
        }
        ++i;
    }
#endif

    //! Just send back end of subnegotiation packet
    uint8 const reply[2] = { 0xFF, 0xF0 };

    return send_data(reinterpret_cast<char const*>(reply), sizeof(reply)) == 0 ? int(sizeof(reply)) : -1;
}

int RASocket::svc(void)
{
    //! Subnegotiation may differ per client - do not react on it
    subnegotiate();

    if (send("Authentication required\r\n") == -1)
        return -1;

    if (authenticate() == -1)
    {
        (void)send("Authentication failed\r\n");
        return -1;
    }

    // send motd
    if (send(std::string(sWorld->GetMotd()) + "\r\n") == -1)
        return -1;

    for (;;)
    {
        // show prompt
        if (send("TC> ") == -1)
            return -1;

        std::string line;

        if (recv_line(line) == -1)
            return -1;

        if (process_command(line) == -1)
            return -1;
    }

    return 0;
}

void RASocket::zprint(void* callbackArg, const char* szText)
{
    if (!szText || !callbackArg)
        return;

    RASocket* socket = static_cast<RASocket*>(callbackArg);
    size_t sz = strlen(szText);

    {
        std::lock_guard<std::mutex> guard(socket->_commandLock);
        socket->_commandOutput.push(std::string(szText, sz));
    }

    socket->_commandCondition.notify_one();
}

void RASocket::commandFinished(void* callbackArg, bool /*success*/)
{
    if (!callbackArg)
        return;

    RASocket* socket = static_cast<RASocket*>(callbackArg);

    {
        std::lock_guard<std::mutex> guard(socket->_commandLock);
        socket->_commandComplete = true;
    }

    socket->_commandExecuting = false;
    socket->_commandCondition.notify_one();
}
