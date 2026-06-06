/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "MemoryUsage.h"

#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

#if PLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#include <psapi.h>
#elif PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
#include <sys/resource.h>
#include <unistd.h>

#if PLATFORM == PLATFORM_UNIX
#include <fstream>
#endif
#endif

namespace
{
    void AppendMemoryField(std::vector<std::string>& fields, char const* label, uint64 bytes)
    {
        std::ostringstream stream;
        stream << label << ' ' << Skyfire::FormatMemorySize(bytes);
        fields.push_back(stream.str());
    }
}

bool Skyfire::GetProcessMemoryUsage(ProcessMemoryUsage& usage)
{
    usage = ProcessMemoryUsage();

#if PLATFORM == PLATFORM_WINDOWS
    PROCESS_MEMORY_COUNTERS_EX counters;
    memset(&counters, 0, sizeof(counters));
    counters.cb = sizeof(counters);

    if (!GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters)))
        return false;

    usage.residentBytes = static_cast<uint64>(counters.WorkingSetSize);
    usage.peakResidentBytes = static_cast<uint64>(counters.PeakWorkingSetSize);
    usage.privateBytes = static_cast<uint64>(counters.PrivateUsage);
    usage.hasResidentBytes = true;
    usage.hasPeakResidentBytes = true;
    usage.hasPrivateBytes = true;
    return true;
#elif PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
    bool hasAnyValue = false;

#if PLATFORM == PLATFORM_UNIX
    std::ifstream statm("/proc/self/statm");
    uint64 virtualPages = 0;
    uint64 residentPages = 0;

    if (statm >> virtualPages >> residentPages)
    {
        long pageSize = sysconf(_SC_PAGESIZE);
        if (pageSize > 0)
        {
            usage.virtualBytes = virtualPages * static_cast<uint64>(pageSize);
            usage.residentBytes = residentPages * static_cast<uint64>(pageSize);
            usage.hasVirtualBytes = true;
            usage.hasResidentBytes = true;
            hasAnyValue = true;
        }
    }
#endif

    struct rusage resourceUsage;
    if (getrusage(RUSAGE_SELF, &resourceUsage) == 0)
    {
#if PLATFORM == PLATFORM_APPLE
        usage.peakResidentBytes = static_cast<uint64>(resourceUsage.ru_maxrss);
#else
        usage.peakResidentBytes = static_cast<uint64>(resourceUsage.ru_maxrss) * 1024;
#endif
        usage.hasPeakResidentBytes = true;
        hasAnyValue = true;
    }

    return hasAnyValue;
#else
    return false;
#endif
}

std::string Skyfire::FormatMemorySize(uint64 bytes)
{
    char const* units[] = { "B", "KB", "MB", "GB", "TB" };
    size_t unitIndex = 0;
    double value = static_cast<double>(bytes);

    while (value >= 1024.0 && unitIndex < (sizeof(units) / sizeof(units[0])) - 1)
    {
        value /= 1024.0;
        ++unitIndex;
    }

    std::ostringstream stream;
    if (unitIndex == 0)
        stream << bytes << " B";
    else
        stream << std::fixed << std::setprecision(2) << value << ' ' << units[unitIndex];

    return stream.str();
}

std::string Skyfire::FormatProcessMemoryUsage(ProcessMemoryUsage const& usage)
{
    std::vector<std::string> fields;

    if (usage.hasResidentBytes)
        AppendMemoryField(fields, "RSS", usage.residentBytes);

    if (usage.hasPrivateBytes)
        AppendMemoryField(fields, "Private", usage.privateBytes);

    if (usage.hasVirtualBytes)
        AppendMemoryField(fields, "Virtual", usage.virtualBytes);

    if (usage.hasPeakResidentBytes)
        AppendMemoryField(fields, "Peak RSS", usage.peakResidentBytes);

    if (fields.empty())
        return "unavailable";

    std::ostringstream stream;
    for (size_t i = 0; i < fields.size(); ++i)
    {
        if (i != 0)
            stream << ", ";

        stream << fields[i];
    }

    return stream.str();
}
