/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "MemoryUsage.h"

#include <iostream>
#include <string>

namespace
{
    bool ExpectEqual(std::string const& actual, char const* expected, char const* message)
    {
        if (actual == expected)
            return true;

        std::cerr << message << ": expected '" << expected << "', got '" << actual << "'\n";
        return false;
    }
}

int main()
{
    bool passed = true;

    passed &= ExpectEqual(Skyfire::FormatMemorySize(0), "0 B", "zero bytes formatting failed");
    passed &= ExpectEqual(Skyfire::FormatMemorySize(512), "512 B", "byte formatting failed");
    passed &= ExpectEqual(Skyfire::FormatMemorySize(1536), "1.50 KB", "kilobyte formatting failed");
    passed &= ExpectEqual(Skyfire::FormatMemorySize(1048576), "1.00 MB", "megabyte formatting failed");
    passed &= ExpectEqual(Skyfire::FormatMemorySize(1073741824), "1.00 GB", "gigabyte formatting failed");

    Skyfire::ProcessMemoryUsage usage;
    usage.residentBytes = 1048576;
    usage.peakResidentBytes = 2097152;
    usage.virtualBytes = 1073741824;
    usage.hasResidentBytes = true;
    usage.hasPeakResidentBytes = true;
    usage.hasVirtualBytes = true;

    passed &= ExpectEqual(
        Skyfire::FormatProcessMemoryUsage(usage),
        "RSS 1.00 MB, Virtual 1.00 GB, Peak RSS 2.00 MB",
        "process memory formatting failed"
    );

    return passed ? 0 : 1;
}
