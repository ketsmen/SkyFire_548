/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BOOST_ASIO_UTILS_H
#define SF_BOOST_ASIO_UTILS_H

#include "Common.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <string>

namespace Skyfire
{
namespace Net
{
    bool IsWouldBlock(boost::system::error_code const& error);

    inline void RestartIoContext(boost::asio::io_context& ioContext)
    {
        ioContext.restart();
    }

    inline void StopIoContext(boost::asio::io_context& ioContext)
    {
        ioContext.stop();
    }

    inline void CloseTcpAcceptor(boost::asio::ip::tcp::acceptor& acceptor)
    {
        boost::system::error_code ignored;
        acceptor.cancel(ignored);
        acceptor.close(ignored);
    }

    inline void CloseTcpSocket(boost::asio::ip::tcp::socket& socket)
    {
        boost::system::error_code ignored;
        socket.cancel(ignored);
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
        socket.close(ignored);
    }

    bool OpenTcpAcceptor(boost::asio::io_context& ioContext, boost::asio::ip::tcp::acceptor& acceptor,
        uint16 port, std::string const& bindAddress, char const* logFilter, char const* logName);
}
}

#endif
