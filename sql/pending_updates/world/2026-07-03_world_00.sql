-- =============================================================================
-- DUSKWOOD: Raven Hold Inn duplicate GOs + Cooking Fire (818) client crash
-- =============================================================================
-- Symptom:  Stack overflow relog at Raven Hold Inn (Darkshire); casting Cooking
--           Fire (818) anywhere crashes client (~12.5 GB alloc in WowClientDB2.cpp)
--           and blocks relog at logout spot until campfire despawns.
-- Cause:    (1) 15 duplicate gameobjects at identical inn positions (guids 5532-
--           5568 vs originals 1450-1489). (2) EffectTransmitted sets CREATED_BY on
--           SPELL_FOCUS GO 29784 (CREATE_OBJECT2 client crash); linked trap 31442
--           compounded the issue. GO was not tracked on player for cleanup.
-- Fix:      Delete duplicate inn spawns. Disable linked trap on 29784; SmartAI
--           Cozy Fire (7353). Requires worldserver rebuild (EffectTransmitted +
--           TrackGameObject — see src/server/game/Spells/SpellEffectsSummon.cpp).
-- Map/NPCs:  Raven Hold Inn map 0 (~ -10517, -1162, 28)
-- Spells:    818 Cooking Fire, 7353 Cozy Fire
-- GOs:       29784 Basic Campfire, 31442 linked trap (unused after data2=0)
-- Apply:     rebuild worldserver, apply SQL, .reload gameobject_template &&
--             .reload smart_scripts
-- =============================================================================
-- ROLLBACK (world_test 2026-07-03):
--   gameobject: re-insert duplicate rows guid 5532-5568 (see SFDB positions)
--   gameobject_template 29784: data2=31442, AIName=''
--   gameobject_template 31442: data6=0
--   smart_scripts: DELETE entryorguid=29784 source_type=1
-- =============================================================================

DELETE FROM `gameobject`
WHERE `guid` IN (
    5532, 5533, 5538, 5540, 5541, 5543,
    5548, 5549, 5550, 5555, 5556, 5558,
    5566, 5567, 5568
)
  AND `map` = 0;

UPDATE `gameobject_template`
SET `data2` = 0,
    `AIName` = 'SmartGameObjectAI'
WHERE `entry` = 29784;

UPDATE `gameobject_template`
SET `data6` = -1
WHERE `entry` = 31442;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 29784 AND `source_type` = 1;

INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    (29784, 1, 0, 0, 1, 0, 100, 0, 0, 2000, 2000, 2000, 0, 75, 7353, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, 'Basic Campfire - Cozy Fire aura on nearby players');
