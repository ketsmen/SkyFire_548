-- Convert legacy ambient critter spawns to matching battle pet creature entries.
-- Exact-name mappings generated from BattlePetSpecies.db2 and existing creature_template rows.
-- Only old creature_template rows with type = 8, rank = 0, and lootid = 0 are converted to avoid special NPCs with matching names.

UPDATE `creature`
SET
    `id` = CASE `id`
        WHEN 620 THEN 62664 -- Chicken -> Chicken, species 646, 139 spawns
        WHEN 721 THEN 61080 -- Rabbit -> Rabbit, species 378, 769 spawns
        WHEN 890 THEN 61165 -- Fawn -> Fawn, species 447, 74 spawns
        WHEN 1412 THEN 61081 -- Squirrel -> Squirrel, species 379, 1032 spawns
        WHEN 1420 THEN 61369 -- Toad -> Toad, species 420, 576 spawns
        WHEN 1933 THEN 61170 -- Sheep -> Sheep, species 394, 185 spawns
        WHEN 2110 THEN 61257 -- Black Rat -> Black Rat, species 398, 473 spawns
        WHEN 2620 THEN 61141 -- Prairie Dog -> Prairie Dog, species 386, 276 spawns
        WHEN 2914 THEN 61142 -- Snake -> Snake, species 387, 591 spawns
        WHEN 3300 THEN 61325 -- Adder -> Adder, species 635, 293 spawns
        WHEN 3835 THEN 62815 -- Biletoad -> Biletoad, species 649, 56 spawns
        WHEN 4075 THEN 61366 -- Rat -> Rat, species 417, 1284 spawns
        WHEN 4076 THEN 61169 -- Roach -> Roach, species 424, 518 spawns
        WHEN 4953 THEN 61372 -- Moccasin -> Moccasin, species 422, 204 spawns
        WHEN 5951 THEN 61751 -- Hare -> Hare, species 448, 147 spawns
        WHEN 6271 THEN 61143 -- Mouse -> Mouse, species 385, 197 spawns
        WHEN 6368 THEN 62019 -- Cat -> Cat, species 459, 41 spawns
        WHEN 6653 THEN 61368 -- Huge Toad -> Huge Toad, species 648, 59 spawns
        WHEN 6827 THEN 61158 -- Shore Crab -> Shore Crab, species 388, 1007 spawns
        WHEN 9600 THEN 61313 -- Parrot -> Parrot, species 403, 86 spawns
        WHEN 9699 THEN 61328 -- Fire Beetle -> Fire Beetle, species 415, 105 spawns
        WHEN 9700 THEN 61383 -- Lava Crab -> Lava Crab, species 423, 31 spawns
        WHEN 10779 THEN 61828 -- Infected Squirrel -> Infected Squirrel, species 627, 57 spawns
        WHEN 13321 THEN 61071 -- Small Frog -> Small Frog, species 419, 877 spawns
        WHEN 14881 THEN 61327 -- Spider -> Spider, species 412, 683 spawns
        WHEN 15475 THEN 61319 -- Beetle -> Beetle, species 406, 343 spawns
        WHEN 16030 THEN 61753 -- Maggot -> Maggot, species 450, 282 spawns
        WHEN 16068 THEN 62022 -- Larva -> Larva, species 461, 37 spawns
        WHEN 16998 THEN 68655 -- Mr. Bigglesworth -> Mr. Bigglesworth, species 1145, 1 spawns
        WHEN 17467 THEN 61255 -- Skunk -> Skunk, species 397, 187 spawns
        WHEN 20725 THEN 61829 -- Bat -> Bat, species 626, 155 spawns
        WHEN 22306 THEN 62638 -- Skittering Cavern Crawler -> Skittering Cavern Crawler, species 637, 30 spawns
        WHEN 22480 THEN 61752 -- Brown Marmot -> Brown Marmot, species 449, 62 spawns
        WHEN 23801 THEN 62648 -- Turkey -> Turkey, species 525, 16 spawns
        WHEN 24174 THEN 62641 -- Fjord Rat -> Fjord Rat, species 644, 53 spawns
        WHEN 24270 THEN 62640 -- Devouring Maggot -> Devouring Maggot, species 523, 16 spawns
        WHEN 26525 THEN 61384 -- Cockroach -> Cockroach, species 393, 137 spawns
        WHEN 28440 THEN 62835 -- Tundra Penguin -> Tundra Penguin, species 536, 33 spawns
        WHEN 29328 THEN 62693 -- Arctic Hare -> Arctic Hare, species 641, 171 spawns
        WHEN 31685 THEN 62695 -- Borean Marmot -> Borean Marmot, species 639, 93 spawns
        WHEN 31889 THEN 62818 -- Grizzly Squirrel -> Grizzly Squirrel, species 647, 66 spawns
        WHEN 31890 THEN 61677 -- Mountain Skunk -> Mountain Skunk, species 633, 108 spawns
        WHEN 32258 THEN 61438 -- Gold Beetle -> Gold Beetle, species 430, 159 spawns
        WHEN 32261 THEN 62435 -- Crystal Spider -> Crystal Spider, species 634, 86 spawns
        WHEN 36591 THEN 62312 -- Frog -> Frog, species 495, 86 spawns
        WHEN 36714 THEN 61170 -- Sheep -> Sheep, species 394, 21 spawns
        WHEN 40539 THEN 61319 -- Beetle -> Beetle, species 406, 8 spawns
        WHEN 44710 THEN 62257 -- Sand Kitten -> Sand Kitten, species 491, 14 spawns
        WHEN 44880 THEN 62953 -- Sea Gull -> Sea Gull, species 560, 107 spawns
        WHEN 45439 THEN 61384 -- Cockroach -> Cockroach, species 393, 237 spawns
        WHEN 47194 THEN 61170 -- Sheep -> Sheep, species 394, 201 spawns
        WHEN 47667 THEN 61323 -- Wharf Rat -> Wharf Rat, species 410, 14 spawns
        WHEN 48686 THEN 61439 -- Rattlesnake -> Rattlesnake, species 431, 86 spawns
        WHEN 48692 THEN 62117 -- Twilight Spider -> Twilight Spider, species 470, 58 spawns
        WHEN 48935 THEN 61690 -- Alpine Hare -> Alpine Hare, species 441, 536 spawns
        WHEN 48956 THEN 61691 -- Irradiated Roach -> Irradiated Roach, species 442, 62 spawns
        WHEN 48972 THEN 61317 -- Long-tailed Mole -> Long-tailed Mole, species 404, 45 spawns
        WHEN 49540 THEN 62954 -- Stormwind Rat -> Stormwind Rat, species 675, 118 spawns
        WHEN 49568 THEN 61385 -- Ash Viper -> Ash Viper, species 425, 42 spawns
        WHEN 49673 THEN 61258 -- Rat Snake -> Rat Snake, species 399, 57 spawns
        WHEN 49690 THEN 61253 -- Dusk Spiderling -> Dusk Spiderling, species 396, 99 spawns
        WHEN 49722 THEN 61318 -- Tree Python -> Tree Python, species 405, 113 spawns
        WHEN 49724 THEN 62190 -- Coral Snake -> Coral Snake, species 488, 38 spawns
        WHEN 49725 THEN 62127 -- Emerald Boa -> Emerald Boa, species 631, 105 spawns
        WHEN 49727 THEN 62523 -- Sidewinder -> Sidewinder, species 511, 181 spawns
        WHEN 49728 THEN 62178 -- Elfin Rabbit -> Elfin Rabbit, species 479, 78 spawns
        WHEN 49732 THEN 62894 -- Horned Lizard -> Horned Lizard, species 851, 188 spawns
        WHEN 49743 THEN 62115 -- Dung Beetle -> Dung Beetle, species 467, 369 spawns
        WHEN 49758 THEN 62921 -- Stowaway Rat -> Stowaway Rat, species 553, 8 spawns
        WHEN 49770 THEN 62924 -- Deepholm Cockroach -> Deepholm Cockroach, species 555, 178 spawns
        WHEN 49771 THEN 62925 -- Crystal Beetle -> Crystal Beetle, species 556, 175 spawns
        WHEN 49773 THEN 62119 -- Robo-Chick -> Robo-Chick, species 471, 36 spawns
        WHEN 49774 THEN 62120 -- Rabid Nut Varmint 5000 -> Rabid Nut Varmint 5000, species 472, 40 spawns
        WHEN 49778 THEN 61757 -- Red-Tailed Chipmunk -> Red-Tailed Chipmunk, species 452, 157 spawns
        WHEN 49779 THEN 62189 -- Alpine Chipmunk -> Alpine Chipmunk, species 487, 312 spawns
        WHEN 49835 THEN 62186 -- Desert Spider -> Desert Spider, species 484, 282 spawns
        WHEN 49836 THEN 62256 -- Stinkbug -> Stinkbug, species 492, 76 spawns
        WHEN 49837 THEN 62114 -- Spiny Lizard -> Spiny Lizard, species 466, 124 spawns
        WHEN 49840 THEN 61441 -- Spiky Lizard -> Spiky Lizard, species 433, 44 spawns
        WHEN 49842 THEN 62177 -- Forest Moth -> Forest Moth, species 478, 235 spawns
        WHEN 49844 THEN 62373 -- Silky Moth -> Silky Moth, species 503, 28 spawns
        WHEN 49847 THEN 62916 -- Fungal Moth -> Fungal Moth, species 756, 91 spawns
        WHEN 49857 THEN 62915 -- Emerald Shale Hatchling -> Emerald Shale Hatchling, species 837, 155 spawns
        WHEN 49858 THEN 62182 -- Amethyst Shale Hatchling -> Amethyst Shale Hatchling, species 838, 148 spawns
        WHEN 49859 THEN 62181 -- Topaz Shale Hatchling -> Topaz Shale Hatchling, species 480, 96 spawns
        WHEN 49861 THEN 62118 -- Twilight Beetle -> Twilight Beetle, species 469, 26 spawns
        WHEN 49928 THEN 61314 -- Crimson Moth -> Crimson Moth, species 421, 100 spawns
        WHEN 49929 THEN 62922 -- Crimson Shale Hatchling -> Crimson Shale Hatchling, species 554, 113 spawns
        WHEN 49965 THEN 61889 -- Undercity Rat -> Undercity Rat, species 454, 30 spawns
        WHEN 49995 THEN 61168 -- Redridge Rat -> Redridge Rat, species 392, 16 spawns
        WHEN 49996 THEN 61167 -- Mountain Cottontail -> Mountain Cottontail, species 391, 101 spawns
        WHEN 50000 THEN 61370 -- Swamp Moth -> Swamp Moth, species 402, 13 spawns
        WHEN 50479 THEN 61321 -- Lizard Hatchling -> Lizard Hatchling, species 408, 94 spawns
        WHEN 50487 THEN 61320 -- Forest Spiderling -> Forest Spiderling, species 407, 93 spawns
        WHEN 50490 THEN 62893 -- Locust -> Locust, species 543, 98 spawns
        WHEN 50491 THEN 62892 -- Mac Frog -> Mac Frog, species 542, 143 spawns
        WHEN 50496 THEN 62895 -- Oasis Moth -> Oasis Moth, species 544, 172 spawns
        WHEN 51126 THEN 62900 -- Wildhammer Gryphon Hatchling -> Wildhammer Gryphon Hatchling, species 548, 9 spawns
        WHEN 55336 THEN 63001 -- Silkbead Snail -> Silkbead Snail, species 568, 42 spawns
        WHEN 55741 THEN 67443 -- Crow -> Crow, species 1068, 38 spawns
        WHEN 55749 THEN 62953 -- Sea Gull -> Sea Gull, species 560, 11 spawns
        WHEN 56829 THEN 65215 -- Garden Moth -> Garden Moth, species 753, 17 spawns
        WHEN 58230 THEN 63006 -- Sandy Petrel -> Sandy Petrel, species 573, 19 spawns
        WHEN 58236 THEN 63006 -- Sandy Petrel -> Sandy Petrel, species 573, 1 spawns
        WHEN 58675 THEN 63005 -- Spirebound Crab -> Spirebound Crab, species 572, 48 spawns
        WHEN 58696 THEN 63002 -- Garden Frog -> Garden Frog, species 569, 32 spawns
        WHEN 58698 THEN 62999 -- Temple Snake -> Temple Snake, species 567, 78 spawns
        WHEN 58882 THEN 65054 -- Feverbite Hatchling -> Feverbite Hatchling, species 714, 140 spawns
        WHEN 59085 THEN 62998 -- Mirror Strider -> Mirror Strider, species 566, 65 spawns
        WHEN 59312 THEN 63004 -- Grove Viper -> Grove Viper, species 571, 157 spawns
        WHEN 59356 THEN 62991 -- Coral Adder -> Coral Adder, species 562, 16 spawns
        WHEN 59357 THEN 62992 -- Bucktooth Flapper -> Bucktooth Flapper, species 380, 51 spawns
        WHEN 59666 THEN 62994 -- Emerald Turtle -> Emerald Turtle, species 564, 131 spawns
        WHEN 59668 THEN 62997 -- Jungle Darter -> Jungle Darter, species 565, 25 spawns
        WHEN 59770 THEN 65216 -- Shrine Fly -> Shrine Fly, species 754, 4 spawns
        WHEN 60761 THEN 61158 -- Shore Crab -> Shore Crab, species 388, 28 spawns
        WHEN 61611 THEN 63559 -- Tiny Goldfish -> Tiny Goldfish, species 652, 19 spawns
        WHEN 63289 THEN 65124 -- Luyu Moth -> Luyu Moth, species 718, 68 spawns
        WHEN 63329 THEN 65203 -- Emperor Crab -> Emperor Crab, species 746, 16 spawns
        WHEN 63361 THEN 65185 -- Mei Li Sparkler -> Mei Li Sparkler, species 722, 28 spawns
        WHEN 63980 THEN 65187 -- Amber Moth -> Amber Moth, species 732, 81 spawns
        WHEN 64761 THEN 63715 -- Jumping Spider -> Jumping Spider, species 699, 87 spawns
        WHEN 64774 THEN 63919 -- Leopard Tree Frog -> Leopard Tree Frog, species 702, 95 spawns
        WHEN 64775 THEN 63003 -- Masked Tanuki -> Masked Tanuki, species 570, 113 spawns
        WHEN 64776 THEN 63716 -- Masked Tanuki Pup -> Masked Tanuki Pup, species 703, 24 spawns
        WHEN 64782 THEN 63062 -- Bandicoon -> Bandicoon, species 706, 277 spawns
        WHEN 64783 THEN 63064 -- Bandicoon Kit -> Bandicoon Kit, species 707, 58 spawns
        WHEN 64784 THEN 63094 -- Malayan Quillrat -> Malayan Quillrat, species 708, 201 spawns
        WHEN 64785 THEN 63095 -- Malayan Quillrat Pup -> Malayan Quillrat Pup, species 709, 60 spawns
        WHEN 64786 THEN 63096 -- Marsh Fiddler -> Marsh Fiddler, species 710, 212 spawns
        WHEN 64787 THEN 63057 -- Sifang Otter -> Sifang Otter, species 711, 42 spawns
        WHEN 64789 THEN 63060 -- Softshell Snapling -> Softshell Snapling, species 713, 198 spawns
        WHEN 64790 THEN 63550 -- Alpine Foxling -> Alpine Foxling, species 724, 20 spawns
        WHEN 64791 THEN 63551 -- Alpine Foxling Kit -> Alpine Foxling Kit, species 725, 10 spawns
        WHEN 64792 THEN 63547 -- Plains Monitor -> Plains Monitor, species 726, 82 spawns
        WHEN 64793 THEN 59702 -- Prairie Mouse -> Prairie Mouse, species 727, 82 spawns
        WHEN 64794 THEN 63585 -- Szechuan Chicken -> Szechuan Chicken, species 728, 5 spawns
        WHEN 64795 THEN 63557 -- Tolai Hare -> Tolai Hare, species 729, 31 spawns
        WHEN 64796 THEN 63558 -- Tolai Hare Pup -> Tolai Hare Pup, species 730, 16 spawns
        WHEN 64797 THEN 63555 -- Zooey Snake -> Zooey Snake, species 731, 48 spawns
        WHEN 64798 THEN 63288 -- Amethyst Spiderling -> Amethyst Spiderling, species 716, 76 spawns
        WHEN 64799 THEN 63291 -- Savory Beetle -> Savory Beetle, species 717, 65 spawns
        WHEN 64800 THEN 63293 -- Spiny Terrapin -> Spiny Terrapin, species 723, 100 spawns
        WHEN 64801 THEN 63549 -- Grassland Hopper -> Grassland Hopper, species 733, 143 spawns
        WHEN 64802 THEN 63957 -- Yakrat -> Yakrat, species 740, 51 spawns
        WHEN 64803 THEN 64242 -- Clouded Hedgehog -> Clouded Hedgehog, species 742, 64 spawns
        WHEN 64805 THEN 63548 -- Crunchy Scorpion -> Crunchy Scorpion, species 745, 29 spawns
        WHEN 64806 THEN 64352 -- Rapana Whelk -> Rapana Whelk, species 743, 51 spawns
        WHEN 64807 THEN 64238 -- Resilient Roach -> Resilient Roach, species 744, 77 spawns
        WHEN 65191 THEN 65190 -- Mongoose -> Mongoose, species 737, 22 spawns
        WHEN 65192 THEN 63954 -- Mongoose Pup -> Mongoose Pup, species 739, 30 spawns
        WHEN 65204 THEN 63850 -- Effervescent Glowfly -> Effervescent Glowfly, species 747, 39 spawns
        WHEN 65205 THEN 63838 -- Gilded Moth -> Gilded Moth, species 748, 72 spawns
        WHEN 65206 THEN 63841 -- Golden Civet -> Golden Civet, species 749, 56 spawns
        WHEN 65207 THEN 63842 -- Golden Civet Kitten -> Golden Civet Kitten, species 750, 10 spawns
        WHEN 65209 THEN 63847 -- Dancing Water Skimmer -> Dancing Water Skimmer, species 751, 40 spawns
        WHEN 65211 THEN 63849 -- Yellow-Bellied Bullfrog -> Yellow-Bellied Bullfrog, species 752, 11 spawns
        WHEN 67679 THEN 61088 -- Eternal Strider -> Eternal Strider, species 383, 2 spawns
        WHEN 73573 THEN 73542 -- Ashwing Moth -> Ashwing Moth, species 1324, 57 spawns
        WHEN 73828 THEN 73543 -- Flamering Moth -> Flamering Moth, species 1325, 16 spawns
        WHEN 73829 THEN 73368 -- Skywisp Moth -> Skywisp Moth, species 1326, 2 spawns
        ELSE `id`
    END,
    `npcflag` = 0,
    `unit_flags` = 0,
    `dynamicflags` = 0
