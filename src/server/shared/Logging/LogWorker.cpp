/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LogWorker.h"

#include <boost/asio/post.hpp>

LogWorker::LogWorker()
    : m_ioContext(), m_workGuard(new WorkGuard(boost::asio::make_work_guard(m_ioContext))),
    m_thread(&LogWorker::svc, this), m_active(true)
{
}

LogWorker::~LogWorker()
{
    {
        std::lock_guard<std::mutex> guard(m_queueLock);
        m_active = false;
        m_workGuard.reset();
    }

    if (m_thread.joinable())
        m_thread.join();
}

int LogWorker::enqueue(LogOperation* op)
{
    if (!op)
        return -1;

    {
        std::lock_guard<std::mutex> guard(m_queueLock);

        if (!m_active)
            return -1;
    }

    boost::asio::post(m_ioContext,
        [op]
        {
            op->call();
            delete op;
        });

    return 0;
}

int LogWorker::svc()
{
    m_ioContext.run();
    return 0;
}
