SweatySummit1_MapScripts::
	.byte 0


SweatySummit1_MagcargoBoss::
	lock
	msgbox SweatySummit1_MagcargoText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SweatySummit1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	waitmovement SPECIES_MAGCARGO
    playmoncry SPECIES_MAGCARGO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_MAGCARGO
	callnative InitBossPartyFromEncounter
	callnative OpenDeckBattle
	removeobject 1
	releaseall
    end

SweatySummit1_End:
	closemessage
	releaseall
	end

SweatySummit1_Swimmer::
	lockall
	faceplayer
	msgbox SweatySummit1_Swimmer_Text
	releaseall
	end

SweatySummit1_Woman::
	lockall
	faceplayer
	msgbox SweatySummit1_Woman_Text
	releaseall
	end

SweatySummit1_MagcargoText:
	.string "Upon closer inspection, you\n"
	.string "realize this is a Magcargo…\p"
	.string "Fight?$"

SweatySummit1_Swimmer_Text:
	.string "It's so hot!\p"
	.string "I wish I could\n"
	.string "go swimming…$"

SweatySummit1_Woman_Text:
	.string "Am I on my way\n"
	.string "to Orchid City?\p"
	.string "No, I just come here\n"
	.string "to do my yoga!$"
