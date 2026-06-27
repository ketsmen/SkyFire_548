-- areatrigger_tavern: correct Goldshire Lion's Pride Inn rest trigger for MoP

DELETE FROM `areatrigger_tavern`
WHERE `id` IN (562, 5830);

INSERT INTO `areatrigger_tavern` (`id`, `name`) VALUES
(5830, 'Elwynn Forest - Goldshire - Lion''s Pride Inn');
