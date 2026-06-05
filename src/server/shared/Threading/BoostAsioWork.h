/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_BOOST_ASIO_WORK_H
#define SF_BOOST_ASIO_WORK_H

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>

#include <memory>

namespace Skyfire
{
namespace Asio
{
    typedef boost::asio::executor_work_guard<boost::asio::io_context::executor_type> IoContextWorkGuard;

    inline std::unique_ptr<IoContextWorkGuard> MakeIoContextWorkGuard(boost::asio::io_context& ioContext)
    {
        return std::unique_ptr<IoContextWorkGuard>(new IoContextWorkGuard(boost::asio::make_work_guard(ioContext)));
    }

    inline void ResetWorkGuard(std::unique_ptr<IoContextWorkGuard>& workGuard)
    {
        workGuard.reset();
    }
}
}

#endif
