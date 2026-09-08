DeepDesertWest1_MapScripts::
	.byte 0

DeepDesertWest1_HippowdonBoss::
	lock
	msgbox DeepDesertWest1_HippowdonText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DeepDesertWest1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	waitmovement 0
    playmoncry SPECIES_HIPPOWDON, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_HIPPOWDON
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 1
	removeobject 2
	releaseall
    end

DeepDesertWest1_End:
	closemessage
	releaseall
	end

DeepDesertWest1_Breeder::
	lockall
	faceplayer
	msgbox DeepDesertWest1_Breeder_Text
	releaseall
	end

DeepDesertWest1_Youngster::
	lockall
	faceplayer
	msgbox DeepDesertWest1_Youngster_Text
	releaseall
	end

DeepDesertWest1_Hiker::
	lockall
	faceplayer
	msgbox DeepDesertWest1_Hiker_Text
	releaseall
	end

DeepDesertWest1_Backpacker::
	lockall
	faceplayer
	msgbox DeepDesertWest1_Backpacker_Text
	releaseall
	end

DeepDesertWest1_HippowdonText:
	.string "A Hippowdon and his son?\p"
	.string "…\p"
	.string "They're blocking the way.\n"
	.string "Fight?$"

DeepDesertWest1_Breeder_Text:
	.string "Have you noticed there are\n"
	.string "some extra strong POKéMON?\p"
	.string "This journey is hard!$"

DeepDesertWest1_Youngster_Text:
	.string "Gosh!\p"
	.string "This is rock really\n"
	.string "REVIVEs my spirits!$"

DeepDesertWest1_Hiker_Text:
	.string "My wife tells me\n"
	.string "we're halfway there!\p"
	.string "I hope so…$"

DeepDesertWest1_Backpacker_Text:
	.string "I'm traveling to Orchid\n"
	.string "City with my husband!$"
