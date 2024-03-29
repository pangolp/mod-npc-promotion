DROP TABLE IF EXISTS `mod_npc_promotion_log`;
CREATE TABLE IF NOT EXISTS `mod_npc_promotion_log` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `accountId` int(11) unsigned NOT NULL,
  `accountName` varchar(32) NOT NULL DEFAULT '',
  `characterName` varchar(12) NOT NULL DEFAULT '',
  `ip` varchar(15) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
