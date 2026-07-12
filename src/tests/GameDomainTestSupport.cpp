/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ByteBuffer.h"
#include "Common.h"
#include "Errors.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

double rand_norm(void)
{
    return 0.25;
}

uint32 BattlePetGetSpeciesAbility(uint16 speciesId, uint8 abilitySlot, uint16 flags, uint8 level)
{
    return uint32(speciesId) * 1000 + uint32(level) * 10 + flags + abilitySlot;
}

float BattlePetSpeciesMainStat(uint16 stateId, uint16 /*speciesId*/)
{
    switch (stateId)
    {
        case 18:
            return 10.0f;
        case 19:
            return 20.0f;
        case 20:
            return 30.0f;
        default:
            return 0.0f;
    }
}

float BattlePetBreedMainStatModifier(uint16 stateId, uint8 breedId)
{
    switch (stateId)
    {
        case 18:
        case 19:
        case 20:
            return float(breedId);
        default:
            return 0.0f;
    }
}

float BattlePetQualityMultiplier(uint8 quality)
{
    return quality ? float(quality) : 1.0f;
}

ByteBufferPositionException::ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize)
{
    std::ostringstream ss;
    ss << "Attempted to " << (add ? "put" : "get") << " value with size: "
        << valueSize << " in ByteBuffer (pos: " << pos << " size: " << size << ")";

    message().assign(ss.str());
}

ByteBufferSourceException::ByteBufferSourceException(size_t pos, size_t size, size_t valueSize)
{
    std::ostringstream ss;
    ss << "Attempted to put a "
        << (valueSize > 0 ? "NULL-pointer" : "zero-sized value")
        << " in ByteBuffer (pos: " << pos << " size: " << size << ")";

    message().assign(ss.str());
}

namespace Skyfire
{
    void Assert(char const* file, int line, char const* function, char const* message)
    {
        std::cerr << file << ':' << line << " in " << function << " ASSERTION FAILED: " << message << '\n';
        std::abort();
    }
}
