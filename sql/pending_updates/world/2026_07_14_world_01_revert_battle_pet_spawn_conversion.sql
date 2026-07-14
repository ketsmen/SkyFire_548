-- Revert legacy ambient critter creature spawns back to their pre-battle-pet entries.
-- This counters the promoted spawn conversion now handled dynamically by battle_pet_wild_pool.
-- Note: a few promoted battle-pet entries had multiple old source entries; those are restored to the lowest original entry ID.

UPDATE `creature`
SET
    `id` = CASE `id`
        WHEN 59702 THEN 64793 -- Prairie Mouse -> Prairie Mouse, species 727, 82 spawns
        WHEN 61071 THEN 13321 -- Small Frog -> Small Frog, species 419, 877 spawns
        WHEN 61080 THEN 721 -- Rabbit -> Rabbit, species 378, 769 spawns
        WHEN 61081 THEN 1412 -- Squirrel -> Squirrel, species 379, 1032 spawns
        WHEN 61088 THEN 67679 -- Eternal Strider -> Eternal Strider, species 383, 2 spawns
        WHEN 61141 THEN 2620 -- Prairie Dog -> Prairie Dog, species 386, 276 spawns
        WHEN 61142 THEN 2914 -- Snake -> Snake, species 387, 591 spawns
        WHEN 61143 THEN 6271 -- Mouse -> Mouse, species 385, 197 spawns
        WHEN 61158 THEN 6827 -- Shore Crab -> Shore Crab, species 388, 1007 spawns; ambiguous old entries: 6827, 60761
        WHEN 61165 THEN 890 -- Fawn -> Fawn, species 447, 74 spawns
        WHEN 61167 THEN 49996 -- Mountain Cottontail -> Mountain Cottontail, species 391, 101 spawns
        WHEN 61168 THEN 49995 -- Redridge Rat -> Redridge Rat, species 392, 16 spawns
        WHEN 61169 THEN 4076 -- Roach -> Roach, species 424, 518 spawns
        WHEN 61170 THEN 1933 -- Sheep -> Sheep, species 394, 185 spawns; ambiguous old entries: 1933, 36714, 47194
        WHEN 61253 THEN 49690 -- Dusk Spiderling -> Dusk Spiderling, species 396, 99 spawns
        WHEN 61255 THEN 17467 -- Skunk -> Skunk, species 397, 187 spawns
        WHEN 61257 THEN 2110 -- Black Rat -> Black Rat, species 398, 473 spawns
        WHEN 61258 THEN 49673 -- Rat Snake -> Rat Snake, species 399, 57 spawns
        WHEN 61313 THEN 9600 -- Parrot -> Parrot, species 403, 86 spawns
        WHEN 61314 THEN 49928 -- Crimson Moth -> Crimson Moth, species 421, 100 spawns
        WHEN 61317 THEN 48972 -- Long-tailed Mole -> Long-tailed Mole, species 404, 45 spawns
        WHEN 61318 THEN 49722 -- Tree Python -> Tree Python, species 405, 113 spawns
        WHEN 61319 THEN 15475 -- Beetle -> Beetle, species 406, 343 spawns; ambiguous old entries: 15475, 40539
        WHEN 61320 THEN 50487 -- Forest Spiderling -> Forest Spiderling, species 407, 93 spawns
        WHEN 61321 THEN 50479 -- Lizard Hatchling -> Lizard Hatchling, species 408, 94 spawns
        WHEN 61323 THEN 47667 -- Wharf Rat -> Wharf Rat, species 410, 14 spawns
        WHEN 61325 THEN 3300 -- Adder -> Adder, species 635, 293 spawns
        WHEN 61327 THEN 14881 -- Spider -> Spider, species 412, 683 spawns
        WHEN 61328 THEN 9699 -- Fire Beetle -> Fire Beetle, species 415, 105 spawns
        WHEN 61366 THEN 4075 -- Rat -> Rat, species 417, 1284 spawns
        WHEN 61368 THEN 6653 -- Huge Toad -> Huge Toad, species 648, 59 spawns
        WHEN 61369 THEN 1420 -- Toad -> Toad, species 420, 576 spawns
        WHEN 61370 THEN 50000 -- Swamp Moth -> Swamp Moth, species 402, 13 spawns
        WHEN 61372 THEN 4953 -- Moccasin -> Moccasin, species 422, 204 spawns
        WHEN 61383 THEN 9700 -- Lava Crab -> Lava Crab, species 423, 31 spawns
        WHEN 61384 THEN 26525 -- Cockroach -> Cockroach, species 393, 137 spawns; ambiguous old entries: 26525, 45439
        WHEN 61385 THEN 49568 -- Ash Viper -> Ash Viper, species 425, 42 spawns
        WHEN 61438 THEN 32258 -- Gold Beetle -> Gold Beetle, species 430, 159 spawns
        WHEN 61439 THEN 48686 -- Rattlesnake -> Rattlesnake, species 431, 86 spawns
        WHEN 61441 THEN 49840 -- Spiky Lizard -> Spiky Lizard, species 433, 44 spawns
        WHEN 61677 THEN 31890 -- Mountain Skunk -> Mountain Skunk, species 633, 108 spawns
        WHEN 61690 THEN 48935 -- Alpine Hare -> Alpine Hare, species 441, 536 spawns
        WHEN 61691 THEN 48956 -- Irradiated Roach -> Irradiated Roach, species 442, 62 spawns
        WHEN 61751 THEN 5951 -- Hare -> Hare, species 448, 147 spawns
        WHEN 61752 THEN 22480 -- Brown Marmot -> Brown Marmot, species 449, 62 spawns
        WHEN 61753 THEN 16030 -- Maggot -> Maggot, species 450, 282 spawns
        WHEN 61757 THEN 49778 -- Red-Tailed Chipmunk -> Red-Tailed Chipmunk, species 452, 157 spawns
        WHEN 61828 THEN 10779 -- Infected Squirrel -> Infected Squirrel, species 627, 57 spawns
        WHEN 61829 THEN 20725 -- Bat -> Bat, species 626, 155 spawns
        WHEN 61889 THEN 49965 -- Undercity Rat -> Undercity Rat, species 454, 30 spawns
        WHEN 62019 THEN 6368 -- Cat -> Cat, species 459, 41 spawns
        WHEN 62022 THEN 16068 -- Larva -> Larva, species 461, 37 spawns
        WHEN 62114 THEN 49837 -- Spiny Lizard -> Spiny Lizard, species 466, 124 spawns
        WHEN 62115 THEN 49743 -- Dung Beetle -> Dung Beetle, species 467, 369 spawns
        WHEN 62117 THEN 48692 -- Twilight Spider -> Twilight Spider, species 470, 58 spawns
        WHEN 62118 THEN 49861 -- Twilight Beetle -> Twilight Beetle, species 469, 26 spawns
        WHEN 62119 THEN 49773 -- Robo-Chick -> Robo-Chick, species 471, 36 spawns
        WHEN 62120 THEN 49774 -- Rabid Nut Varmint 5000 -> Rabid Nut Varmint 5000, species 472, 40 spawns
        WHEN 62127 THEN 49725 -- Emerald Boa -> Emerald Boa, species 631, 105 spawns
        WHEN 62177 THEN 49842 -- Forest Moth -> Forest Moth, species 478, 235 spawns
        WHEN 62178 THEN 49728 -- Elfin Rabbit -> Elfin Rabbit, species 479, 78 spawns
        WHEN 62181 THEN 49859 -- Topaz Shale Hatchling -> Topaz Shale Hatchling, species 480, 96 spawns
        WHEN 62182 THEN 49858 -- Amethyst Shale Hatchling -> Amethyst Shale Hatchling, species 838, 148 spawns
        WHEN 62186 THEN 49835 -- Desert Spider -> Desert Spider, species 484, 282 spawns
        WHEN 62189 THEN 49779 -- Alpine Chipmunk -> Alpine Chipmunk, species 487, 312 spawns
        WHEN 62190 THEN 49724 -- Coral Snake -> Coral Snake, species 488, 38 spawns
        WHEN 62256 THEN 49836 -- Stinkbug -> Stinkbug, species 492, 76 spawns
        WHEN 62257 THEN 44710 -- Sand Kitten -> Sand Kitten, species 491, 14 spawns
        WHEN 62312 THEN 36591 -- Frog -> Frog, species 495, 86 spawns
        WHEN 62373 THEN 49844 -- Silky Moth -> Silky Moth, species 503, 28 spawns
        WHEN 62435 THEN 32261 -- Crystal Spider -> Crystal Spider, species 634, 86 spawns
        WHEN 62523 THEN 49727 -- Sidewinder -> Sidewinder, species 511, 181 spawns
        WHEN 62638 THEN 22306 -- Skittering Cavern Crawler -> Skittering Cavern Crawler, species 637, 30 spawns
        WHEN 62640 THEN 24270 -- Devouring Maggot -> Devouring Maggot, species 523, 16 spawns
        WHEN 62641 THEN 24174 -- Fjord Rat -> Fjord Rat, species 644, 53 spawns
        WHEN 62648 THEN 23801 -- Turkey -> Turkey, species 525, 16 spawns
        WHEN 62664 THEN 620 -- Chicken -> Chicken, species 646, 139 spawns
        WHEN 62693 THEN 29328 -- Arctic Hare -> Arctic Hare, species 641, 171 spawns
        WHEN 62695 THEN 31685 -- Borean Marmot -> Borean Marmot, species 639, 93 spawns
        WHEN 62815 THEN 3835 -- Biletoad -> Biletoad, species 649, 56 spawns
        WHEN 62818 THEN 31889 -- Grizzly Squirrel -> Grizzly Squirrel, species 647, 66 spawns
        WHEN 62835 THEN 28440 -- Tundra Penguin -> Tundra Penguin, species 536, 33 spawns
        WHEN 62892 THEN 50491 -- Mac Frog -> Mac Frog, species 542, 143 spawns
        WHEN 62893 THEN 50490 -- Locust -> Locust, species 543, 98 spawns
        WHEN 62894 THEN 49732 -- Horned Lizard -> Horned Lizard, species 851, 188 spawns
        WHEN 62895 THEN 50496 -- Oasis Moth -> Oasis Moth, species 544, 172 spawns
        WHEN 62900 THEN 51126 -- Wildhammer Gryphon Hatchling -> Wildhammer Gryphon Hatchling, species 548, 9 spawns
        WHEN 62915 THEN 49857 -- Emerald Shale Hatchling -> Emerald Shale Hatchling, species 837, 155 spawns
        WHEN 62916 THEN 49847 -- Fungal Moth -> Fungal Moth, species 756, 91 spawns
        WHEN 62921 THEN 49758 -- Stowaway Rat -> Stowaway Rat, species 553, 8 spawns
        WHEN 62922 THEN 49929 -- Crimson Shale Hatchling -> Crimson Shale Hatchling, species 554, 113 spawns
        WHEN 62924 THEN 49770 -- Deepholm Cockroach -> Deepholm Cockroach, species 555, 178 spawns
        WHEN 62925 THEN 49771 -- Crystal Beetle -> Crystal Beetle, species 556, 175 spawns
        WHEN 62953 THEN 44880 -- Sea Gull -> Sea Gull, species 560, 107 spawns; ambiguous old entries: 44880, 55749
        WHEN 62954 THEN 49540 -- Stormwind Rat -> Stormwind Rat, species 675, 118 spawns
        WHEN 62991 THEN 59356 -- Coral Adder -> Coral Adder, species 562, 16 spawns
        WHEN 62992 THEN 59357 -- Bucktooth Flapper -> Bucktooth Flapper, species 380, 51 spawns
        WHEN 62994 THEN 59666 -- Emerald Turtle -> Emerald Turtle, species 564, 131 spawns
        WHEN 62997 THEN 59668 -- Jungle Darter -> Jungle Darter, species 565, 25 spawns
        WHEN 62998 THEN 59085 -- Mirror Strider -> Mirror Strider, species 566, 65 spawns
        WHEN 62999 THEN 58698 -- Temple Snake -> Temple Snake, species 567, 78 spawns
        WHEN 63001 THEN 55336 -- Silkbead Snail -> Silkbead Snail, species 568, 42 spawns
        WHEN 63002 THEN 58696 -- Garden Frog -> Garden Frog, species 569, 32 spawns
        WHEN 63003 THEN 64775 -- Masked Tanuki -> Masked Tanuki, species 570, 113 spawns
        WHEN 63004 THEN 59312 -- Grove Viper -> Grove Viper, species 571, 157 spawns
        WHEN 63005 THEN 58675 -- Spirebound Crab -> Spirebound Crab, species 572, 48 spawns
        WHEN 63006 THEN 58230 -- Sandy Petrel -> Sandy Petrel, species 573, 19 spawns; ambiguous old entries: 58230, 58236
        WHEN 63057 THEN 64787 -- Sifang Otter -> Sifang Otter, species 711, 42 spawns
        WHEN 63060 THEN 64789 -- Softshell Snapling -> Softshell Snapling, species 713, 198 spawns
        WHEN 63062 THEN 64782 -- Bandicoon -> Bandicoon, species 706, 277 spawns
        WHEN 63064 THEN 64783 -- Bandicoon Kit -> Bandicoon Kit, species 707, 58 spawns
        WHEN 63094 THEN 64784 -- Malayan Quillrat -> Malayan Quillrat, species 708, 201 spawns
        WHEN 63095 THEN 64785 -- Malayan Quillrat Pup -> Malayan Quillrat Pup, species 709, 60 spawns
        WHEN 63096 THEN 64786 -- Marsh Fiddler -> Marsh Fiddler, species 710, 212 spawns
        WHEN 63288 THEN 64798 -- Amethyst Spiderling -> Amethyst Spiderling, species 716, 76 spawns
        WHEN 63291 THEN 64799 -- Savory Beetle -> Savory Beetle, species 717, 65 spawns
        WHEN 63293 THEN 64800 -- Spiny Terrapin -> Spiny Terrapin, species 723, 100 spawns
        WHEN 63547 THEN 64792 -- Plains Monitor -> Plains Monitor, species 726, 82 spawns
        WHEN 63548 THEN 64805 -- Crunchy Scorpion -> Crunchy Scorpion, species 745, 29 spawns
        WHEN 63549 THEN 64801 -- Grassland Hopper -> Grassland Hopper, species 733, 143 spawns
        WHEN 63550 THEN 64790 -- Alpine Foxling -> Alpine Foxling, species 724, 20 spawns
        WHEN 63551 THEN 64791 -- Alpine Foxling Kit -> Alpine Foxling Kit, species 725, 10 spawns
        WHEN 63555 THEN 64797 -- Zooey Snake -> Zooey Snake, species 731, 48 spawns
        WHEN 63557 THEN 64795 -- Tolai Hare -> Tolai Hare, species 729, 31 spawns
        WHEN 63558 THEN 64796 -- Tolai Hare Pup -> Tolai Hare Pup, species 730, 16 spawns
        WHEN 63559 THEN 61611 -- Tiny Goldfish -> Tiny Goldfish, species 652, 19 spawns
        WHEN 63585 THEN 64794 -- Szechuan Chicken -> Szechuan Chicken, species 728, 5 spawns
        WHEN 63715 THEN 64761 -- Jumping Spider -> Jumping Spider, species 699, 87 spawns
        WHEN 63716 THEN 64776 -- Masked Tanuki Pup -> Masked Tanuki Pup, species 703, 24 spawns
        WHEN 63838 THEN 65205 -- Gilded Moth -> Gilded Moth, species 748, 72 spawns
        WHEN 63841 THEN 65206 -- Golden Civet -> Golden Civet, species 749, 56 spawns
        WHEN 63842 THEN 65207 -- Golden Civet Kitten -> Golden Civet Kitten, species 750, 10 spawns
        WHEN 63847 THEN 65209 -- Dancing Water Skimmer -> Dancing Water Skimmer, species 751, 40 spawns
        WHEN 63849 THEN 65211 -- Yellow-Bellied Bullfrog -> Yellow-Bellied Bullfrog, species 752, 11 spawns
        WHEN 63850 THEN 65204 -- Effervescent Glowfly -> Effervescent Glowfly, species 747, 39 spawns
        WHEN 63919 THEN 64774 -- Leopard Tree Frog -> Leopard Tree Frog, species 702, 95 spawns
        WHEN 63954 THEN 65192 -- Mongoose Pup -> Mongoose Pup, species 739, 30 spawns
        WHEN 63957 THEN 64802 -- Yakrat -> Yakrat, species 740, 51 spawns
        WHEN 64238 THEN 64807 -- Resilient Roach -> Resilient Roach, species 744, 77 spawns
        WHEN 64242 THEN 64803 -- Clouded Hedgehog -> Clouded Hedgehog, species 742, 64 spawns
        WHEN 64352 THEN 64806 -- Rapana Whelk -> Rapana Whelk, species 743, 51 spawns
        WHEN 65054 THEN 58882 -- Feverbite Hatchling -> Feverbite Hatchling, species 714, 140 spawns
        WHEN 65124 THEN 63289 -- Luyu Moth -> Luyu Moth, species 718, 68 spawns
        WHEN 65185 THEN 63361 -- Mei Li Sparkler -> Mei Li Sparkler, species 722, 28 spawns
        WHEN 65187 THEN 63980 -- Amber Moth -> Amber Moth, species 732, 81 spawns
        WHEN 65190 THEN 65191 -- Mongoose -> Mongoose, species 737, 22 spawns
        WHEN 65203 THEN 63329 -- Emperor Crab -> Emperor Crab, species 746, 16 spawns
        WHEN 65215 THEN 56829 -- Garden Moth -> Garden Moth, species 753, 17 spawns
        WHEN 65216 THEN 59770 -- Shrine Fly -> Shrine Fly, species 754, 4 spawns
        WHEN 67443 THEN 55741 -- Crow -> Crow, species 1068, 38 spawns
        WHEN 68655 THEN 16998 -- Mr. Bigglesworth -> Mr. Bigglesworth, species 1145, 1 spawns
        WHEN 73368 THEN 73829 -- Skywisp Moth -> Skywisp Moth, species 1326, 2 spawns
        WHEN 73542 THEN 73573 -- Ashwing Moth -> Ashwing Moth, species 1324, 57 spawns
        WHEN 73543 THEN 73828 -- Flamering Moth -> Flamering Moth, species 1325, 16 spawns
        ELSE `id`
    END,
    `npcflag` = 0,
    `unit_flags` = 0,
    `dynamicflags` = 0
