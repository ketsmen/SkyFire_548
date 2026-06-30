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
#include <sysinfoapi.h>
#elif PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
#include <sys/resource.h>
#include <unistd.h>

#if PLATFORM == PLATFORM_UNIX
#include <fstream>
#include <sys/sysinfo.h>
#endif

#if PLATFORM == PLATFORM_APPLE
#include <mach/mach.h>
#include <sys/sysctl.h>
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

    void AppendPercentageField(std::vector<std::string>& fields, char const* label, double percent)
    {
        std::ostringstream stream;
        stream << label << ' ' << std::fixed << std::setprecision(1) << percent << '%';
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

bool Skyfire::GetSystemMemoryUsage(SystemMemoryUsage& usage)
{
    usage = SystemMemoryUsage();

#if PLATFORM == PLATFORM_WINDOWS
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (!GlobalMemoryStatusEx(&memoryStatus))
        return false;

    usage.totalPhysicalBytes = memoryStatus.ullTotalPhys;
    usage.availablePhysicalBytes = memoryStatus.ullAvailPhys;
    usage.usedPhysicalBytes = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
    usage.totalVirtualBytes = memoryStatus.ullTotalPageFile;
    usage.usedVirtualBytes = memoryStatus.ullTotalPageFile - memoryStatus.ullAvailPageFile;
    usage.totalSwapBytes = 0;
    usage.usedSwapBytes = 0;

    if (usage.totalPhysicalBytes > 0)
    {
        usage.physicalUsagePercent = (static_cast<double>(usage.usedPhysicalBytes) /
            static_cast<double>(usage.totalPhysicalBytes)) * 100.0;
    }

    if (usage.totalVirtualBytes > 0)
    {
        usage.virtualUsagePercent = (static_cast<double>(usage.usedVirtualBytes) /
            static_cast<double>(usage.totalVirtualBytes)) * 100.0;
    }

    usage.hasValidData = true;
    return true;

#elif PLATFORM == PLATFORM_UNIX
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open())
        return false;

    std::string key;
    uint64 value;
    std::string unit;
    uint64 memTotal = 0, memAvailable = 0, swapTotal = 0, swapFree = 0;

    while (meminfo >> key >> value >> unit)
    {
        if (key == "MemTotal:")
            memTotal = value * 1024;
        else if (key == "MemAvailable:")
            memAvailable = value * 1024;
        else if (key == "SwapTotal:")
            swapTotal = value * 1024;
        else if (key == "SwapFree:")
            swapFree = value * 1024;
    }

    if (memTotal == 0)
        return false;

    usage.totalPhysicalBytes = memTotal;
    usage.availablePhysicalBytes = memAvailable > 0 ? memAvailable : 0;
    usage.usedPhysicalBytes = memTotal - (memAvailable > 0 ? memAvailable : 0);
    usage.totalSwapBytes = swapTotal;
    usage.usedSwapBytes = swapTotal - swapFree;
    usage.totalVirtualBytes = memTotal + swapTotal;
    usage.usedVirtualBytes = usage.usedPhysicalBytes + usage.usedSwapBytes;

    if (usage.totalPhysicalBytes > 0)
    {
        usage.physicalUsagePercent = (static_cast<double>(usage.usedPhysicalBytes) /
            static_cast<double>(usage.totalPhysicalBytes)) * 100.0;
    }

    if (usage.totalVirtualBytes > 0)
    {
        usage.virtualUsagePercent = (static_cast<double>(usage.usedVirtualBytes) /
            static_cast<double>(usage.totalVirtualBytes)) * 100.0;
    }

    usage.hasValidData = true;
    return true;

#elif PLATFORM == PLATFORM_APPLE
    int mib[2];
    size_t length;

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(uint64_t);
    if (sysctl(mib, 2, &usage.totalPhysicalBytes, &length, nullptr, 0) != 0)
        return false;

    vm_statistics64_data_t vmStats;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    kern_return_t kernReturn = host_statistics64(mach_host_self(),
        HOST_VM_INFO64,
        reinterpret_cast<host_info_t>(&vmStats),
        &count);

    if (kernReturn != KERN_SUCCESS)
        return false;

    int pageSize;
    mib[0] = CTL_HW;
    mib[1] = HW_PAGESIZE;
    length = sizeof(int);
    if (sysctl(mib, 2, &pageSize, &length, nullptr, 0) != 0)
        return false;

    uint64 usedMemory = static_cast<uint64>(vmStats.active_count +
        vmStats.wire_count +
        vmStats.compressor_page_count) * pageSize;
    uint64 availableMemory = static_cast<uint64>(vmStats.free_count +
        vmStats.inactive_count +
        vmStats.speculative_count) * pageSize;

    usage.usedPhysicalBytes = usedMemory;
    usage.availablePhysicalBytes = availableMemory;

    struct xsw_usage swapUsage;
    mib[0] = CTL_VM;
    mib[1] = VM_SWAPUSAGE;
    length = sizeof(swapUsage);
    if (sysctl(mib, 2, &swapUsage, &length, nullptr, 0) == 0)
    {
        usage.totalSwapBytes = swapUsage.xsu_total;
        usage.usedSwapBytes = swapUsage.xsu_used;
    }
    else
    {
        usage.totalSwapBytes = 0;
        usage.usedSwapBytes = 0;
    }

    usage.totalVirtualBytes = usage.totalPhysicalBytes + usage.totalSwapBytes;
    usage.usedVirtualBytes = usage.usedPhysicalBytes + usage.usedSwapBytes;

    if (usage.totalPhysicalBytes > 0)
    {
        usage.physicalUsagePercent = (static_cast<double>(usage.usedPhysicalBytes) /
            static_cast<double>(usage.totalPhysicalBytes)) * 100.0;
    }

    if (usage.totalVirtualBytes > 0)
    {
        usage.virtualUsagePercent = (static_cast<double>(usage.usedVirtualBytes) /
            static_cast<double>(usage.totalVirtualBytes)) * 100.0;
    }

    usage.hasValidData = true;
    return true;

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

std::string Skyfire::FormatSystemMemoryUsage(SystemMemoryUsage const& usage)
{
    if (!usage.hasValidData)
        return "System memory info unavailable";

    std::vector<std::string> fields;

    AppendMemoryField(fields, "Total RAM", usage.totalPhysicalBytes);
    AppendMemoryField(fields, "Used RAM", usage.usedPhysicalBytes);
    AppendMemoryField(fields, "Available RAM", usage.availablePhysicalBytes);
    AppendPercentageField(fields, "RAM Usage", usage.physicalUsagePercent);

    if (usage.totalSwapBytes > 0)
    {
        AppendMemoryField(fields, "Total Swap", usage.totalSwapBytes);
        AppendMemoryField(fields, "Used Swap", usage.usedSwapBytes);
    }

    if (usage.totalVirtualBytes > 0)
    {
        AppendMemoryField(fields, "Total Virtual", usage.totalVirtualBytes);
        AppendMemoryField(fields, "Used Virtual", usage.usedVirtualBytes);
        AppendPercentageField(fields, "Virtual Usage", usage.virtualUsagePercent);
    }

    std::ostringstream stream;
    for (size_t i = 0; i < fields.size(); ++i)
    {
        if (i != 0)
            stream << ", ";

        stream << fields[i];
    }

    return stream.str();
}

std::string Skyfire::FormatMemoryUsageSummary(ProcessMemoryUsage const& processUsage,
    SystemMemoryUsage const& systemUsage)
{
    std::ostringstream stream;

    stream << "Process: " << FormatProcessMemoryUsage(processUsage);

    if (systemUsage.hasValidData)
    {
        stream << " | System: ";
        stream << "RAM " << std::fixed << std::setprecision(1)
            << systemUsage.physicalUsagePercent << "% ";
        stream << "(" << FormatMemorySize(systemUsage.usedPhysicalBytes) << "/"
            << FormatMemorySize(systemUsage.totalPhysicalBytes) << ")";

        if (systemUsage.totalSwapBytes > 0)
        {
            stream << ", Swap " << std::fixed << std::setprecision(1)
                << ((systemUsage.totalSwapBytes > 0) ?
                    (static_cast<double>(systemUsage.usedSwapBytes) /
                        static_cast<double>(systemUsage.totalSwapBytes) * 100.0) : 0.0) << "%";
        }
    }
    else
    {
        stream << " | System info unavailable";
    }

    return stream.str();
}