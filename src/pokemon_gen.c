#include "global.h"
#include "data_util.h"
#include "deck_battle.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "map_gen.h"
#include "random.h"
#include "trail_interface.h"
#include "constants/songs.h"

#include "data/encounters.h"

static const u32 sCheckpointEncounterLevel[CHECKPOINT_COUNT] =
{
    5, 5, 12, 20, 28, 35, 42, 50, 55, 60, 60, 60, 60
};

// Returns what level an encounter should be based on checkpoint status.
u32 GetEncounterLevel(void)
{
    u32 checkpointsCount = GetCheckpointsCount();

    if (checkpointsCount >= CHECKPOINT_COUNT)
        return 100;
    else
        return sCheckpointEncounterLevel[checkpointsCount];
}

// Returns the species of an overworld obj. event in a room using its local ID.
u16 GetOverworldSpeciesInRoom(u32 index, u32 localId)
{
    // Advance RNG to a repeatable state based on the local ID.
    // This is to allow for consistency between saves and seed.
    SeedFloorRng(GetRoomSeed(index));
    for (u32 i = 0; i < localId; ++i)
        RandomF();

    // Check for evolutions.
    u32 species = ChooseElementFromPool(GetCurrentTemplateRules()->encounterPool);
    while (gSpeciesInfo[species].evolutions[0].method == EVO_LEVEL && gSpeciesInfo[species].evolutions[0].param <= GetEncounterLevel())
        species = gSpeciesInfo[species].evolutions[0].targetSpecies;

    return species;    
}

void InitEnemyPartyFromEncounter(void) // used by callnative
{
    u32 level = GetEncounterLevel();
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        if (gSaveBlock1Ptr->currentTemplateType == TEMPLATES_TERRIBLE_TUNNEL)
            CreateMon(&gEnemyParty[i], gTerribleEncountersInfo[gSpecialVar_0x8000][i], level + (Random() % 2) + 3, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
        else
            CreateMon(&gEnemyParty[i], gEncountersInfo[gSpecialVar_0x8000][i], level + (Random() % 2), USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
        SetMonData(&gEnemyParty[i], MON_DATA_POSITION, &i);
    }
    gDeckStruct.isBossBattle = FALSE;
    if (gSaveBlock1Ptr->currentTemplateType == TEMPLATES_TERRIBLE_TUNNEL)
        gDeckStruct.bossHPMult = UQ_4_12(2.0);
    else
        gDeckStruct.bossHPMult = 0;
    gDeckStruct.musicOverride = MUS_NONE;
}

void InitBossPartyFromEncounter(void) // used by callnative
{
    u32 species = gSpecialVar_0x8000;
    u32 level = 5;

    gDeckStruct.isBossBattle = TRUE;
    gDeckStruct.bossHPMult = UQ_4_12(1.5);
    gDeckStruct.musicOverride = MUS_NONE;

    switch (species)
    {
        default:
        case SPECIES_RATTATA:
            level = 8;
            break;
        case SPECIES_SHUCKLE:
            level = 20;
            gDeckStruct.bossHPMult = UQ_4_12(2.0);
            break;
        case SPECIES_SCYTHER:
            level = 20;
            gDeckStruct.bossHPMult = UQ_4_12(3.0);
            break;
        case SPECIES_HIPPOWDON:
            level = 28;
            gDeckStruct.bossHPMult = UQ_4_12(2.5);
            break;
        case SPECIES_LUDICOLO:
            level = 28;
            gDeckStruct.musicOverride = MUS_VS_CHAMPION;
            gDeckStruct.bossHPMult = UQ_4_12(2.0);
            break;
        case SPECIES_MAGCARGO:
            level = 34;
            gDeckStruct.bossHPMult = UQ_4_12(2.0);
            break;
        case SPECIES_MUK:
            level = 34;
            gDeckStruct.bossHPMult = UQ_4_12(2.5);
            break;
        case SPECIES_CACTURNE:
            level = 34;
            gDeckStruct.bossHPMult = UQ_4_12(2.0);
            break;
    }
    
    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        CreateMon(&gEnemyParty[i], gBossEncountersInfo[species][i], level, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
        SetMonData(&gEnemyParty[i], MON_DATA_POSITION, &i);
    }
}

