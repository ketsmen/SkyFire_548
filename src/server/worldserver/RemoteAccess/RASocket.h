/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Skyfired
/// @{
/// \file

#ifndef _RASOCKET_H
#define _RASOCKET_H

#include "Common.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

typedef boost::asio::ip::tcp::socket RASocketHandle;

/// Remote Administration socket
class RASocket
{
public:
    RASocket(std::shared_ptr<boost::asio::io_context> ioContext, std::unique_ptr<RASocketHandle> socket, std::string const& remoteAddress);
    virtual ~RASocket() { }

    void start();

private:
    int svc();
    void close();
    bool is_open() const;
    int recv_line(std::string& outLine);
    int process_command(const std::string& command);
    int authenticate();
    int subnegotiate();     ///< Used by telnet protocol RFC 854 / 855
    int check_access_level(const std::string& user);
    int check_password(const std::string& user, const std::string& pass);
    int send_data(char const* data, size_t length);
    int send(const std::string& line);

    static void zprint(void* callbackArg, const char* szText);
    static void commandFinished(void* callbackArg, bool success);

private:
    std::shared_ptr<boost::asio::io_context> _ioContext;
    std::unique_ptr<RASocketHandle> _socket;
    std::string _remoteAddress;
    uint8 _minLevel; ///< Minimum security level required to connect
    std::atomic<bool> _commandExecuting;
    std::mutex _commandLock;
    std::condition_variable _commandCondition;
    std::queue<std::string> _commandOutput;
    bool _commandComplete;
};

#endif

/// @}
