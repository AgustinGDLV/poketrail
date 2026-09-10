TerribleTunnelSouth1_MapScripts::
	.byte 0

TerribleTunnelSouth1_Backpacker::
	lockall
	faceplayer
	msgbox TerribleTunnelSouth1_Backpacker_Text
	releaseall
	end

TerribleTunnelSouth1_Backpacker_Text:
	.string "We're so close!$"
