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

    struct SystemMemoryUsage
    {
        uint64 totalPhysicalBytes = 0;
        uint64 availablePhysicalBytes = 0;
        uint64 usedPhysicalBytes = 0;
        uint64 totalVirtualBytes = 0;
        uint64 usedVirtualBytes = 0;
        uint64 totalSwapBytes = 0;
        uint64 usedSwapBytes = 0;
        double physicalUsagePercent = 0.0;
        double virtualUsagePercent = 0.0;

        bool hasValidData = false;
    };

    bool GetProcessMemoryUsage(ProcessMemoryUsage& usage);
    bool GetSystemMemoryUsage(SystemMemoryUsage& usage);

    std::string FormatMemorySize(uint64 bytes);
    std::string FormatProcessMemoryUsage(ProcessMemoryUsage const& usage);
    std::string FormatSystemMemoryUsage(SystemMemoryUsage const& usage);
    std::string FormatMemoryUsageSummary(ProcessMemoryUsage const& processUsage, SystemMemoryUsage const& systemUsage);
}

#endif
