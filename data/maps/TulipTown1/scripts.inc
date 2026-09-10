TulipTown1_MapScripts::
	.byte 0

TulipTown1_Man::
	lockall
	faceplayer
	msgbox TulipTown1_Man_Text
	releaseall
	end

TulipTown1_Man_Text:
	.string "Welcome to Tulip Town!$"

TulipTown1_Woman1::
	lockall
	faceplayer
	msgbox TulipTown1_Woman1_Text
	releaseall
	end

TulipTown1_Woman1_Text:
	.string "How has your journey\n"
	.string "to Orchid City been?$"

TulipTown1_Woman2::
	lockall
	faceplayer
	msgbox TulipTown1_Woman2_Text
	giveitem ITEM_REVIVE, 4
	releaseall
	end

TulipTown1_Woman2_Text:
	.string "You must be tired!\p"
	.string "Take this for the\n"
	.string "final stretch!$"

TulipTown1_Scientist::
	lockall
	faceplayer
	msgbox TulipTown1_Scientist_Text
	releaseall
	end

TulipTown1_Scientist_Text:
	.string "Your PROF sent\n"
	.string "you too?\p"
	.string "This is hard…$"

TulipTown1_BugCatcher::
	lockall
	faceplayer
	msgbox TulipTown1_BugCatcher_Text
	releaseall
	end

TulipTown1_BugCatcher_Text:
	.string "I love tulips!$"
