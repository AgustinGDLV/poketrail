IntroSequence_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, IntroSequence_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, IntroSequence_OnFrame
	.byte 0

IntroSequence_OnWarp:
	map_script_2 VAR_TEMP_1, 0, IntroSequence_HidePlayer
	.2byte 0

IntroSequence_OnFrame:
	map_script_2 VAR_TEMP_0, 0, IntroSequence_Introduction
	.2byte 0

IntroSequence_HidePlayer:
	setvar VAR_TEMP_1, 1
	hideobjectat OBJ_EVENT_ID_PLAYER, MAP_LITTLEROOT_TOWN
	end

IntroSequence_Introduction:
	lockall
	setvar VAR_TEMP_0, 1
	message IntroSequence_IntroductionText
	waitmessage
	dynmultipush IntroSequence_CharacterBugCatcher, 0
	dynmultipush IntroSequence_CharacterBreeder, 1
	dynmultipush IntroSequence_CharacterSwimmer, 2
	dynmultipush IntroSequence_CharacterHiker, 3
	dynmultistack 9, 3, TRUE, 4, FALSE, 0, NULL
	callnative AssignPlayerCharacter
	special SpawnCameraObject
	setobjectxy OBJ_EVENT_ID_PLAYER, 7, 9
	showobjectat OBJ_EVENT_ID_PLAYER, MAP_LITTLEROOT_TOWN
	applymovement OBJ_EVENT_ID_PLAYER, IntroSequence_Movement_WalkUp4
	switch VAR_RESULT
	case 0, IntroSequence_ChoseBugCatcher
	case 1, IntroSequence_ChoseBreeder
	case 2, IntroSequence_ChoseSwimmer
	case 3, IntroSequence_ChoseHiker
	end

IntroSequence_ChoseBugCatcher:
	message IntroSequence_ChoseBugCatcherText
	waitmessage
	goto IntroSequence_AskPlayerName
	end

IntroSequence_ChoseBreeder:
	message IntroSequence_ChoseBreederText
	waitmessage
	goto IntroSequence_AskPlayerName
	end

IntroSequence_ChoseSwimmer:
	message IntroSequence_ChoseSwimmerText
	waitmessage
	goto IntroSequence_AskPlayerName
	end

IntroSequence_ChoseHiker:
	message IntroSequence_ChoseHikerText
	waitmessage
	goto IntroSequence_AskPlayerName
	end

IntroSequence_AskPlayerName:
	callnative AssignPlayerName
	waitstate
	msgbox IntroSequence_ConfirmName, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, IntroSequence_AskPlayerName
	goto_if_eq VAR_RESULT, YES, IntroSequence_AskStarter
	end

IntroSequence_AskStarter:
	message IntroSequence_AskStarterText
	waitmessage
	callnative GetCharacterId
	switch VAR_RESULT
	case 0, IntroSequence_BugCatcherChooseStarter
	case 1, IntroSequence_BreederChooseStarter
	case 2, IntroSequence_SwimmerChooseStarter
	case 3, IntroSequence_HikerChooseStarter
	end

IntroSequence_BugCatcherChooseStarter:
	message IntroSequence_AskStarterText2
	waitmessage
	dynmultipush IntroSequence_Caterpie, 0
	dynmultipush IntroSequence_Kricketot, 1
	dynmultistack 9, 2, TRUE, 4, FALSE, 0, NULL
	callnative AssignStarterToPlayer
	goto IntroSequence_StartJourney
	end

IntroSequence_BreederChooseStarter:
	message IntroSequence_AskStarterText2
	waitmessage
	dynmultipush IntroSequence_Swinub, 1
	dynmultipush IntroSequence_Budew, 2
	dynmultistack 10, 2, TRUE, 4, FALSE, 0, NULL
	callnative AssignStarterToPlayer
	goto IntroSequence_StartJourney
	end

IntroSequence_SwimmerChooseStarter:
	message IntroSequence_AskStarterText2
	waitmessage
	dynmultipush IntroSequence_Slowpoke, 0
	dynmultipush IntroSequence_Krabby, 1
	dynmultistack 9, 2, TRUE, 4, FALSE, 0, NULL
	callnative AssignStarterToPlayer
	goto IntroSequence_StartJourney
	end

IntroSequence_HikerChooseStarter:
	message IntroSequence_AskStarterText2
	waitmessage
	dynmultipush IntroSequence_Diglett, 0
	dynmultipush IntroSequence_Whismur, 2
	dynmultistack 9, 2, TRUE, 4, FALSE, 0, NULL
	callnative AssignStarterToPlayer
	goto IntroSequence_StartJourney
	end

IntroSequence_StartJourney:
	msgbox IntroSequence_StartJourneyText
	applymovement 1, IntroSequence_Movement_WalkDown
	applymovement OBJ_EVENT_ID_PLAYER, IntroSequence_Movement_FaceUp
	waitmovement 1
	additem ITEM_POTION, 5
	playfanfare MUS_OBTAIN_ITEM
	msgbox IntroSequence_StarterKit
	msgbox IntroSequence_JourneyQuestion, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, 0, IntroSequence_StartJourney_End
