
SET @MENUID:=62000;

DELETE FROM `gossip_menu_option` WHERE `MenuID`=@MENUID AND `OptionID` BETWEEN 0 AND 21;

INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@MENUID, 0, 0, "WARRIOR TANK", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 1, 0, "WARRIOR DPS", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 2, 0, "PALADIN TANK", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 3, 0, "PALADIN HEAL", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 4, 0, "PALADIN DPS", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 5, 0, "HUNTER", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 6, 0, "ROGUE", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 7, 0, "PRIEST HEAL", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 8, 0, "PRIEST DPS", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 9, 0, "DK TANK", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 10, 0, "DK DPS", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 11, 0, "SHAMAN CASTER", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 12, 0, "SHAMAN MELEE", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 13, 0, "SHAMAN HEAL", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 14, 0, "MAGE", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 15, 0, "WARLOCK", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 16, 0, "DRUID TANK", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 17, 0, "DRUID HEAL", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 18, 0, "DRUID CASTER", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 19, 0, "DRUID MELEE", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 20, 0, "CLOSED MENU", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 21, 0, "GO TO DALARAN", 0, 1, 1, 0, 0, 0, 0, "", 0, 0);

DELETE FROM `gossip_menu_option_locale` WHERE `MenuID`=@MENUID AND `OptionID` BETWEEN 0 AND 21;

INSERT INTO `gossip_menu_option_locale` (`MenuID`, `OptionID`, `Locale`, `OptionText`, `BoxText`) VALUES
(@MENUID, 0, "esES", "GUERRERO TANQUE", ""),
(@MENUID, 1, "esES", "GUERRERO DPS", ""),
(@MENUID, 2, "esES", "PALADIN TANQUE", ""),
(@MENUID, 3, "esES", "PALADIN HEAL", ""),
(@MENUID, 4, "esES", "PALADIN DPS", ""),
(@MENUID, 5, "esES", "CAZADOR", ""),
(@MENUID, 6, "esES", "PICARO", ""),
(@MENUID, 7, "esES", "SACERDOTE HEAL", ""),
(@MENUID, 8, "esES", "SACERDOTE CASTER", ""),
(@MENUID, 9, "esES", "DK TANQUE", ""),
(@MENUID, 10, "esES", "DK DPS", ""),
(@MENUID, 11, "esES", "CHAMAN CASTER", ""),
(@MENUID, 12, "esES", "CHAMAN MELEE", ""),
(@MENUID, 13, "esES", "CHAMAN HEAL", ""),
(@MENUID, 14, "esES", "MAGO", ""),
(@MENUID, 15, "esES", "BRUJO", ""),
(@MENUID, 16, "esES", "DRUIDA TANQUE", ""),
(@MENUID, 17, "esES", "DRUIDA HEAL", ""),
(@MENUID, 18, "esES", "DRUIDA CASTER", ""),
(@MENUID, 19, "esES", "DRUIDA MELEE", ""),
(@MENUID, 20, "esES", "CERRAR MENU", ""),
(@MENUID, 21, "esES", "IR A DALARAN", "");
