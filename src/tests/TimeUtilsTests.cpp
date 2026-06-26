/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Platform/TimeUtils.h"
#include "ServerRestartSchedule.h"

#include <iostream>

namespace
{
    constexpr uint32 TestMinute = 60;
    constexpr uint32 TestHour = 60 * TestMinute;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool ExpectRestartDelay(char const* input, uint32 currentSecondsOfDay, uint32 expectedDelay, char const* message)
    {
        uint32 delay = 0;
        if (!Skyfire::ServerRestartSchedule::CalculateRestartDelay(input, currentSecondsOfDay, delay))
        {
            std::cerr << message << ": parser rejected '" << input << "'\n";
            return false;
        }

        if (delay == expectedDelay)
            return true;

        std::cerr << message << ": expected " << expectedDelay << ", got " << delay << '\n';
        return false;
    }

    bool ExpectInvalidRestartTime(char const* input, char const* message)
    {
        uint32 delay = 0;
        if (!Skyfire::ServerRestartSchedule::CalculateRestartDelay(input, uint32(0), delay))
            return true;

        std::cerr << message << ": parser accepted '" << input << "'\n";
        return false;
    }
}

int main()
{
    bool passed = true;

    Skyfire::SleepForMicroseconds(0);
    Skyfire::SleepForMilliseconds(0);
    Skyfire::SleepForSeconds(0);

    if (Skyfire::GetMSTimeDiff(100, 150) != 50)
    {
        std::cerr << "GetMSTimeDiff did not handle normal forward time\n";
        return 1;
    }

    if (Skyfire::GetMSTimeDiff(0xFFFFFFF0, 20) != 35)
    {
        std::cerr << "GetMSTimeDiff did not handle wrapped time\n";
        return 1;
    }

    passed &= ExpectRestartDelay("10:30", 10 * TestHour, 30 * TestMinute, "same-day restart delay failed");
    passed &= ExpectRestartDelay("00:10", 23 * TestHour + 50 * TestMinute, 20 * TestMinute, "next-day restart delay failed");
    passed &= ExpectRestartDelay("10:00", 10 * TestHour, 0, "restart at current wall-clock time should be immediate");
    passed &= ExpectRestartDelay("01:02:10", TestHour + 2 * TestMinute + 3, 7, "restart delay with seconds failed");
    passed &= ExpectRestartDelay("8:30", 8 * TestHour, 30 * TestMinute, "single-digit hour restart delay failed");

    passed &= ExpectInvalidRestartTime("", "empty restart time should fail");
    passed &= ExpectInvalidRestartTime("24:00", "hour 24 should fail");
    passed &= ExpectInvalidRestartTime("10:60", "minute 60 should fail");
    passed &= ExpectInvalidRestartTime("10:30:60", "second 60 should fail");
    passed &= ExpectInvalidRestartTime("10:3", "single-digit minute should fail");
    passed &= ExpectInvalidRestartTime("10:30x", "trailing characters should fail");

    uint32 before = Skyfire::GetMSTime();
    Skyfire::SleepFor(0);
    uint32 after = Skyfire::GetMSTime();

    if (Skyfire::GetMSTimeDiff(before, after) > 1000)
    {
        std::cerr << "SleepFor compatibility wrapper slept unexpectedly long\n";
        return 1;
    }

    return passed ? 0 : 1;
}
