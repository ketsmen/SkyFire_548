/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/** \file
    \ingroup Skyfired
 */

#include "Common.h"
#include "Config.h"
#include "Log.h"
#include "RARunnable.h"
#include "World.h"

#include "RASocket.h"

#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/system/error_code.hpp>
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>

namespace
{
    bool IsWouldBlock(boost::system::error_code const& error)
    {
        return error == boost::asio::error::would_block || error == boost::asio::error::try_again;
    }
}

void RARunnable::Run()
{
    if (!sConfigMgr->GetBoolDefault("Ra.Enable", false))
        return;

    uint16 raPort = uint16(sConfigMgr->GetIntDefault("Ra.Port", 3443));
    std::string stringIp = sConfigMgr->GetStringDefault("Ra.IP", "0.0.0.0");

    std::shared_ptr<boost::asio::io_context> ioContext(new boost::asio::io_context);

    boost::system::error_code error;
    boost::asio::ip::tcp::resolver resolver(*ioContext);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(stringIp, std::to_string(raPort), boost::asio::ip::resolver_base::passive, error);
    if (error)
    {
        SF_LOG_ERROR("server.worldserver", "Skyfire RA can not resolve bind address %s:%d, error %d", stringIp.c_str(), raPort, error.value());
        return;
    }

    boost::asio::ip::tcp::acceptor acceptor(*ioContext);
    boost::system::error_code lastError;

    for (boost::asio::ip::tcp::endpoint const& endpoint : endpoints)
    {
        acceptor.open(endpoint.protocol(), error);
        if (error)
        {
            lastError = error;
            continue;
        }

        acceptor.set_option(boost::asio::socket_base::reuse_address(true), error);
        if (!error)
            acceptor.bind(endpoint, error);

        if (!error)
            acceptor.listen(boost::asio::socket_base::max_listen_connections, error);

        if (!error)
            break;

        lastError = error;

        boost::system::error_code ignored;
        acceptor.close(ignored);
    }

    if (!acceptor.is_open())
    {
        SF_LOG_ERROR("server.worldserver", "Skyfire RA can not bind to port %d on %s, error %d", raPort, stringIp.c_str(), lastError.value());
        return;
    }

    acceptor.non_blocking(true, error);
    if (error)
    {
        SF_LOG_ERROR("server.worldserver", "Skyfire RA can not set listener nonblocking, error %d", error.value());
        return;
    }

    SF_LOG_INFO("server.worldserver", "Starting Skyfire RA on port %d on %s", raPort, stringIp.c_str());

    while (!World::IsStopped())
    {
        error.clear();
        std::unique_ptr<RASocketHandle> clientSocket(new RASocketHandle(*ioContext));
        acceptor.accept(*clientSocket, error);

        if (error)
        {
            if (!IsWouldBlock(error))
                SF_LOG_ERROR("commands.ra", "Skyfire RA failed to accept socket, error %d", error.value());

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        boost::asio::ip::tcp::endpoint remoteEndpoint = clientSocket->remote_endpoint(error);
        std::string remote = error ? std::string("<unknown>") : remoteEndpoint.address().to_string();

        SF_LOG_INFO("commands.ra", "Incoming connection from %s", remote.c_str());

        (new RASocket(ioContext, std::move(clientSocket), remote))->start();
    }

    boost::system::error_code ignored;
    acceptor.close(ignored);

    SF_LOG_DEBUG("server.worldserver", "Skyfire RA thread exiting");
}
