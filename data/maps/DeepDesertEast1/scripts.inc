DeepDesertEast1_MapScripts::
	.byte 0

DeepDesertEast1_CacturneBoss::
	lock
	msgbox DeepDesertEast1_CacturneText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DeepDesertEast1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	applymovement 3, Common_Movement_ExclamationMark
	waitmovement SPECIES_CACTURNE
    playmoncry SPECIES_CACTURNE, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_CACTURNE
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 1
	removeobject 2
	removeobject 3
	releaseall
    end

DeepDesertEast1_End:
	closemessage
	releaseall
	end

DeepDesertEast1_BugCatcher::
	lockall
	faceplayer
	msgbox DeepDesertEast1_BugCatcher_Text
	releaseall
	end

DeepDesertEast1_Hiker::
	lockall
	faceplayer
	msgbox DeepDesertEast1_Hiker_Text
	releaseall
	end

DeepDesertEast1_CacturneText:
	.string "This gang of Cacturne won't\n"
	.string "let you pass…\p"
	.string "Fight?$"

DeepDesertEast1_BugCatcher_Text:
	.string "I can't find!\n"
	.string "any bugs here…\p"
	.string "This stinks!$"

DeepDesertEast1_Hiker_Text:
	.string "Headed to Orchid City?\p"
	.string "You should stop\n"
	.string "at Tulip Town!\p"
	.string "Some of the townsfolk are\n"
	.string "helping tired travelers.$"
