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
#include "Network/BoostAsioUtils.h"
#include "RARunnable.h"
#include "Threading/BoostAsioExecutor.h"
#include "World.h"

#include "RASocket.h"

#include <boost/asio/error.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/error_code.hpp>
#include <chrono>
#include <memory>
#include <utility>

namespace
{
    void AsyncAcceptRA(std::shared_ptr<Skyfire::Asio::IoContextExecutor> const& executor, std::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor);
    void ScheduleStopCheck(std::shared_ptr<Skyfire::Asio::IoContextExecutor> const& executor, std::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor,
        std::shared_ptr<boost::asio::steady_timer> const& timer);
}

void RARunnable::Run()
{
    if (!sConfigMgr->GetBoolDefault("Ra.Enable", false))
        return;

    uint16 raPort = uint16(sConfigMgr->GetIntDefault("Ra.Port", 3443));
    std::string stringIp = sConfigMgr->GetStringDefault("Ra.IP", "0.0.0.0");

    std::shared_ptr<Skyfire::Asio::IoContextExecutor> executor(new Skyfire::Asio::IoContextExecutor);
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor(new boost::asio::ip::tcp::acceptor(executor->GetIoContext()));
    if (!Skyfire::Net::OpenTcpAcceptor(executor->GetIoContext(), *acceptor, raPort, stringIp, "server.worldserver", "Skyfire RA"))
        return;

    SF_LOG_INFO("server.worldserver", "Starting Skyfire RA on port %d on %s", raPort, stringIp.c_str());

    AsyncAcceptRA(executor, acceptor);

    std::shared_ptr<boost::asio::steady_timer> stopTimer(new boost::asio::steady_timer(executor->GetIoContext()));
    ScheduleStopCheck(executor, acceptor, stopTimer);

    executor->Run();

    Skyfire::Net::CloseTcpAcceptor(*acceptor);
    Skyfire::Net::CancelTimer(*stopTimer);

    SF_LOG_DEBUG("server.worldserver", "Skyfire RA thread exiting");
}

namespace
{
    void AsyncAcceptRA(std::shared_ptr<Skyfire::Asio::IoContextExecutor> const& executor, std::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor)
    {
        if (!acceptor->is_open())
            return;

        std::shared_ptr<RASocketHandle> clientSocket(new RASocketHandle(executor->GetIoContext()));
        acceptor->async_accept(*clientSocket,
            [executor, acceptor, clientSocket](boost::system::error_code const& error)
            {
                if (error)
                {
                    if (error != boost::asio::error::operation_aborted)
                        SF_LOG_ERROR("commands.ra", "Skyfire RA failed to accept socket, error %d", error.value());
                }
                else
                {
                    boost::system::error_code endpointError;
                    boost::asio::ip::tcp::endpoint remoteEndpoint = clientSocket->remote_endpoint(endpointError);
                    std::string remote = endpointError ? std::string("<unknown>") : remoteEndpoint.address().to_string();

                    SF_LOG_INFO("commands.ra", "Incoming connection from %s", remote.c_str());

                    std::unique_ptr<RASocketHandle> socketHandle(new RASocketHandle(std::move(*clientSocket)));
                    std::make_shared<RASocket>(executor, std::move(socketHandle), remote)->start();
                }

                if (!World::IsStopped())
                    AsyncAcceptRA(executor, acceptor);
            });
    }

    void ScheduleStopCheck(std::shared_ptr<Skyfire::Asio::IoContextExecutor> const& executor, std::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor,
        std::shared_ptr<boost::asio::steady_timer> const& timer)
    {
        timer->expires_after(std::chrono::milliseconds(100));
        timer->async_wait(
            [executor, acceptor, timer](boost::system::error_code const& error)
            {
                if (error == boost::asio::error::operation_aborted)
                    return;

                if (World::IsStopped())
                {
                    Skyfire::Net::CloseTcpAcceptor(*acceptor);
                    executor->Stop();
                    return;
                }

                ScheduleStopCheck(executor, acceptor, timer);
            });
    }
}
