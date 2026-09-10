SmellySwampSouth1_MapScripts::
	.byte 0

SmellySwampSouth1_MukBoss::
	lock
	msgbox SmellySwampSouth1_MukText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SmellySwampSouth1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	applymovement 3, Common_Movement_ExclamationMark
	applymovement 4, Common_Movement_ExclamationMark
	applymovement 5, Common_Movement_ExclamationMark
	applymovement 6, Common_Movement_ExclamationMark
	applymovement 7, Common_Movement_ExclamationMark
	waitmovement SPECIES_MUK
    playmoncry SPECIES_MUK, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_MUK
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

SmellySwampSouth1_End:
	closemessage
	releaseall
	end

SmellySwampSouth1_Swimmer::
	lockall
	faceplayer
	msgbox SmellySwampSouth1_Swimmer_Text
	releaseall
	end

SmellySwampSouth1_MukText:
	.string "These Lotad seem caught\n"
	.string "in an endless dance…\p"
	.string "Fight?$"

SmellySwampSouth1_Swimmer_Text:
	.string "Hmm…\p"
	.string "This swamp feels\n"
	.string "familiar.$"
