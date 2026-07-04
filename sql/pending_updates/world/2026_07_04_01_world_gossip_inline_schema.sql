-- Convert gossip_menu_option to the inline option/action/box schema.

ALTER TABLE `gossip_menu_option`
    CHANGE COLUMN `MenuId` `MenuID` INT UNSIGNED NOT NULL DEFAULT 0,
    CHANGE COLUMN `OptionIndex` `OptionID` INT UNSIGNED NOT NULL DEFAULT 0,
    CHANGE COLUMN `OptionBroadcastTextId` `OptionBroadcastTextID` INT UNSIGNED NOT NULL DEFAULT 0,
    ADD COLUMN `ActionMenuID` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `OptionNpcflag`,
    ADD COLUMN `ActionPoiID` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `ActionMenuID`,
    ADD COLUMN `BoxCoded` TINYINT UNSIGNED NOT NULL DEFAULT 0 AFTER `ActionPoiID`,
    ADD COLUMN `BoxMoney` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `BoxCoded`,
    ADD COLUMN `BoxText` MEDIUMTEXT NULL AFTER `BoxMoney`,
    ADD COLUMN `BoxBroadcastTextID` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `BoxText`;

UPDATE `gossip_menu_option` option_row
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
    option_row.`BoxBroadcastTextID` = COALESCE(box_row.`BoxBroadcastTextId`, 0);

DROP TABLE `gossip_menu_option_action`;
DROP TABLE `gossip_menu_option_box`;
