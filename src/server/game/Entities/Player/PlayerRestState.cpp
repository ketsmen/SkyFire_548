/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PlayerRestState.h"

#include <cmath>

namespace
{
    constexpr double TwoPi = 6.28318530717958647692;
}

namespace Skyfire
{
namespace Rest
{
    bool IsInsideInnArea(InnAreaBounds const& bounds, uint32 playerMapId, float playerX, float playerY, float playerZ, float padding)
    {
        if (playerMapId != bounds.MapId)
            return false;

        if (bounds.Radius > 0.0f)
        {
            float const dx = playerX - bounds.X;
            float const dy = playerY - bounds.Y;
            float const dz = playerZ - bounds.Z;
            float const distance = std::sqrt(dx * dx + dy * dy + dz * dz);
            return distance <= bounds.Radius + padding;
        }

        double const rotation = TwoPi - bounds.BoxOrientation;
        double const sinVal = std::sin(rotation);
        double const cosVal = std::cos(rotation);

        float const playerBoxDistX = playerX - bounds.X;
        float const playerBoxDistY = playerY - bounds.Y;

        float const rotPlayerX = float(bounds.X + playerBoxDistX * cosVal - playerBoxDistY * sinVal);
        float const rotPlayerY = float(bounds.Y + playerBoxDistY * cosVal + playerBoxDistX * sinVal);

        float const dx = rotPlayerX - bounds.X;
        float const dy = rotPlayerY - bounds.Y;
        float const dz = playerZ - bounds.Z;

        return std::fabs(dx) <= bounds.BoxX / 2.0f + padding &&
            std::fabs(dy) <= bounds.BoxY / 2.0f + padding &&
            std::fabs(dz) <= bounds.BoxZ / 2.0f + padding;
    }
}
}
