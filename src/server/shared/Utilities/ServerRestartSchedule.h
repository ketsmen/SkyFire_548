/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_SERVER_RESTART_SCHEDULE_H
#define SKYFIRE_SERVER_RESTART_SCHEDULE_H

#include "Define.h"

#include <ctime>

namespace Skyfire
{
    namespace ServerRestartSchedule
    {
        bool ParseTimeOfDay(char const* text, uint32& secondsOfDay);
        bool CalculateRestartDelay(char const* text, uint32 currentSecondsOfDay, uint32& delaySeconds);
        bool CalculateRestartDelay(char const* text, time_t now, uint32& delaySeconds);
        bool CalculateRestartDelay(char const* text, uint32& delaySeconds);
    }
}

#endif
