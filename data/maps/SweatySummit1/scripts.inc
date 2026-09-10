SweatySummit1_MapScripts::
	.byte 0


SweatySummit1_TorkoalBoss::
	lock
	msgbox SweatySummit1_TorkoalText, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SweatySummit1_End
	playse SE_PIN
	applymovement 1, Common_Movement_ExclamationMark
	applymovement 2, Common_Movement_ExclamationMark
	waitmovement SPECIES_TORKOAL
    playmoncry SPECIES_TORKOAL, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_0x8000, SPECIES_TORKOAL
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

SweatySummit1_TorkoalText:
	.string "Upon closer inspection, you\p"
	.string "realize this is a Torkoal…\p"
	.string "Fight?$"

SweatySummit1_Swimmer_Text:
	.string "I'm not convinced\n"
	.string "this isn't water.$"

SweatySummit1_Woman_Text:
	.string "Am I on my way\p"
	.string "to Orchid City?!\n"
	.string "No, I just come here\p"
	.string "to do my yoga!$"
