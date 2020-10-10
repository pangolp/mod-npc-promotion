
CREATE TABLE IF NOT EXISTS `promotion` (
	`accountId` int(11) unsigned NOT NULL,
	`accountName` varchar(32) NOT NULL DEFAULT '',
	`characterName` varchar(12) NOT NULL DEFAULT '',
	`ip` varchar(15) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
	`date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
