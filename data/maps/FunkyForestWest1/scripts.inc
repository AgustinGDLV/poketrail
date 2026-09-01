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
	removeobject 5
	releaseall
    end

FunkyForestWest1_End::
	end

FunkyForestWest1_ShuckleeText:
	.string "A large Shuckle blocks the\n"
	.string "way. Try to pass?$"
