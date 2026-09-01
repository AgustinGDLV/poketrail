FunkyForestNorth1_MapScripts::
	.byte 0

FunkyForestNorth1_RattataBoss::
	lock
	msgbox FunkyForestNorth1_RattataText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FunkyForestNorth1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	applymovement 3, Common_Movement_ExclamationMark
	waitmovement 0
    playmoncry SPECIES_RATTATA, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_RATTATA
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 1
	removeobject 2
	removeobject 3
	releaseall
    end

FunkyForestNorth1_End:
	closemessage
	releaseall
	end

FunkyForestNorth1_BugCatcher1::
	lockall
	faceplayer
	callnative GetCharacterId
	call_if_eq VAR_RESULT, 0, FunkyForestNorth1_BugCatcher1_Alt
	msgbox FunkyForestNorth1_BugCatcher1_Text
	releaseall
	end

FunkyForestNorth1_BugCatcher1_Alt::
	msgbox FunkyForestNorth1_BugCatcher1_Text_Alt
	releaseall
	end

FunkyForestNorth1_BugCatcher2::
	lockall
	faceplayer
	msgbox FunkyForestNorth1_BugCatcher2_Text
	releaseall
	end

FunkyForestNorth1_BugCatcher3::
	lockall
	faceplayer
	call_if_set FLAG_FUNKY_FOREST_NORTH_BOSS, FunkyForestNorth1_BugCatcher3_Alt
	msgbox FunkyForestNorth1_BugCatcher3_Text
	releaseall
	end

FunkyForestNorth1_BugCatcher3_Alt::
	msgbox FunkyForestNorth1_BugCatcher3_Text_Alt
	releaseall
	end

FunkyForestNorth1_RattataText:
	.string "A swarm of Rattata block the\n"
	.string "way. Fight?$"

FunkyForestNorth1_BugCatcher1_Text:
	.string "Are you on your way\n"
	.string "to Orchid City?\p"
	.string "I think only bug catchers\n"
	.string "can handle this forest!$"

FunkyForestNorth1_BugCatcher1_Text_Alt:
	.string "You're a bug catcher?\n"
	.string "Isn't this forest cool!$"

FunkyForestNorth1_BugCatcher2_Text:
	.string "These mushrooms are\n"
	.string "as tall as me!$"

FunkyForestNorth1_BugCatcher3_Text:
	.string "I'm hoping to wait\n"
	.string "those Rattata out…$"

FunkyForestNorth1_BugCatcher3_Text_Alt:
	.string "Wow! You defeated\n"
	.string "the Rattata!$"
