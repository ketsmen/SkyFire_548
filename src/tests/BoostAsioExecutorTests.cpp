/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Threading/BoostAsioExecutor.h"

#include <boost/asio/post.hpp>
#include <iostream>

int main()
{
    Skyfire::Asio::IoContextExecutor executor;

    executor.KeepAlive();

    bool firstTaskRan = false;
    executor.Post([&firstTaskRan]
    {
        firstTaskRan = true;
    });

    executor.ResetWork();

    if (executor.Run() != 1)
    {
        std::cerr << "IoContextExecutor did not run the first posted task\n";
        return 1;
    }

    if (!firstTaskRan)
    {
        std::cerr << "First posted task did not run\n";
        return 1;
    }

    executor.Restart();
    executor.KeepAlive();

    bool secondTaskRan = false;
    executor.Post([&secondTaskRan]
    {
        secondTaskRan = true;
    });

    executor.ResetWork();

    if (executor.Run() != 1)
    {
        std::cerr << "IoContextExecutor did not run after restart\n";
        return 1;
    }

    if (!secondTaskRan)
    {
        std::cerr << "Second posted task did not run after restart\n";
        return 1;
    }

    executor.Restart();
    executor.KeepAlive();
    executor.Post([]
    {
    });
    executor.Stop();
    executor.ResetWork();

    if (!executor.GetIoContext().stopped())
    {
        std::cerr << "IoContextExecutor::Stop did not stop the io_context\n";
        return 1;
    }

    executor.Restart();

    bool taskRanAfterStop = false;
    boost::asio::post(executor.GetIoContext(), [&taskRanAfterStop]
    {
        taskRanAfterStop = true;
    });

    std::size_t handlersRunAfterStop = executor.Run();
    if (handlersRunAfterStop == 0 || !taskRanAfterStop)
    {
        std::cerr << "IoContextExecutor did not restart cleanly after Stop\n";
        return 1;
    }

    return 0;
}
