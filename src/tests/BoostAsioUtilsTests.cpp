/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Network/BoostAsioUtils.h"

#include <boost/asio/error.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/system/error_code.hpp>

#include <iostream>

int main()
{
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor(ioContext);
    boost::asio::ip::tcp::socket peer(ioContext);

    boost::system::error_code error;
    acceptor.open(boost::asio::ip::tcp::v4(), error);
    if (error)
    {
        std::cerr << "Failed to open acceptor: " << error.message() << '\n';
        return 1;
    }

    acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0), error);
    if (error)
    {
        std::cerr << "Failed to bind acceptor: " << error.message() << '\n';
        return 1;
    }

    acceptor.listen(boost::asio::socket_base::max_listen_connections, error);
    if (error)
    {
        std::cerr << "Failed to listen on acceptor: " << error.message() << '\n';
        return 1;
    }

    bool completed = false;
    boost::system::error_code acceptError;
    acceptor.async_accept(peer, [&completed, &acceptError](boost::system::error_code const& callbackError)
    {
        completed = true;
        acceptError = callbackError;
    });

    Skyfire::Net::CloseTcpAcceptor(acceptor);
    Skyfire::Net::CloseTcpAcceptor(acceptor);
    ioContext.run();

    if (acceptor.is_open())
    {
        std::cerr << "CloseTcpAcceptor left the acceptor open\n";
        return 1;
    }

    if (!completed)
    {
        std::cerr << "CloseTcpAcceptor did not complete the pending accept\n";
        return 1;
    }

    if (acceptError != boost::asio::error::operation_aborted)
    {
        std::cerr << "Pending accept completed with " << acceptError.message() << '\n';
        return 1;
    }

    bool postedAfterStop = false;
    boost::asio::post(ioContext, [&postedAfterStop]
    {
        postedAfterStop = true;
    });

    Skyfire::Net::RestartIoContext(ioContext);
    ioContext.run();

    if (!postedAfterStop)
    {
        std::cerr << "RestartIoContext did not allow queued work to run after stop\n";
        return 1;
    }

    boost::asio::io_context guardedContext;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard =
        boost::asio::make_work_guard(guardedContext);

    Skyfire::Net::StopIoContext(guardedContext);

    if (guardedContext.run() != 0)
    {
        std::cerr << "StopIoContext unexpectedly allowed guarded work to run\n";
        return 1;
    }

    if (!guardedContext.stopped())
    {
        std::cerr << "StopIoContext did not leave the io_context stopped\n";
        return 1;
    }

    guard.reset();
    Skyfire::Net::RestartIoContext(guardedContext);

    bool postedAfterExplicitStop = false;
    boost::asio::post(guardedContext, [&postedAfterExplicitStop]
    {
        postedAfterExplicitStop = true;
    });

    guardedContext.run();

    if (!postedAfterExplicitStop)
    {
        std::cerr << "RestartIoContext did not recover after StopIoContext\n";
        return 1;
    }

    return 0;
}
