OrchidCity1_MapScripts::
	.byte 0

OrchidCity1_Porygon::
	lockall
	msgbox OrchidCity1_Porygon_Text
	applymovement 1, OrchidCity1_Movement_Jump
	waitmovement 1
	playmoncry SPECIES_PORYGON, CRY_MODE_ENCOUNTER
	fadescreenswapbuffers FADE_TO_BLACK
	callnative EndTrailJourney
	releaseall
	end

OrchidCity1_Movement_Jump:
	jump_in_place_left
	jump_in_place_right
	jump_in_place_down
	face_player
	step_end

OrchidCity1_Porygon_Text:
	.string "This is what you\n"
	.string "journeyed for…\p"
	.string "It's so cool!$"

OrchidCity1_Breeder::
	lockall
	faceplayer
	msgbox OrchidCity1_Breeder_Text
	releaseall
	end

OrchidCity1_Breeder_Text:
	.string "I've never seen a\n"
	.string "POKéMON like that!$"

OrchidCity1_Swimmer::
	lockall
	faceplayer
	msgbox OrchidCity1_Swimmer_Text
	releaseall
	end

OrchidCity1_Swimmer_Text:
	.string "That journey did not\n"
	.string "have enough water!$"

OrchidCity1_Hiker::
	lockall
	faceplayer
	msgbox OrchidCity1_Hiker_Text
	releaseall
	end

OrchidCity1_Hiker_Text:
	.string "My favorite thing to do\n"
	.string "after a trek like that?\p"
	.string "Sit down.$"

OrchidCity1_BugCatcher::
	lockall
	faceplayer
	msgbox OrchidCity1_BugCatcher_Text
	releaseall
	end

OrchidCity1_BugCatcher_Text:
	.string "I didn't think\n"
	.string "I could do it!$"

OrchidCity1_Scientist::
	lockall
	faceplayer
	msgbox OrchidCity1_Scientist_Text
	releaseall
	end

OrchidCity1_Scientist_Text:
	.string "What a weird\n"
	.string "looking fella…\p"
	.string "The POKéMON,\n"
	.string "I mean.$"
