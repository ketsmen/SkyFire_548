/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LogWorker.h"
#include "Threading/BoostAsioThreadGroup.h"

#include <mutex>

struct LogWorker::Impl
{
    Impl()
        : threadGroup(), active(true)
    {
    }

    Skyfire::Asio::IoContextThreadGroup threadGroup;
    std::mutex queueLock;
    bool active;
};

LogWorker::LogWorker()
    : m_impl(new Impl)
{
    if (m_impl->threadGroup.Start(1) == -1)
        m_impl->active = false;
}

LogWorker::~LogWorker()
{
    {
        std::lock_guard<std::mutex> guard(m_impl->queueLock);
        m_impl->active = false;
    }

    m_impl->threadGroup.DrainAndJoin();
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

    m_impl->threadGroup.GetExecutor().Post(
        [op]
        {
            op->call();
            delete op;
        });

    return 0;
}
