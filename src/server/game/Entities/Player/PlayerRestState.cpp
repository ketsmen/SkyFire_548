/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "PlayerRestState.h"

#include <cmath>

namespace Skyfire
{
namespace Rest
{
    namespace
    {
        constexpr double TwoPi = 6.28318530717958647692;
    }

    bool HasInnAreaBounds(InnAreaBounds const& bounds)
    {
        return bounds.Radius > 0.0f || bounds.BoxX > 0.0f || bounds.BoxY > 0.0f || bounds.BoxZ > 0.0f;
    }

    bool IsInsideInnArea(InnAreaBounds const& bounds, uint32 mapId, float x, float y, float z, float padding)
    {
        if (mapId != bounds.MapId)
            return false;

        if (bounds.Radius > 0.0f)
        {
            float const dx = x - bounds.X;
            float const dy = y - bounds.Y;
            float const dz = z - bounds.Z;
            float const distanceSquared = dx * dx + dy * dy + dz * dz;
            float const allowedDistance = bounds.Radius + padding;
            return distanceSquared <= allowedDistance * allowedDistance;
        }

        if (bounds.BoxX <= 0.0f || bounds.BoxY <= 0.0f || bounds.BoxZ <= 0.0f)
            return false;

        double const rotation = TwoPi - bounds.BoxOrientation;
        double const sinVal = std::sin(rotation);
        double const cosVal = std::cos(rotation);

        float const boxDistX = x - bounds.X;
        float const boxDistY = y - bounds.Y;

        float const rotatedX = float(bounds.X + boxDistX * cosVal - boxDistY * sinVal);
        float const rotatedY = float(bounds.Y + boxDistY * cosVal + boxDistX * sinVal);

        return std::fabs(rotatedX - bounds.X) <= bounds.BoxX / 2.0f + padding &&
            std::fabs(rotatedY - bounds.Y) <= bounds.BoxY / 2.0f + padding &&
            std::fabs(z - bounds.Z) <= bounds.BoxZ / 2.0f + padding;
    }
}
}
