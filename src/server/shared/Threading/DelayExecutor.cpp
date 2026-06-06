/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DelayExecutor.h"
#include "Platform/Singleton.h"
#include "Threading/BoostAsioThreadGroup.h"

#include <mutex>
#include <utility>

struct DelayExecutor::Impl
{
    Impl()
        : activated(false)
    {
    }

    Skyfire::Asio::IoContextThreadGroup threadGroup;
    std::unique_ptr<DelayTask> preSvcHook;
    std::unique_ptr<DelayTask> postSvcHook;
    std::mutex stateLock;
    bool activated;
};

DelayExecutor* DelayExecutor::instance()
{
    return Skyfire::Singleton<DelayExecutor, Skyfire::Mutex>::instance();
}

DelayExecutor::DelayExecutor()
    : impl_(new Impl) { }

DelayExecutor::~DelayExecutor()
{
    deactivate();
}

int DelayExecutor::deactivate()
{
    {
        std::lock_guard<std::mutex> guard(impl_->stateLock);

        if (!impl_->activated)
            return 0;

        impl_->activated = false;
        impl_->threadGroup.Drain();
    }

    impl_->threadGroup.Join();
    impl_->preSvcHook.reset();
    impl_->postSvcHook.reset();

    return 0;
}

int DelayExecutor::svc()
{
    if (impl_->preSvcHook)
        impl_->preSvcHook->call();

    impl_->threadGroup.GetExecutor().Run();

    if (impl_->postSvcHook)
        impl_->postSvcHook->call();

    return 0;
}

int DelayExecutor::start(int num_threads, std::unique_ptr<DelayTask> pre_svc_hook, std::unique_ptr<DelayTask> post_svc_hook)
{
    if (activated())
        return -1;

    if (num_threads < 1)
        return -1;

    impl_->preSvcHook = std::move(pre_svc_hook);
    impl_->postSvcHook = std::move(post_svc_hook);

    int const started = impl_->threadGroup.Start(static_cast<size_t>(num_threads),
        [this]
        {
            if (impl_->preSvcHook)
                impl_->preSvcHook->call();
        },
        [this]
        {
            if (impl_->postSvcHook)
                impl_->postSvcHook->call();
        });

    if (started == -1)
    {
        impl_->preSvcHook.reset();
        impl_->postSvcHook.reset();
        return -1;
    }

    activated(true);

    return 0;
}

int DelayExecutor::execute(std::unique_ptr<DelayTask> new_req)
{
    if (!new_req)
        return -1;

    {
        std::lock_guard<std::mutex> guard(impl_->stateLock);

        if (!impl_->activated)
            return -1;
    }

    impl_->threadGroup.GetExecutor().Post(
        [task = std::move(new_req)]() mutable
        {
            task->call();
        });

    return 0;
}

bool DelayExecutor::activated()
{
    std::lock_guard<std::mutex> guard(impl_->stateLock);
    return impl_->activated;
}

void DelayExecutor::activated(bool s)
{
    std::lock_guard<std::mutex> guard(impl_->stateLock);
    impl_->activated = s;
}
