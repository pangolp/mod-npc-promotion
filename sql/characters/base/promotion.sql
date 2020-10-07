
CREATE TABLE IF NOT EXISTS `promotion` (
	`accountId` int(11) unsigned NOT NULL,
	`accountName` varchar(32) NOT NULL DEFAULT '',
	`characterName` varchar(12) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
