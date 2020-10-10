
DELETE FROM `command` WHERE `name` IN ('promotion', 'promotion view', 'promotion reset');

INSERT INTO `command` (`name`, `security`, `help`) VALUES
('promotion', 1, 'Syntax: .promotion .It allows to obtain information about the promotion.'),
('promotion view', 1, 'Syntax: .promotion view #playerName. It allows to obtain information about the player in question.'),
('promotion reset', 4, 'Syntax: .promotion reset #playerName. Allows you to remove the promotion to apply for another one.');
