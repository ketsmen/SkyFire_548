/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ServerRestartSchedule.h"

#include "Platform/TimeUtils.h"

#include <cctype>

namespace
{
    constexpr uint32 SecondsPerMinute = 60;
    constexpr uint32 SecondsPerHour = 60 * SecondsPerMinute;
    constexpr uint32 SecondsPerDay = 24 * SecondsPerHour;

    bool IsDigit(char value)
    {
        return std::isdigit(static_cast<unsigned char>(value)) != 0;
    }

    bool ParseTwoDigits(char const*& cursor, uint32& value)
    {
        if (!IsDigit(cursor[0]) || !IsDigit(cursor[1]))
            return false;

        value = uint32(cursor[0] - '0') * 10 + uint32(cursor[1] - '0');
        cursor += 2;
        return true;
    }
}

bool Skyfire::ServerRestartSchedule::ParseTimeOfDay(char const* text, uint32& secondsOfDay)
{
    secondsOfDay = 0;

    if (!text || !*text || !IsDigit(*text))
        return false;

    char const* cursor = text;
    uint32 hour = uint32(*cursor - '0');
    ++cursor;

    if (IsDigit(*cursor))
    {
        hour = hour * 10 + uint32(*cursor - '0');
        ++cursor;
    }

    if (*cursor != ':')
        return false;

    ++cursor;

    uint32 minute = 0;
    if (!ParseTwoDigits(cursor, minute))
        return false;

    uint32 second = 0;
    if (*cursor == ':')
    {
        ++cursor;

        if (!ParseTwoDigits(cursor, second))
            return false;
    }

    if (*cursor != '\0' || hour > 23 || minute > 59 || second > 59)
        return false;

    secondsOfDay = hour * SecondsPerHour + minute * SecondsPerMinute + second;
    return true;
}

bool Skyfire::ServerRestartSchedule::CalculateRestartDelay(char const* text, uint32 currentSecondsOfDay, uint32& delaySeconds)
{
    delaySeconds = 0;

    if (currentSecondsOfDay >= SecondsPerDay)
        return false;

    uint32 targetSecondsOfDay = 0;
    if (!ParseTimeOfDay(text, targetSecondsOfDay))
        return false;

    delaySeconds = targetSecondsOfDay >= currentSecondsOfDay
        ? targetSecondsOfDay - currentSecondsOfDay
        : SecondsPerDay - currentSecondsOfDay + targetSecondsOfDay;

    return true;
}

bool Skyfire::ServerRestartSchedule::CalculateRestartDelay(char const* text, time_t now, uint32& delaySeconds)
{
    tm localTime;
    if (!Skyfire::LocalTime(now, localTime))
        return false;

    uint32 currentSecondsOfDay = uint32(localTime.tm_hour) * SecondsPerHour
        + uint32(localTime.tm_min) * SecondsPerMinute
        + uint32(localTime.tm_sec);

    return CalculateRestartDelay(text, currentSecondsOfDay, delaySeconds);
}

bool Skyfire::ServerRestartSchedule::CalculateRestartDelay(char const* text, uint32& delaySeconds)
{
    return CalculateRestartDelay(text, time(NULL), delaySeconds);
}
