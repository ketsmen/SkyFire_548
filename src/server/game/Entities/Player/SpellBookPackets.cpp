/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SpellBookPackets.h"

namespace Skyfire::SpellBook
{
    WorldPacket BuildSupercededSpellPacket(std::vector<SupercededSpellPair> const& spellPairs)
    {
        WorldPacket data(SMSG_SUPERCEDED_SPELL);
        data.WriteBits(spellPairs.size(), 22);
        data.WriteBits(spellPairs.size(), 22);
        data.FlushBits();

        for (SupercededSpellPair const& spellPair : spellPairs)
        {
            data << uint32(spellPair.first);
            data << uint32(spellPair.second);
        }

        return data;
    }
}
