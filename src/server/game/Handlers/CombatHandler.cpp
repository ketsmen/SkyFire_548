/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "CreatureAI.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "ObjectDefines.h"
#include "Opcodes.h"
#include "Player.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleAttackSwingOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 6, 5, 7, 0, 3, 1, 4, 2);
    recvData.ReadGuidBytes(guid, 6, 7, 1, 3, 2, 0, 4, 5);

    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_ATTACKSWING Message guidlow:%u guidhigh:%u", GUID_LOPART(guid), GUID_HIPART(guid));

    Unit* pEnemy = ObjectAccessor::GetUnit(*_player, guid);

    if (!pEnemy)
    {
        // stop attack state at client
        SendAttackStop(NULL);
        return;
    }

    if (!_player->IsValidAttackTarget(pEnemy))
    {
        // stop attack state at client
        SendAttackStop(pEnemy);
        return;
    }

    //! Client explicitly checks the following before sending CMSG_ATTACKSWING packet,
    //! so we'll place the same check here. Note that it might be possible to reuse this snippet
    //! in other places as well.
    if (Vehicle* vehicle = _player->GetVehicle())
    {
        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(_player);
        ASSERT(seat);
        if (!(seat->m_flags & VEHICLE_SEAT_FLAG_CAN_ATTACK))
        {
            SendAttackStop(pEnemy);
            return;
        }
    }

    _player->Attack(pEnemy, true);
}

void WorldSession::HandleAttackStopOpcode(WorldPacket& /*recvData*/)
{
    GetPlayer()->AttackStop();
}

void WorldSession::HandleSetSheathedOpcode(WorldPacket& recvData)
{
    uint32 sheathed;
    bool hasData = false;

    recvData >> sheathed;
    hasData = recvData.ReadBit();

    //SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_SETSHEATHED Message guidlow:%u value1:%u", GetPlayer()->GetGUIDLow(), sheathed);

    if (hasData)
    {
        if (sheathed >= MAX_SHEATH_STATE)
        {
            SF_LOG_ERROR("network", "Unknown sheath state %u ??", sheathed);
            return;
        }

        GetPlayer()->SetSheath(SheathState(sheathed));
    }
}

void WorldSession::SendAttackStop(Unit const* enemy)
{
    WorldPacket data(SMSG_ATTACKSTOP, (8 + 8 + 4));             // we guess size
    data.append(GetPlayer()->GetPackGUID());
    data.append(enemy ? enemy->GetPackGUID() : 0);          // must be packed guid
    data << uint32(0);                                      // unk, can be 1 also
    SendPacket(&data);
}
