/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SFSoap.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>

int main()
{
    SFSoapService service;

    if (service.IsRunning())
    {
        std::cerr << "New SFSoapService reported running\n";
        return 1;
    }

    if (service.StartWithRunner(std::function<void()>()))
    {
        std::cerr << "SFSoapService accepted an empty runner\n";
        return 1;
    }

    std::mutex lock;
    std::condition_variable changed;
    bool entered = false;
    bool finish = false;

    if (!service.StartWithRunner([&lock, &changed, &entered, &finish]
    {
        {
            std::lock_guard<std::mutex> guard(lock);
            entered = true;
        }

        changed.notify_all();

        std::unique_lock<std::mutex> waitLock(lock);
        changed.wait(waitLock, [&finish]
        {
            return finish;
        });
    }))
    {
        std::cerr << "SFSoapService rejected the first runner\n";
        return 1;
    }

    if (!service.IsRunning())
    {
        std::cerr << "SFSoapService did not report running after start\n";
        return 1;
    }

    {
        std::unique_lock<std::mutex> waitLock(lock);
        if (!changed.wait_for(waitLock, std::chrono::seconds(2), [&entered]
        {
            return entered;
        }))
        {
            std::cerr << "SFSoapService runner did not start\n";
            return 1;
        }
    }

    if (service.StartWithRunner([] { }))
    {
        std::cerr << "SFSoapService accepted a second runner while running\n";
        return 1;
    }

    {
        std::lock_guard<std::mutex> guard(lock);
        finish = true;
    }

    changed.notify_all();
    service.Join();

    if (service.IsRunning())
    {
        std::cerr << "SFSoapService still reported running after join\n";
        return 1;
    }

    service.Join();

    return 0;
}
