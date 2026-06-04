/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef LOGWORKER_H
#define LOGWORKER_H

#include "LogOperation.h"

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include <mutex>
#include <thread>

class LogWorker
{
public:
    LogWorker();
    ~LogWorker();

    enum
    {
        HIGH_WATERMARK = 8 * 1024 * 1024,
        LOW_WATERMARK = 8 * 1024 * 1024
    };

    int enqueue(LogOperation* op);

private:
    typedef boost::asio::executor_work_guard<boost::asio::io_context::executor_type> WorkGuard;

    int svc();

    boost::asio::io_context m_ioContext;
    std::unique_ptr<WorkGuard> m_workGuard;
    std::mutex m_queueLock;
    std::thread m_thread;
    bool m_active;
};

#endif
