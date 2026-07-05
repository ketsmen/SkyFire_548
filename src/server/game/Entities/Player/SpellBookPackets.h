/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SpellBookPackets_h__
#define SpellBookPackets_h__

#include "Common.h"
#include "WorldPacket.h"

#include <utility>
#include <vector>

namespace Skyfire::SpellBook
{
    using SupercededSpellPair = std::pair<uint32, uint32>;

    WorldPacket BuildSupercededSpellPacket(std::vector<SupercededSpellPair> const& spellPairs);
}

#endif // SpellBookPackets_h__
