/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BattlePet.h"
#include "Common.h"
#include "DB2Enums.h"
#include "DB2Stores.h"
#include "DBCStores.h"

#include <algorithm>

uint16 BattlePetHealthFromPercent(uint16 maxHealth, uint8 percent)
{
    if (!maxHealth || !percent)
        return 0;

    if (percent >= 100)
        return maxHealth;

    return uint16(std::min<uint32>(maxHealth, (uint32(maxHealth) * percent + 99) / 100));
}

uint16 BattlePetExperienceForNextLevel(uint8 level)
{
    if (!level || level >= BATTLE_PET_MAX_LEVEL)
        return 0;

    uint32 const levelIndex = uint32(level) - 1;
    GtBattlePetXpEntry const* base = sGtBattlePetXpStore.LookupEntry(100 + levelIndex);
    GtBattlePetXpEntry const* multiplier = sGtBattlePetXpStore.LookupEntry(levelIndex);
    if (base && multiplier)
    {
        float const threshold = base->value * multiplier->value;
        if (threshold > 0.0f)
            return uint16(std::min<uint32>(uint32(threshold + 0.5f), 0xFFFF));
    }

    return uint16(level) * 100;
}

uint16 BattlePetExperienceReward(uint8 petLevel, uint8 enemyLevel, uint8 participatingPetCount)
{
    if (!petLevel || !enemyLevel || !participatingPetCount || petLevel >= BATTLE_PET_MAX_LEVEL)
        return 0;

    int32 levelDifference = int32(enemyLevel) - int32(petLevel);
    levelDifference = std::min<int32>(levelDifference, 2);
    levelDifference = std::max<int32>(levelDifference, -4);

    uint32 const reward = (uint32(enemyLevel) + 9) * uint32(levelDifference + 5);
    return uint16(std::min<uint32>(reward / participatingPetCount, 0xFFFF));
}

uint8 BattlePetNormalizeWildLevel(uint8 level)
{
    if (!level)
        return 1;

    return std::min<uint8>(level, BATTLE_PET_MAX_LEVEL);
}

bool BattlePetSpeciesFlagsAllowWildCapture(uint32 flags)
{
    return (flags & BATTLE_PET_FLAG_NOT_TAMEABLE) == 0;
}

void BattlePet::CalculateStats(bool currentHealth)
{
    float basePower = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_POWER, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_POWER, m_breed);
    float baseHealth = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_STAMINA, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_STAMINA, m_breed);
    float baseSpeed = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_SPEED, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_SPEED, m_breed);

    float qualityMod = BattlePetQualityMultiplier(m_quality);

    // No round in older cpp, just for compatibility
    m_maxHealth = floor(((baseHealth * 5.0f * m_level * qualityMod) + 100.0f) + 0.5f);

    if (currentHealth)
        m_curHealth = m_maxHealth;

    m_power = floor((basePower * m_level * qualityMod) + 0.5f);
    m_speed = floor((baseSpeed * m_level * qualityMod) + 0.5f);

    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetNickname(std::string const& nickname)
{
    m_nickname = nickname;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetTimestamp(uint32 timestamp)
{
    if (m_timestamp == timestamp)
        return;

    m_timestamp = timestamp;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetQuality(ItemQualities quality)
{
    m_quality = quality;
    CalculateStats(false);
}

void BattlePet::SetXp(uint16 xp)
{
    if (m_xp == xp)
        return;

    m_xp = xp;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetCurrentHealth(uint16 health)
{
    uint16 const newHealth = std::min<uint16>(health, m_maxHealth);
    if (m_curHealth == newHealth)
        return;

    m_curHealth = newHealth;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetFlag(uint16 flag)
{
    if (HasFlag(flag))
        return;

    m_flags |= flag;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::UnSetFlag(uint16 flag)
{
    if (!HasFlag(flag))
        return;

    m_flags &= ~flag;
    m_dbState = BattlePetDbState::BATTLE_PET_DB_STATE_SAVE;
}

void BattlePet::SetLevel(uint8 level)
{
    if (!level)
        level = 1;
    else if (level > BATTLE_PET_MAX_LEVEL)
        level = BATTLE_PET_MAX_LEVEL;

    if (m_level == level)
        return;

    m_level = level;
    CalculateStats(true);
}

uint16 BattlePet::AddExperience(uint16 xp)
{
    if (!xp || m_level >= BATTLE_PET_MAX_LEVEL)
        return 0;

    uint32 remainingXp = uint32(m_xp) + xp;
    while (m_level < BATTLE_PET_MAX_LEVEL)
    {
        uint16 const nextLevelXp = BattlePetExperienceForNextLevel(m_level);
        if (!nextLevelXp || remainingXp < nextLevelXp)
            break;

        remainingXp -= nextLevelXp;
        SetLevel(m_level + 1);
    }

    if (m_level >= BATTLE_PET_MAX_LEVEL)
        remainingXp = 0;

    SetXp(uint16(std::min<uint32>(remainingXp, 0xFFFF)));
    return xp;
}
