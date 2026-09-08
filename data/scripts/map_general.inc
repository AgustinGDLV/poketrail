EventScript_OverworldEncounterStart::
    lock
    playse SE_PIN
	applymovement VAR_0x8001, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8001, Common_Movement_FacePlayer
	waitmovement 0
    playmoncry VAR_0x8000, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	callnative InitEnemyPartyFromEncounter
	callnative OpenDeckBattle
	removeobject VAR_0x8001
	releaseall
    end

EventScript_ItemBall::
	callnative ChooseOverworldItem
	finditem VAR_0x8000
	end

EventScript_KecleonShop::
    lock
	faceplayer
	playmoncry SPECIES_KECLEON, CRY_MODE_NORMAL
	callnative InitKecleonShop
	waitmoncry
	releaseall
	end

EventScript_EnterBossRoom_Text:
	.string "You sense a strong presence\n"
	.string "ahead… Should you enter?$"

EventScript_EnterBossRoom::
	goto_if_set FLAG_BOSS_ENCOUNTER, EventScript_WarpToBossRoom
	lock
	msgbox EventScript_EnterBossRoom_Text, MSGBOX_YESNO
	waitmessage
	releaseall
	callnative WarpToBossRoomOrTurn 
	end

EventScript_WarpToBossRoom:
	setvar VAR_RESULT, 1
	callnative WarpToBossRoomOrTurn
	end

EventScript_Movement_WalkSouth:
	walk_down
	step_end

EventScript_Movement_WalkNorth:
	walk_up
	step_end

EventScript_Movement_WalkEast:
	walk_right
	step_end

EventScript_Movement_WalkWest:
	walk_left
	step_end

EventScript_WalkSouth:
	applymovement OBJ_EVENT_ID_PLAYER, EventScript_Movement_WalkSouth
	waitmovement 0
	releaseall
	end

EventScript_WalkNorth:
	applymovement OBJ_EVENT_ID_PLAYER, EventScript_Movement_WalkNorth
	waitmovement 0
	releaseall
	end

EventScript_WalkEast:
	applymovement OBJ_EVENT_ID_PLAYER, EventScript_Movement_WalkEast
	waitmovement 0
	releaseall
	end

EventScript_WalkWest:
	applymovement OBJ_EVENT_ID_PLAYER, EventScript_Movement_WalkWest
	waitmovement 0
	releaseall
	end

EventScript_WalkForward:
	specialvar VAR_RESULT, GetPlayerFacingDirection
	switch VAR_RESULT
	case DIR_SOUTH, EventScript_WalkSouth
	case DIR_NORTH, EventScript_WalkNorth
	case DIR_EAST, EventScript_WalkEast
	case DIR_WEST, EventScript_WalkWest

EventScript_WalkOppposite:
	specialvar VAR_RESULT, GetPlayerFacingDirection
	switch VAR_RESULT
	case DIR_SOUTH, EventScript_WalkNorth
	case DIR_NORTH, EventScript_WalkSouth
	case DIR_EAST, EventScript_WalkWest
	case DIR_WEST, EventScript_WalkEast

EventScript_EnterBossRoomFromConnection::
	goto_if_set FLAG_BOSS_ENCOUNTER, EventScript_End
	lock
	playse SE_PIN
	msgbox EventScript_EnterBossRoom_Text, MSGBOX_YESNO
	waitmessage
	releaseall
	switch VAR_RESULT
	case 1, EventScript_WalkForward
	case 0, EventScript_WalkOppposite

EventScript_End:
	closemessage
	releaseall
	end

EventScript_GoToNextFloor_Text:
	.string "Continue to the next floor?$"

EventScript_GoToNextFloor::
	lock
	goto_if_unset FLAG_BOSS_ENCOUNTER, EventScript_End
	msgbox EventScript_GoToNextFloor_Text, MSGBOX_YESNO
	waitmessage
	releaseall
	goto_if_eq VAR_RESULT, 0, EventScript_End
	callnative GoToNextFloor
	end

EventScript_Campfire::
	lockall
	message EventScript_Campfire_Text
	waitmessage
	dynmultipush EventScript_Rest_Text, 0
	dynmultipush EventScript_Resume_Text, 1
	dynmultipush EventScript_Stay_Text, 2
	dynmultistack 21, 6, FALSE, 3, FALSE, 0, NULL
	releaseall
	switch VAR_RESULT
	case 0, EventScript_Campfire_Rest
	case 1, EventScript_Campfire_Resume
	case 2, EventScript_End
	end

EventScript_Campfire_Rest:
	message EventScript_RestHowLong_Text
	waitmessage
	dynmultipush EventScript_3_Hours_Text, 0
	dynmultipush EventScript_6_Hours_Text, 1
	dynmultipush EventScript_12_Hours_Text, 2
	dynmultipush EventScript_Exit_Text, 3
	dynmultistack 21, 4, TRUE, 4, FALSE, 0, NULL
	goto_if_eq VAR_RESULT, 3, EventScript_End
	fadescreenswapbuffers FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	callnative RestAtCampsite
	fadescreenswapbuffers FADE_FROM_BLACK
	releaseall
	closemessage
	end

EventScript_Campfire_Resume:
	callnative GoToTrailMap
	end

EventScript_Campfire_Text:
	.string "What would you\n"
	.string "like to do?$"

EventScript_Rest_Text:
	.string "Rest$"

EventScript_Resume_Text:
	.string "Travel$"

EventScript_Stay_Text:
	.string "Stay$"

EventScript_RestHowLong_Text:
	.string "Rest how long?$"

EventScript_3_Hours_Text:
	.string "3 hours$"

EventScript_6_Hours_Text:
	.string "6 hours$"

EventScript_12_Hours_Text:
	.string "12 hours$"

EventScript_Exit_Text:
	.string "Exit$"
