/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_PLAYER_REST_STATE_H
#define SKYFIRE_PLAYER_REST_STATE_H

#include "Define.h"

namespace Skyfire
{
namespace Rest
{
    constexpr float InnAreaPadding = 5.0f;

    struct InnAreaBounds
    {
        uint32 MapId = 0;
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;
        float Radius = 0.0f;
        float BoxX = 0.0f;
        float BoxY = 0.0f;
        float BoxZ = 0.0f;
        float BoxOrientation = 0.0f;
    };

    bool IsInsideInnArea(InnAreaBounds const& bounds, uint32 playerMapId, float playerX, float playerY, float playerZ, float padding = InnAreaPadding);
}
}

#endif
