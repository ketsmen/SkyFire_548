/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Threading/BoostAsioWork.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

#include <iostream>
#include <memory>

int main()
{
    boost::asio::io_context ioContext;
    std::unique_ptr<Skyfire::Asio::IoContextWorkGuard> workGuard = Skyfire::Asio::MakeIoContextWorkGuard(ioContext);

    if (!workGuard)
    {
        std::cerr << "MakeIoContextWorkGuard returned an empty guard\n";
        return 1;
    }

    bool posted = false;
    boost::asio::post(ioContext, [&posted]
    {
        posted = true;
    });

    Skyfire::Asio::ResetWorkGuard(workGuard);

    if (workGuard)
    {
        std::cerr << "ResetWorkGuard left the guard allocated\n";
        return 1;
    }

    if (ioContext.run() != 1)
    {
        std::cerr << "io_context did not run exactly one posted handler\n";
        return 1;
    }

    if (!posted)
    {
        std::cerr << "Posted handler did not run after work guard reset\n";
        return 1;
    }

    return 0;
}
