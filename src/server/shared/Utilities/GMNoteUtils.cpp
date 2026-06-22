/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "GMNoteUtils.h"

#include <iomanip>
#include <sstream>

Skyfire::GMNoteValidation Skyfire::ValidateGMNoteText(std::string const& note)
{
    if (note.empty())
        return { false, "Note text is required." };

    if (note.size() > GMNoteMaxLength)
        return { false, "GM notes are limited to 255 characters." };

    return { true, "" };
}

std::string Skyfire::FormatGMNoteLocation(GMNoteLocation const& location)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2)
           << "map=" << location.MapId
           << " zone=" << location.ZoneId
           << " area=" << location.AreaId
           << " x=" << location.X
           << " y=" << location.Y
           << " z=" << location.Z
           << " o=" << location.Orientation;

    return stream.str();
}