WHERE `id` IN (59702, 61071, 61080, 61081, 61088, 61141, 61142, 61143, 61158, 61165, 61167, 61168, 61169, 61170, 61253, 61255, 61257, 61258, 61313, 61314, 61317, 61318, 61319, 61320, 61321, 61323, 61325, 61327, 61328, 61366, 61368, 61369, 61370, 61372, 61383, 61384, 61385, 61438, 61439, 61441, 61677, 61690, 61691, 61751, 61752, 61753, 61757, 61828, 61829, 61889, 62019, 62022, 62114, 62115, 62117, 62118, 62119, 62120, 62127, 62177, 62178, 62181, 62182, 62186, 62189, 62190, 62256, 62257, 62312, 62373, 62435, 62523, 62638, 62640, 62641, 62648, 62664, 62693, 62695, 62815, 62818, 62835, 62892, 62893, 62894, 62895, 62900, 62915, 62916, 62921, 62922, 62924, 62925, 62953, 62954, 62991, 62992, 62994, 62997, 62998, 62999, 63001, 63002, 63003, 63004, 63005, 63006, 63057, 63060, 63062, 63064, 63094, 63095, 63096, 63288, 63291, 63293, 63547, 63548, 63549, 63550, 63551, 63555, 63557, 63558, 63559, 63585, 63715, 63716, 63838, 63841, 63842, 63847, 63849, 63850, 63919, 63954, 63957, 64238, 64242, 64352, 65054, 65124, 65185, 65187, 65190, 65203, 65215, 65216, 67443, 68655, 73368, 73542, 73543);
