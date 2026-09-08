FunkyForestWest1_MapScripts::
	.byte 0

FunkyForestWest1_ShuckleBoss::
	lock
	msgbox FunkyForestWest1_ShuckleeText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FunkyForestWest1_End
	playse SE_PIN
	applymovement 5, Common_Movement_ExclamationMark
	waitmovement 0
    playmoncry SPECIES_SHUCKLE, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_SHUCKLE
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 4
	releaseall
    end

FunkyForestWest1_End::
	closemessage
	releaseall
	end

FunkyForestWest1_ShuckleeText:
	.string "A large Shuckle blocks the\n"
	.string "way. Try to pass?$"

FunkyForestWest1_Youngster::
	lockall
	faceplayer
	msgbox FunkyForestWest1_Youngster_Text
	releaseall
	end

FunkyForestWest1_Youngster_Text::
	.string "They taught me in school\n"
	.string "about POKéMON types…\l"
	.string "but they don't seem to work\p"
	.string "School stinks!$"

FunkyForestWest1_Hiker::
	lockall
	faceplayer
	msgbox FunkyForestWest1_Hiker_Text
	releaseall
	end

FunkyForestWest1_Hiker_Text::
	.string "There's a desert ahead, but what\n"
	.string "I'm really looking forward to is…\p"
	.string "Sweaty Summit!$"
