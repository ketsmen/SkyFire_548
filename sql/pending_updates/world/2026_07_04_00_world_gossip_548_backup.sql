-- Preserve the current 5.4.8 gossip data before switching to inline gossip options.

DROP TABLE IF EXISTS `gossip_menu_548_backup`;
CREATE TABLE `gossip_menu_548_backup` LIKE `gossip_menu`;
INSERT INTO `gossip_menu_548_backup` SELECT * FROM `gossip_menu`;

DROP TABLE IF EXISTS `gossip_menu_option_548_backup`;
CREATE TABLE `gossip_menu_option_548_backup` LIKE `gossip_menu_option`;
INSERT INTO `gossip_menu_option_548_backup` SELECT * FROM `gossip_menu_option`;

DROP TABLE IF EXISTS `gossip_menu_option_action_548_backup`;
CREATE TABLE `gossip_menu_option_action_548_backup` LIKE `gossip_menu_option_action`;
INSERT INTO `gossip_menu_option_action_548_backup` SELECT * FROM `gossip_menu_option_action`;

DROP TABLE IF EXISTS `gossip_menu_option_box_548_backup`;
CREATE TABLE `gossip_menu_option_box_548_backup` LIKE `gossip_menu_option_box`;
INSERT INTO `gossip_menu_option_box_548_backup` SELECT * FROM `gossip_menu_option_box`;

DROP TABLE IF EXISTS `gossip_menu_option_locale_548_backup`;
CREATE TABLE `gossip_menu_option_locale_548_backup` LIKE `gossip_menu_option_locale`;
INSERT INTO `gossip_menu_option_locale_548_backup` SELECT * FROM `gossip_menu_option_locale`;
