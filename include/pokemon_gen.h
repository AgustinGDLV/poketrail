#ifndef GUARD_POKEMON_GEN_H
#define GUARD_POKEMON_GEN_H

u32 GetEncounterLevel(void);
u16 GetOverworldSpeciesInRoom(u32 index, u32 localId);
void InitEnemyPartyFromEncounter(void);

extern const u16 gEncountersInfo[NUM_SPECIES][POSITIONS_COUNT];
extern const u16 gTerribleEncountersInfo[NUM_SPECIES][POSITIONS_COUNT];
extern const u16 gBossEncountersInfo[NUM_SPECIES][POSITIONS_COUNT];

#endif
