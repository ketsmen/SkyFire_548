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

    bool HasInnAreaBounds(InnAreaBounds const& bounds);
    bool IsInsideInnArea(InnAreaBounds const& bounds, uint32 mapId, float x, float y, float z, float padding = 0.0f);
}
}

#endif
