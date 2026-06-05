/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Log.h"

Log::Log() : AppenderId(0), worker(NULL)
{
}

Log::~Log()
{
}

void Log::vlog(std::string const&, LogLevel, char const*, va_list)
{
}

LogLevel Logger::getLogLevel() const
{
    return LogLevel::LOG_LEVEL_DISABLED;
}
