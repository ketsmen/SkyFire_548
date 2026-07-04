-- Merge preserved 5.4.8 gossip rows back into the inline gossip schema.

INSERT INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`)
SELECT `MenuID`, `TextID`, `VerifiedBuild`
FROM `gossip_menu_548_backup`
ON DUPLICATE KEY UPDATE
    `VerifiedBuild` = VALUES(`VerifiedBuild`);

INSERT INTO `gossip_menu_option` (
    `MenuID`,
    `OptionID`,
    `OptionIcon`,
    `OptionText`,
    `OptionBroadcastTextID`,
    `OptionType`,
    `OptionNpcflag`,
    `ActionMenuID`,
    `ActionPoiID`,
    `BoxCoded`,
    `BoxMoney`,
    `BoxText`,
    `BoxBroadcastTextID`,
    `VerifiedBuild`
)
SELECT
    option_row.`MenuId`,
    option_row.`OptionIndex`,
    option_row.`OptionIcon`,
    option_row.`OptionText`,
    option_row.`OptionBroadcastTextId`,
    option_row.`OptionType`,
    option_row.`OptionNpcflag`,
    COALESCE(action_row.`ActionMenuId`, 0),
    COALESCE(action_row.`ActionPoiId`, 0),
    COALESCE(box_row.`BoxCoded`, 0),
    COALESCE(box_row.`BoxMoney`, 0),
    box_row.`BoxText`,
    COALESCE(box_row.`BoxBroadcastTextId`, 0),
    option_row.`VerifiedBuild`
FROM `gossip_menu_option_548_backup` option_row
LEFT JOIN `gossip_menu_option_action_548_backup` action_row
    ON action_row.`MenuId` = option_row.`MenuId`
    AND action_row.`OptionIndex` = option_row.`OptionIndex`
LEFT JOIN `gossip_menu_option_box_548_backup` box_row
    ON box_row.`MenuId` = option_row.`MenuId`
    AND box_row.`OptionIndex` = option_row.`OptionIndex`
ON DUPLICATE KEY UPDATE
    `OptionIcon` = VALUES(`OptionIcon`),
    `OptionText` = VALUES(`OptionText`),
    `OptionBroadcastTextID` = VALUES(`OptionBroadcastTextID`),
    `OptionType` = VALUES(`OptionType`),
    `OptionNpcflag` = VALUES(`OptionNpcflag`),
    `ActionMenuID` = VALUES(`ActionMenuID`),
    `ActionPoiID` = VALUES(`ActionPoiID`),
    `BoxCoded` = VALUES(`BoxCoded`),
    `BoxMoney` = VALUES(`BoxMoney`),
    `BoxText` = VALUES(`BoxText`),
    `BoxBroadcastTextID` = VALUES(`BoxBroadcastTextID`),
    `VerifiedBuild` = VALUES(`VerifiedBuild`);

INSERT INTO `gossip_menu_option_locale` (`MenuID`, `OptionID`, `Locale`, `OptionText`, `BoxText`)
SELECT `MenuID`, `OptionID`, `Locale`, `OptionText`, `BoxText`
FROM `gossip_menu_option_locale_548_backup`
ON DUPLICATE KEY UPDATE
    `OptionText` = VALUES(`OptionText`),
    `BoxText` = VALUES(`BoxText`);
