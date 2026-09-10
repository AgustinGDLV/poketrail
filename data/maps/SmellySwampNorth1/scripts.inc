SmellySwampNorth1_MapScripts::
	.byte 0

SmellySwampNorth1_LudicoloBoss::
	lock
	msgbox SmellySwampNorth1_LudicoloText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SmellySwampNorth1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	applymovement 3, Common_Movement_ExclamationMark
	applymovement 4, Common_Movement_ExclamationMark
	applymovement 5, Common_Movement_ExclamationMark
	applymovement 6, Common_Movement_ExclamationMark
	applymovement 7, Common_Movement_ExclamationMark
	waitmovement SPECIES_LUDICOLO
    playmoncry SPECIES_LUDICOLO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_LUDICOLO
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 1
	removeobject 2
	removeobject 3
	removeobject 4
	removeobject 5
	removeobject 6
	removeobject 7
	releaseall
    end

SmellySwampNorth1_End:
	closemessage
	releaseall
	end

SmellySwampNorth1_Swimmer::
	lockall
	faceplayer
	msgbox SmellySwampNorth1_Swimmer_Text
	releaseall
	end

SmellySwampNorth1_LudicoloText:
	.string "These Lotad seem caught\n"
	.string "in an endless dance…\p"
	.string "Fight?$"

SmellySwampNorth1_Swimmer_Text:
	.string "I don't think it'd be\n"
	.string "a good idea to dive here…$"
