/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_GM_NOTE_UTILS_H
#define SKYFIRE_GM_NOTE_UTILS_H

#include "Define.h"

#include <string>

namespace Skyfire
{
    struct GMNoteValidation
    {
        bool IsValid = false;
        char const* Message = "";
    };

    struct GMNoteLocation
    {
        uint32 MapId = 0;
        uint32 ZoneId = 0;
        uint32 AreaId = 0;
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
        float Orientation = 0.0f;
    };

    constexpr size_t GMNoteMaxLength = 255;

    GMNoteValidation ValidateGMNoteText(std::string const& note);
    std::string FormatGMNoteLocation(GMNoteLocation const& location);
}

#endif
