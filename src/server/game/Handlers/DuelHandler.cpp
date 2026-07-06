/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Player.h"
#include "UpdateData.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#define SPELL_DUEL         7266
#define SPELL_MOUNTED_DUEL 62875

void WorldSession::HandleDuelProposedOpcode(WorldPacket& recvPacket)
{
    ObjectGuid guid;

    recvPacket.ReadGuidMask(guid, 1, 5, 4, 6, 3, 2, 7, 0);
    recvPacket.ReadGuidBytes(guid, 4, 2, 5, 7, 1, 3, 6, 0);

    if (Player* player = sObjectAccessor->FindPlayer(guid))
    {
        if (_player->IsMounted())
            _player->CastSpell(player, SPELL_MOUNTED_DUEL, false);
        else
            _player->CastSpell(player, SPELL_DUEL, false);
    }
}

void WorldSession::HandleDuelResponseOpcode(WorldPacket& recvPacket)
{
    bool accepted;
    ObjectGuid guid;
    Player* player;

    recvPacket.ReadGuidMask(guid, 7, 1, 3, 4, 0, 2, 6);
    accepted = recvPacket.ReadBit();
    recvPacket.ReadGuidMask(guid, 5);

    recvPacket.ReadGuidBytes(guid, 6, 4, 5, 0, 1, 2, 7, 3);

    if (!GetPlayer()->duel)                                  // ignore accept from duel-sender
        return;

    if (accepted)
    {
        player = GetPlayer();
        Player* plTarget = player->duel->opponent;

        if (player == player->duel->initiator || !plTarget || player == plTarget || player->duel->startTime != 0 || plTarget->duel->startTime != 0)
            return;

        //SF_LOG_DEBUG("network", "WORLD: Received CMSG_DUEL_ACCEPTED");
        SF_LOG_DEBUG("network", "Player 1 is: %u (%s)", player->GetGUIDLow(), player->GetName().c_str());
        SF_LOG_DEBUG("network", "Player 2 is: %u (%s)", plTarget->GetGUIDLow(), plTarget->GetName().c_str());

        time_t now = time(NULL);
        player->duel->startTimer = now;
        plTarget->duel->startTimer = now;

        player->SendDuelCountdown(3000);
        plTarget->SendDuelCountdown(3000);
    }
    else
    {
        // player surrendered in a duel using /forfeit
        if (GetPlayer()->duel->startTime != 0)
        {
            GetPlayer()->CombatStopWithPets(true);
            if (GetPlayer()->duel->opponent)
                GetPlayer()->duel->opponent->CombatStopWithPets(true);

            GetPlayer()->CastSpell(GetPlayer(), 7267, true);    // beg
            GetPlayer()->DuelComplete(DuelCompleteType::DUEL_WON);
            return;
        }

        GetPlayer()->DuelComplete(DuelCompleteType::DUEL_INTERRUPTED);
    }
}
