SET @ENTRY:=40000;

DELETE FROM `acore_string` WHERE `entry`=@ENTRY;
INSERT INTO `acore_string` (`entry`, `content_default`, `locale_koKR`, `locale_frFR`, `locale_deDE`, `locale_zhCN`, `locale_zhTW`, `locale_esES`, `locale_esMX`, `locale_ruRU`) VALUES
(@ENTRY, "This server is running the |cff4CFF00Npc Promotion |rmodule.", NULL, NULL, NULL, NULL, NULL, "Este servidor está utilizando |cff4CFF00 mod-npc-promotion |r.", "Este servidor está utilizando |cff4CFF00 mod-npc-promotion |r.", NULL);