IntroSequence_StartJourney_Explanation::
	msgbox IntroSequence_TrailExplanation, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, 0, IntroSequence_StartJourney_Explanation
IntroSequence_StartJourney_End::
	msgbox IntroSequence_GoAhead
	closemessage
	special RemoveCameraObject
	applymovement OBJ_EVENT_ID_PLAYER, IntroSequence_Movement_WalkDown
	waitmovement 0
	releaseall
	end

IntroSequence_Trigger::
	msgbox IntroSequence_GoOnThen
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, IntroSequence_Movement_WalkDown
	waitmovement 0
	end

IntroSequence_IntroductionText:
	.string "Hello trainer!\p"
	.string "As you are no doubt aware, there\n"
	.string "is rumored to be a mysterious\l"
	.string "POKéMON in Orchid City.\p"
	.string "That is why, out of dozens of\n"
	.string "applicants, I've chosen you, a…\p$"

IntroSequence_ChoseBugCatcherText:
	.string "…fearless bug catcher!\p"
	.string "You might be young, but I\n"
	.string "know you can handle this!\p"
	.string "…\n"
	.string "What was your name again?\p$"

IntroSequence_ChoseBreederText:
	.string "…savvy POKéMON breeder!\p"
	.string "The wild POKéMON that await\n"
	.string "will be no match for you!\p"
	.string "…\n"
	.string "What was your name again?\p$"

IntroSequence_ChoseSwimmerText:
	.string "…hearty swimmer!\p"
	.string "You can certainly endure\n"
	.string "the journey waiting for you!\p"
	.string "…\n"
	.string "What was your name again?\p$"

IntroSequence_ChoseHikerText:
	.string "…rugged hiker!\p"
	.string "The terrain ahead will\n"
	.string "be no problem for you!\p"
	.string "…\n"
	.string "What was your name again?\p$"

IntroSequence_ConfirmName:
	.string "{PLAYER}, is that right?$"

IntroSequence_AskStarterText:
	.string "Right, it was {PLAYER}!\p"
	.string "Well, {PLAYER}, I am sure\n"
	.string "you and your…\p$"

IntroSequence_AskStarterText2:
	.string "You have a POKéMON right?\p$"

IntroSequence_StartJourneyText:
	.string "Good! You will certainly need\n"
	.string "{STR_VAR_1} for this journey!\p"
	.string "I will need you both to travel\n"
	.string "to Orchid City and study the\l"
	.string "POKéMON for me.\p"
	.string "Let me give you some supplies\n"
	.string "to get you started!$"

IntroSequence_StarterKit:
	.string "{PLAYER} received a\n"
	.string "starter kit!$"

IntroSequence_JourneyQuestion:
	.string "Is this your first time\n"
	.string "outside of Peony Town?$"

IntroSequence_TrailExplanation:
	.string "At any point on the trail,\n"
	.string "you can press A to camp.\p"
	.string "You can also press L to open\n"
	.string "your bag and R to see your party.\p"
	.string "When you camp, you can search\n"
	.string "for items, battle, or rest.\p"
	.string "Fainted POKéMON need to rest\n"
	.string "for 12 hours to re-energize.\p"
	.string "When you battle, you can press\n"
	.string "START to swap battler positions.\p"
	.string "This does not consume a move, and\n"
	.string "this even boosts some POKéMON.\p"
	.string "You can also press L to run\n"
	.string "if no actions are selected.\p"
	.string "Does that all make sense?\p$"

IntroSequence_GoAhead:
	.string "Well…\n"
	.string "You should be all set!\p"
	.string "Be safe!$"

IntroSequence_GoOnThen:
	.string "Wrong way!\n"
	.string "You'll be fine!$"

IntroSequence_CharacterBugCatcher:
	.string "Bug Catcher$"

IntroSequence_CharacterBreeder:
	.string "Breeder$"

IntroSequence_CharacterSwimmer:
	.string "Swimmer$"

IntroSequence_CharacterHiker:
	.string "Hiker$"

IntroSequence_Caterpie:
	.string "Caterpie$"

IntroSequence_Kricketot:
	.string "Kricketot$"

IntroSequence_Ledyba:
	.string "Ledyba$"

IntroSequence_Mareep:
	.string "Mareep$"

IntroSequence_Swinub:
	.string "Swinub$"

IntroSequence_Budew:
	.string "Budew$"

IntroSequence_Slowpoke:
	.string "Slowpoke$"

IntroSequence_Krabby:
	.string "Krabby$"

IntroSequence_Wingull:
	.string "Wingull$"

IntroSequence_Diglett:
	.string "Diglett$"

IntroSequence_Roggenrola:
	.string "Roggenrola$"

IntroSequence_Whismur:
	.string "Whismur$"

IntroSequence_Movement_WalkUp4:
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

IntroSequence_Movement_FaceUp:
	face_up
	step_end

IntroSequence_Movement_WalkDown:
	walk_down
	step_end
