/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LogWorker.h"
#include "Threading/BoostAsioWork.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

#include <mutex>
#include <thread>

struct LogWorker::Impl
{
    Impl()
        : ioContext(), workGuard(Skyfire::Asio::MakeIoContextWorkGuard(ioContext)),
        active(true)
    {
    }

    boost::asio::io_context ioContext;
    std::unique_ptr<Skyfire::Asio::IoContextWorkGuard> workGuard;
    std::mutex queueLock;
    std::thread thread;
    bool active;
};

LogWorker::LogWorker()
    : m_impl(new Impl)
{
    m_impl->thread = std::thread(&LogWorker::svc, this);
}

LogWorker::~LogWorker()
{
    {
        std::lock_guard<std::mutex> guard(m_impl->queueLock);
        m_impl->active = false;
        Skyfire::Asio::ResetWorkGuard(m_impl->workGuard);
    }

    if (m_impl->thread.joinable())
        m_impl->thread.join();
}

int LogWorker::enqueue(LogOperation* op)
{
    if (!op)
        return -1;

    {
        std::lock_guard<std::mutex> guard(m_impl->queueLock);

        if (!m_impl->active)
            return -1;
    }

    boost::asio::post(m_impl->ioContext,
        [op]
        {
            op->call();
            delete op;
        });

    return 0;
}

int LogWorker::svc()
{
    m_impl->ioContext.run();
    return 0;
}
