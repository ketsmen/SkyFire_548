/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _M_DELAY_EXECUTOR_H
#define _M_DELAY_EXECUTOR_H

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class DelayTask
{
public:
    virtual ~DelayTask() { }
    virtual int call() = 0;
};

class DelayExecutor
{
public:
    DelayExecutor();
    virtual ~DelayExecutor();
    static DelayExecutor* instance();
    int execute(std::unique_ptr<DelayTask> new_req);
    int start(int num_threads = 1, std::unique_ptr<DelayTask> pre_svc_hook = std::unique_ptr<DelayTask>(), std::unique_ptr<DelayTask> post_svc_hook = std::unique_ptr<DelayTask>());
    int deactivate();
    bool activated();
    int svc();

private:
    typedef boost::asio::executor_work_guard<boost::asio::io_context::executor_type> WorkGuard;

    boost::asio::io_context io_context_;
    std::unique_ptr<WorkGuard> work_guard_;
    std::unique_ptr<DelayTask> pre_svc_hook_;
    std::unique_ptr<DelayTask> post_svc_hook_;
    std::vector<std::thread> threads_;
    std::mutex state_lock_;
    bool activated_;

    void activated(bool s);
};

#endif // _M_DELAY_EXECUTOR_H
