/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Threading/BoostAsioThreadGroup.h"

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>

namespace
{
    bool WaitForCount(int& count, int expected, std::mutex& countLock, std::condition_variable& countChanged)
    {
        std::unique_lock<std::mutex> lock(countLock);
        return countChanged.wait_for(lock, std::chrono::seconds(2),
            [&count, expected]
            {
                return count >= expected;
            });
    }
}

int main()
{
    Skyfire::Asio::IoContextThreadGroup group;

    if (group.Start(0) != -1)
    {
        std::cerr << "IoContextThreadGroup accepted zero threads\n";
        return 1;
    }

    int preRuns = 0;
    int postRuns = 0;
    int taskRuns = 0;
    std::mutex lock;
    std::condition_variable changed;

    if (group.Start(2,
        [&]
        {
            std::lock_guard<std::mutex> guard(lock);
            ++preRuns;
            changed.notify_all();
        },
        [&]
        {
            std::lock_guard<std::mutex> guard(lock);
            ++postRuns;
            changed.notify_all();
        }) != 0)
    {
        std::cerr << "IoContextThreadGroup did not start\n";
        return 1;
    }

    if (group.Start(1) != -1)
    {
        std::cerr << "IoContextThreadGroup started twice\n";
        return 1;
    }

    for (int i = 0; i < 4; ++i)
    {
        group.GetExecutor().Post([&]
        {
            std::lock_guard<std::mutex> guard(lock);
            ++taskRuns;
            changed.notify_all();
        });
    }

    if (!WaitForCount(taskRuns, 4, lock, changed))
    {
        std::cerr << "IoContextThreadGroup did not run posted tasks\n";
        return 1;
    }

    group.Drain();
    group.Join();

    if (!WaitForCount(postRuns, 2, lock, changed))
    {
        std::cerr << "IoContextThreadGroup did not run post hooks\n";
        return 1;
    }

    if (preRuns != 2 || postRuns != 2)
    {
        std::cerr << "IoContextThreadGroup hook counts were pre=" << preRuns << " post=" << postRuns << '\n';
        return 1;
    }

    if (group.IsRunning())
    {
        std::cerr << "IoContextThreadGroup still reported running after join\n";
        return 1;
    }

    if (group.Start(1) != 0)
    {
        std::cerr << "IoContextThreadGroup did not restart\n";
        return 1;
    }

    int restartRuns = 0;
    group.GetExecutor().Post([&]
    {
        std::lock_guard<std::mutex> guard(lock);
        ++restartRuns;
        changed.notify_all();
    });

    if (!WaitForCount(restartRuns, 1, lock, changed))
    {
        std::cerr << "IoContextThreadGroup did not run work after restart\n";
        return 1;
    }

    group.DrainAndJoin();

    return 0;
}
