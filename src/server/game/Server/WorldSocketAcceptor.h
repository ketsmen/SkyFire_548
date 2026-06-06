/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/** \addtogroup u2w User to World Communication
 *  @{
 *  \file WorldSocketMgr.h
 */

#ifndef SF_WORLDSOCKETACCEPTOR_H
#define SF_WORLDSOCKETACCEPTOR_H

#include "Common.h"
#include "Threading/BoostAsioThreadGroup.h"
#include "WorldSocket.h"
#include <boost/asio/ip/tcp.hpp>
#include <atomic>
#include <memory>

class ReactorRunnable;

class WorldSocketAcceptor
{
public:
    WorldSocketAcceptor();
    ~WorldSocketAcceptor();

    bool Open(uint16 port, const char* address);
    void Close();
    void Update();

private:
    void AsyncAccept();
    void HandleAccept(ReactorRunnable* reactor, WorldSocketHandle socket, boost::asio::ip::tcp::endpoint const& remoteEndpoint,
        boost::system::error_code const& error);

    Skyfire::Asio::IoContextThreadGroup m_ThreadGroup;
    boost::asio::ip::tcp::acceptor m_Acceptor;
    std::atomic<bool> m_Closed;
};

#endif /* __WORLDSOCKETACCEPTOR_H_ */
/// @}
