SET @MENUID:=62000;

DELETE FROM `gossip_menu_option` WHERE `MenuID`=@MENUID;

INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@MENUID, 0, 0, "WARRIOR TANK", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 1, 0, "WARRIOR DPS", 0, 1, 1, 0, 0, 0, 0, "", 0, 0),
(@MENUID, 19, 0, "CLOSED MENU", 0, 1, 1, 0, 0, 0, 0, "", 0, 0);

DELETE FROM `gossip_menu_option_locale` WHERE `MenuID`=@MENUID;

INSERT INTO `gossip_menu_option_locale` (`MenuID`, `OptionID`, `Locale`, `OptionText`, `BoxText`) VALUES
(@MENUID, 0, "esES", "GUERRERO TANQUE", ""),
(@MENUID, 1, "esES", "GUERRERO DPS", ""),
(@MENUID, 19, "esES", "CERRAR MENU", "");
