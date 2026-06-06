/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "Network/BoostAsioWriteQueue.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#endif

namespace
{
    using boost::asio::ip::tcp;

#ifdef _WIN32
    class WinsockScope
    {
    public:
        WinsockScope()
        {
            WSAStartup(MAKEWORD(2, 2), &_data);
        }

        ~WinsockScope()
        {
            WSACleanup();
        }

    private:
        WSADATA _data;
    };
#endif

    void CreateConnectedSockets(boost::asio::io_context& ioContext, tcp::socket& server, tcp::socket& peer)
    {
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 0));

        peer = tcp::socket(ioContext);
        peer.connect(tcp::endpoint(boost::asio::ip::address_v4::loopback(), acceptor.local_endpoint().port()));
        peer.non_blocking(true);

        acceptor.accept(server);
    }

    bool IsWouldBlock(boost::system::error_code const& error)
    {
        return error == boost::asio::error::would_block || error == boost::asio::error::try_again;
    }

    bool TryRead(tcp::socket& peer, std::string& received, std::string& errorText)
    {
        std::array<char, 64> buffer = {};
        boost::system::error_code error;
        size_t bytes = peer.read_some(boost::asio::buffer(buffer), error);

        if (!error)
        {
            received.append(buffer.data(), bytes);
            return true;
        }

        if (IsWouldBlock(error))
            return false;

        errorText = error.message();
        return false;
    }

    bool WaitForRead(boost::asio::io_context& ioContext, tcp::socket& peer, std::string& received, size_t expectedSize)
    {
        std::string errorText;
        for (uint32 i = 0; i < 200 && received.size() < expectedSize; ++i)
        {
            ioContext.restart();
            ioContext.poll();

            errorText.clear();
            TryRead(peer, received, errorText);
            if (!errorText.empty())
            {
                std::cerr << "Unexpected peer read error: " << errorText << '\n';
                return false;
            }

            if (received.size() < expectedSize)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        return received.size() >= expectedSize;
    }

    std::vector<char> MakeData(char const* text)
    {
        return std::vector<char>(text, text + std::strlen(text));
    }
}

int main()
{
#ifdef _WIN32
    WinsockScope winsock;
#endif

    boost::asio::io_context ioContext;
    tcp::socket server(ioContext);
    tcp::socket peer(ioContext);
    CreateConnectedSockets(ioContext, server, peer);

    Skyfire::Net::BoostAsioWriteQueue<tcp::socket> queue(server);

    uint32 completedWrites = 0;
    boost::system::error_code firstError;
    boost::system::error_code secondError;

    queue.Queue(MakeData("first"),
        [&completedWrites, &firstError](boost::system::error_code const& error, size_t bytes)
        {
            firstError = error;
            if (bytes == 5)
                ++completedWrites;
        });

    queue.Queue(MakeData("second"),
        [&completedWrites, &secondError](boost::system::error_code const& error, size_t bytes)
        {
            secondError = error;
            if (bytes == 6)
                ++completedWrites;
        });

    if (!queue.HasPendingOutput())
    {
        std::cerr << "Write queue did not report pending output after queued writes\n";
        return 1;
    }

    std::string received;
    if (!WaitForRead(ioContext, peer, received, std::strlen("firstsecond")))
    {
        std::cerr << "Timed out waiting for queued writes\n";
        return 1;
    }

    if (received != "firstsecond")
    {
        std::cerr << "Queued writes were not serialized in FIFO order. Received: " << received << '\n';
        return 1;
    }

    if (completedWrites != 2 || firstError || secondError)
    {
        std::cerr << "Queued write callbacks did not complete successfully\n";
        return 1;
    }

    for (uint32 i = 0; i < 200 && queue.HasPendingOutput(); ++i)
    {
        ioContext.restart();
        ioContext.poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (queue.HasPendingOutput())
    {
        std::cerr << "Write queue still reports pending output after completed writes\n";
        return 1;
    }

    return 0;
}
