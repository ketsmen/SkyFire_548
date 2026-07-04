/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See COPYRIGHT file for Copyright information
*/

#include "SpellBookPackets.h"

#include <iostream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << "FAIL: " << message << std::endl;

        return condition;
    }

    bool TestSupercededSpellPacketLayout()
    {
        bool passed = true;

        WorldPacket packet = Skyfire::SpellBook::BuildSupercededSpellPacket({ { 205, 116 } });
        passed &= Expect(packet.GetOpcode() == SMSG_SUPERCEDED_SPELL,
            "Superceded spell packet should use SMSG_SUPERCEDED_SPELL");

        packet.rpos(0);
        uint32 firstCount = packet.ReadBits(22);
        uint32 secondCount = packet.ReadBits(22);
        passed &= Expect(firstCount == 1,
            "Superceded spell packet should write the first 22-bit replacement count");
        passed &= Expect(secondCount == 1,
            "Superceded spell packet should write the second 22-bit replacement count");

        uint32 firstSpell = 0;
        uint32 secondSpell = 0;
        packet >> firstSpell >> secondSpell;
        passed &= Expect(firstSpell == 205,
            "Superceded spell packet should write the first spell id in the pair");
        passed &= Expect(secondSpell == 116,
            "Superceded spell packet should write the second spell id in the pair");

        return passed;
    }
}

int main()
{
    bool passed = TestSupercededSpellPacketLayout();
    std::cout << (passed ? "Spell book packet tests passed" : "Spell book packet tests failed") << std::endl;
    return passed ? 0 : 1;
}