WHERE `id` IN (620, 721, 890, 1412, 1420, 1933, 2110, 2620, 2914, 3300, 3835, 4075, 4076, 4953, 5951, 6271, 6368, 6653, 6827, 9600, 9699, 9700, 10779, 13321, 14881, 15475, 16030, 16068, 16998, 17467, 20725, 22306, 22480, 23801, 24174, 24270, 26525, 28440, 29328, 31685, 31889, 31890, 32258, 32261, 36591, 36714, 40539, 44710, 44880, 45439, 47194, 47667, 48686, 48692, 48935, 48956, 48972, 49540, 49568, 49673, 49690, 49722, 49724, 49725, 49727, 49728, 49732, 49743, 49758, 49770, 49771, 49773, 49774, 49778, 49779, 49835, 49836, 49837, 49840, 49842, 49844, 49847, 49857, 49858, 49859, 49861, 49928, 49929, 49965, 49995, 49996, 50000, 50479, 50487, 50490, 50491, 50496, 51126, 55336, 55741, 55749, 56829, 58230, 58236, 58675, 58696, 58698, 58882, 59085, 59312, 59356, 59357, 59666, 59668, 59770, 60761, 61611, 63289, 63329, 63361, 63980, 64761, 64774, 64775, 64776, 64782, 64783, 64784, 64785, 64786, 64787, 64789, 64790, 64791, 64792, 64793, 64794, 64795, 64796, 64797, 64798, 64799, 64800, 64801, 64802, 64803, 64805, 64806, 64807, 65191, 65192, 65204, 65205, 65206, 65207, 65209, 65211, 67679, 73573, 73828, 73829);
