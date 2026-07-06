/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Log.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "WorldSession.h"

void WorldSession::HandleGrantLevel(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_GRANT_LEVEL");

    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 2, 1, 5, 3, 7, 4, 0, 6);
    recvData.ReadGuidBytes(guid, 1, 4, 2, 7, 5, 3, 6, 0);

    Player* target = ObjectAccessor::GetObjectInWorld(guid, _player);

    // check cheating
    uint8 levels = _player->GetGrantableLevels();
    uint8 error = 0;
    if (!target)
        error = ERR_REFER_A_FRIEND_NO_TARGET;
    else if (levels == 0)
        error = ERR_REFER_A_FRIEND_INSUFFICIENT_GRANTABLE_LEVELS;
    else if (GetRecruiterId() != target->GetSession()->GetAccountId())
        error = ERR_REFER_A_FRIEND_NOT_REFERRED_BY;
    else if (target->GetTeamId() != _player->GetTeamId())
        error = ERR_REFER_A_FRIEND_DIFFERENT_FACTION;
    else if (target->getLevel() >= _player->getLevel())
        error = ERR_REFER_A_FRIEND_TARGET_TOO_HIGH;
    else if (target->getLevel() >= sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL))
        error = ERR_REFER_A_FRIEND_GRANT_LEVEL_MAX_I;
    else if (target->GetGroup() != _player->GetGroup())
        error = ERR_REFER_A_FRIEND_NOT_IN_GROUP;

    if (error)
    {
        WorldPacket data(SMSG_REFER_A_FRIEND_FAILURE, 24);
        data << uint32(error);
        if (error == ERR_REFER_A_FRIEND_NOT_IN_GROUP)
            data << target->GetName();

        SendPacket(&data);
        return;
    }

    WorldPacket data2(SMSG_PROPOSE_LEVEL_GRANT, 8);

    data2.WriteGuidMask(guid, 6, 7, 2, 5, 3, 0, 1, 4);
    data2.WriteGuidBytes(guid, 2, 5, 6, 7, 1, 4, 3, 0);

    target->GetSession()->SendPacket(&data2);
}

void WorldSession::HandleAcceptGrantLevel(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_ACCEPT_LEVEL_GRANT");

    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 2, 7, 5, 4, 3, 0, 1, 6);
    recvData.ReadGuidBytes(guid, 5, 3, 2, 7, 4, 1, 0, 6);

    Player* other = ObjectAccessor::GetObjectInWorld(guid, _player);
    if (!(other && other->GetSession()))
        return;

    if (GetAccountId() != other->GetSession()->GetRecruiterId())
        return;

    if (other->GetGrantableLevels())
        other->SetGrantableLevels(other->GetGrantableLevels() - 1);
    else
        return;

    _player->GiveLevel(_player->getLevel() + 1);
}
