/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_MEMORY_USAGE_H
#define SKYFIRE_MEMORY_USAGE_H

#include "Define.h"

#include <string>

namespace Skyfire
{
    struct ProcessMemoryUsage
    {
        uint64 residentBytes = 0;
        uint64 peakResidentBytes = 0;
        uint64 virtualBytes = 0;
        uint64 privateBytes = 0;

        bool hasResidentBytes = false;
        bool hasPeakResidentBytes = false;
        bool hasVirtualBytes = false;
        bool hasPrivateBytes = false;
    };

    bool GetProcessMemoryUsage(ProcessMemoryUsage& usage);
    std::string FormatMemorySize(uint64 bytes);
    std::string FormatProcessMemoryUsage(ProcessMemoryUsage const& usage);
}

#endif
