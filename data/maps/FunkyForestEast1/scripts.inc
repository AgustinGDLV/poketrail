FunkyForestEast1_MapScripts::
	.byte 0

FunkyForestEast1_ScytherBoss::
	lock
	msgbox FunkyForestEast1_ScytherText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FunkyForestEast1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	waitmovement 0
    playmoncry SPECIES_SCYTHER, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_SCYTHER
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 1
	removeobject 2
	releaseall
    end

FunkyForestEast1_End:
	closemessage
	releaseall
	end

FunkyForestEast1_Youngster::
	lockall
	faceplayer
	msgbox FunkyForestEast1_Youngster_Text
	applymovement 2, FunkyForestEast1_Movement_FaceUp
	waitmovement 2
	releaseall
	end

FunkyForestEast1_Movement_FaceUp:
	face_up
	step_end

FunkyForestEast1_Swimmer::
	lockall
	faceplayer
	msgbox FunkyForestEast1_Swimmer_Text
	releaseall
	end

FunkyForestEast1_ScytherText:
	.string "A big bug blocks the way.\n"
	.string "Fight?$"

FunkyForestEast1_Swimmer_Text:
	.string "The water looks so inviting,\n"
	.string "I want to dive in!$"

FunkyForestEast1_Youngster_Text:
	.string "I had a question to ask you…\p"
	.string "…but I think I'll\n"
	.string "just look it up!$"
