/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BOOST_ASIO_THREAD_GROUP_H
#define SF_BOOST_ASIO_THREAD_GROUP_H

#include "Threading/BoostAsioExecutor.h"

#include <atomic>
#include <cstddef>
#include <functional>
#include <thread>
#include <utility>
#include <vector>

namespace Skyfire
{
namespace Asio
{
    class IoContextThreadGroup
    {
    public:
        typedef std::function<void()> Hook;

        IoContextThreadGroup() : _executor(), _running(false) { }

        ~IoContextThreadGroup()
        {
            StopAndJoin();
        }

        IoContextExecutor& GetExecutor() { return _executor; }
        boost::asio::io_context& GetIoContext() { return _executor.GetIoContext(); }
        bool IsRunning() const { return _running; }

        int Start(std::size_t threadCount, Hook preRun = Hook(), Hook postRun = Hook())
        {
            if (threadCount == 0)
                return -1;

            bool expected = false;
            if (!_running.compare_exchange_strong(expected, true))
                return -1;

            _executor.Restart();
            _executor.KeepAlive();
            _preRun = std::move(preRun);
            _postRun = std::move(postRun);

            try
            {
                for (std::size_t i = 0; i < threadCount; ++i)
                    _threads.push_back(std::thread(&IoContextThreadGroup::Run, this));
            }
            catch (...)
            {
                StopAndJoin();
                return -1;
            }

            return 0;
        }

        void Drain()
        {
            _executor.ResetWork();
        }

        void Stop()
        {
            _executor.Stop();
            _executor.ResetWork();
        }

        void Join()
        {
            for (std::thread& thread : _threads)
                if (thread.joinable())
                    thread.join();

            _threads.clear();
            _preRun = Hook();
            _postRun = Hook();
            _running = false;
        }

        void DrainAndJoin()
        {
            Drain();
            Join();
        }

        void StopAndJoin()
        {
            Stop();
            Join();
        }

    private:
        void Run()
        {
            if (_preRun)
                _preRun();

            _executor.Run();

            if (_postRun)
                _postRun();
        }

        IoContextExecutor _executor;
        std::vector<std::thread> _threads;
        Hook _preRun;
        Hook _postRun;
        std::atomic<bool> _running;
    };
}
}

#endif
