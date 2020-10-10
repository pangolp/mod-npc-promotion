
DELETE FROM `command` WHERE `name`='promotion view';

INSERT INTO `command` (`name`, `security`, `help`) VALUES
('promotion view', 1, 'Syntax: .promotion view #playerName. It allows to obtain information about the player in question.');