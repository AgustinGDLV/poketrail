TerribleTunnelNorth1_MapScripts::
	.byte 0

TerribleTunnelNorth1_Breeder::
	lockall
	faceplayer
	msgbox TerribleTunnelNorth1_Breeder_Text
	releaseall
	end

TerribleTunnelNorth1_Breeder_Text:
	.string "I'm stopping to rest,\p"
	.string "the POKéMON here feel\n"
	.string "terribly strong!$"
