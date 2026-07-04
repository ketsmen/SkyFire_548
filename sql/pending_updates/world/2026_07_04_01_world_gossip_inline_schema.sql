-- Convert gossip_menu_option to the inline option/action/box schema.

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'MenuId'
    ),
    'ALTER TABLE `gossip_menu_option` CHANGE COLUMN `MenuId` `MenuID` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'OptionIndex'
    ),
    'ALTER TABLE `gossip_menu_option` CHANGE COLUMN `OptionIndex` `OptionID` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'OptionBroadcastTextId'
    ),
    'ALTER TABLE `gossip_menu_option` CHANGE COLUMN `OptionBroadcastTextId` `OptionBroadcastTextID` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'npc_option_npcflag'
    ),
    'ALTER TABLE `gossip_menu_option` CHANGE COLUMN `npc_option_npcflag` `OptionNpcflag` BIGINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'OptionNpcFlag'
    ),
    'ALTER TABLE `gossip_menu_option` CHANGE COLUMN `OptionNpcFlag` `OptionNpcflag` BIGINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'OptionNpcflag'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `OptionNpcflag` BIGINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option_548_backup'
            AND COLUMN_NAME = 'npc_option_npcflag'
    ),
    'ALTER TABLE `gossip_menu_option_548_backup` CHANGE COLUMN `npc_option_npcflag` `OptionNpcflag` BIGINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option_548_backup'
            AND COLUMN_NAME = 'OptionNpcFlag'
    ),
    'ALTER TABLE `gossip_menu_option_548_backup` CHANGE COLUMN `OptionNpcFlag` `OptionNpcflag` BIGINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.TABLES
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option_548_backup'
    )
    AND NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option_548_backup'
            AND COLUMN_NAME = 'OptionNpcflag'
    ),
    'ALTER TABLE `gossip_menu_option_548_backup` ADD COLUMN `OptionNpcflag` BIGINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'ActionMenuID'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `ActionMenuID` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'ActionPoiID'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `ActionPoiID` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'BoxCoded'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `BoxCoded` TINYINT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'BoxMoney'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `BoxMoney` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'BoxText'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `BoxText` MEDIUMTEXT NULL',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @stmt := IF(
    NOT EXISTS (
        SELECT 1 FROM INFORMATION_SCHEMA.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE()
            AND TABLE_NAME = 'gossip_menu_option'
            AND COLUMN_NAME = 'BoxBroadcastTextID'
    ),
    'ALTER TABLE `gossip_menu_option` ADD COLUMN `BoxBroadcastTextID` INT UNSIGNED NOT NULL DEFAULT 0',
    'SELECT 1'
);
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @has_action_table := EXISTS (
    SELECT 1 FROM INFORMATION_SCHEMA.TABLES
    WHERE TABLE_SCHEMA = DATABASE()
        AND TABLE_NAME = 'gossip_menu_option_action'
);

SET @has_box_table := EXISTS (
    SELECT 1 FROM INFORMATION_SCHEMA.TABLES
    WHERE TABLE_SCHEMA = DATABASE()
        AND TABLE_NAME = 'gossip_menu_option_box'
);

SET @stmt := CASE
    WHEN @has_action_table AND @has_box_table THEN
        'UPDATE `gossip_menu_option` option_row
        LEFT JOIN `gossip_menu_option_action` action_row
            ON action_row.`MenuId` = option_row.`MenuID`
            AND action_row.`OptionIndex` = option_row.`OptionID`
        LEFT JOIN `gossip_menu_option_box` box_row
            ON box_row.`MenuId` = option_row.`MenuID`
            AND box_row.`OptionIndex` = option_row.`OptionID`
        SET
            option_row.`ActionMenuID` = COALESCE(action_row.`ActionMenuId`, 0),
            option_row.`ActionPoiID` = COALESCE(action_row.`ActionPoiId`, 0),
            option_row.`BoxCoded` = COALESCE(box_row.`BoxCoded`, 0),
            option_row.`BoxMoney` = COALESCE(box_row.`BoxMoney`, 0),
            option_row.`BoxText` = box_row.`BoxText`,
            option_row.`BoxBroadcastTextID` = COALESCE(box_row.`BoxBroadcastTextId`, 0)'
    WHEN @has_action_table THEN
        'UPDATE `gossip_menu_option` option_row
        LEFT JOIN `gossip_menu_option_action` action_row
            ON action_row.`MenuId` = option_row.`MenuID`
            AND action_row.`OptionIndex` = option_row.`OptionID`
        SET
            option_row.`ActionMenuID` = COALESCE(action_row.`ActionMenuId`, 0),
            option_row.`ActionPoiID` = COALESCE(action_row.`ActionPoiId`, 0)'
    WHEN @has_box_table THEN
        'UPDATE `gossip_menu_option` option_row
        LEFT JOIN `gossip_menu_option_box` box_row
            ON box_row.`MenuId` = option_row.`MenuID`
            AND box_row.`OptionIndex` = option_row.`OptionID`
        SET
            option_row.`BoxCoded` = COALESCE(box_row.`BoxCoded`, 0),
            option_row.`BoxMoney` = COALESCE(box_row.`BoxMoney`, 0),
            option_row.`BoxText` = box_row.`BoxText`,
            option_row.`BoxBroadcastTextID` = COALESCE(box_row.`BoxBroadcastTextId`, 0)'
    ELSE 'SELECT 1'
END;
PREPARE stmt FROM @stmt;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

DROP TABLE IF EXISTS `gossip_menu_option_action`;
DROP TABLE IF EXISTS `gossip_menu_option_box`;
