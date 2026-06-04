/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_REALMACCEPTOR_H
#define SF_REALMACCEPTOR_H

#include "Common.h"
#include "RealmSocket.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <atomic>
#include <thread>
#include <string>

class RealmAcceptor
{
public:
    RealmAcceptor();
    ~RealmAcceptor();

    bool Open(uint16 port, std::string const& bindIp);
    void Close();
    void Update();

private:
    void AsyncAccept();
    void HandleAccept(std::shared_ptr<RealmSocketHandle> clientSocket, boost::system::error_code const& error);

    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::thread _thread;
    std::atomic<bool> _closed;
};

#endif
