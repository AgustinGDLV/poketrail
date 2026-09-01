#include "global.h"
#include "data_util.h"
#include "deck_battle.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "floor_preview.h"
#include "main.h"
#include "malloc.h"
#include "map_gen.h"
#include "map_preview.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon_gen.h"
#include "random.h"
#include "save.h"
#include "sound.h"
#include "script.h"
#include "strings.h"
#include "string_util.h"
#include "text.h"
#include "trail_interface.h"
#include "constants/songs.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"

// global floorplan
EWRAM_DATA struct Floorplan gFloorplan = {0};
EWRAM_DATA struct MapConnections gRoomMapConnections = {0};

#include "data/template_rules.h"
#include "data/character_infos.h"

// forward declarations
static u32 CountNeighbors(struct Floorplan* floorplan, u32 i);
static bool32 Visit(struct Floorplan* floorplan, u32 i);
static void ZeroFloorplan(struct Floorplan* floorplan);
static u8 GetMaxRooms(void);
static void PopulateFloorplan(struct Floorplan* floorplan);
static void AssignSpecialRoomTypes(struct Floorplan* floorplan);
static void AssignRoomMapIds(struct Floorplan* floorplan);

// Returns the number of occupied neighbors for a room index.
static u32 CountNeighbors(struct Floorplan* floorplan, u32 i)
{
    return (floorplan->layout[i-10].type >= NORMAL_ROOM) + (floorplan->layout[i-1].type >= NORMAL_ROOM) \
    + (floorplan->layout[i+1].type >= NORMAL_ROOM) + (floorplan->layout[i+10].type >= NORMAL_ROOM);
}

// The visited flags for a room are set in a saveblock bitfield.
void SetRoomAsVisited(u32 i)
{
    if (!IsRoomVisited(i))
    {
        gSaveBlock1Ptr->visitedRooms[i / 32] |= (1 << i % 32);
        IncrementTrailTime(20);

        if (!gPaletteFade.active)
        {
            struct TimeBlendSettings cachedBlend = gTimeBlend;
            u32 *bld0 = (u32*)&cachedBlend;
            u32 *bld1 = (u32*)&gTimeBlend;
            UpdateTimeOfDay();
            if (bld0[0] != bld1[0]
            || bld0[1] != bld1[1]
            || bld0[2] != bld1[2])
            {
            UpdateAltBgPalettes(PALETTES_BG);
            UpdatePalettesWithTime(PALETTES_ALL);
            }
        }
    }
}

bool32 IsRoomVisited(u32 i)
{
    return gSaveBlock1Ptr->visitedRooms[i / 32] & (1 << i % 32);
}

// "Visits" a room during generation and marks it as occupied or ignores it.
static bool32 Visit(struct Floorplan* floorplan, u32 i)
{
    if (floorplan->numRooms >= floorplan->maxRooms)
        return FALSE;
    if (floorplan->layout[i].type >= NORMAL_ROOM)
        return FALSE;
    if (CountNeighbors(floorplan, i) > 1)
        return FALSE;
    if (i != STARTING_ROOM && (RandomF() % 2))
        return FALSE;

    Enqueue(&floorplan->queue, i);
    floorplan->layout[i].type = NORMAL_ROOM;
    floorplan->occupiedRooms[floorplan->numRooms] = i;
    floorplan->numRooms += 1;
    return TRUE;
}

// Zeroes all the data in a Floorplan.
static void ZeroFloorplan(struct Floorplan* floorplan)
{
    floorplan->numRooms = 0;
    floorplan->maxRooms = 0;
    ZeroQueue(&floorplan->queue);
    ZeroStack(&floorplan->endrooms);
    floorplan->templateType = TEMPLATES_UNDERWATER;
    memset(floorplan->layout, 0, sizeof(floorplan->layout));
    memset(floorplan->occupiedRooms, 0, sizeof(floorplan->occupiedRooms));
}

// TODO: Take into account depth.
static u8 GetMaxRooms(void)
{
    return 15;
}

// TODO: Take into account depth.
static u32 GetTemplateType(void)
{
    return RandomF() % TEMPLATE_TYPES_COUNT;
}

// Creates rooms and endroom stack for an empty floorplan.
static void PopulateFloorplan(struct Floorplan* floorplan)
{
    // Set up floorplan.
    ZeroFloorplan(floorplan);
    floorplan->templateType = GetTemplateType();
    floorplan->maxRooms = GetMaxRooms();
    Enqueue(&floorplan->queue, STARTING_ROOM);
    floorplan->numRooms = 1;
    floorplan->layout[STARTING_ROOM].type = NORMAL_ROOM;
    SetRoomAsVisited(STARTING_ROOM);
    floorplan->occupiedRooms[0] = STARTING_ROOM;

    // Generate rooms.
    while (floorplan->queue.size > 0)
    {
        u32 i, x;
        bool32 createdRoom = FALSE;
        // DebugPrintQueue(&floorplan->queue);
        i = Dequeue(&floorplan->queue);
        x = i % 10;
        if (x > 1)
            createdRoom |= Visit(floorplan, i - 1);
        if (x < 9)
            createdRoom |= Visit(floorplan, i + 1);
        if (i > 20)
            createdRoom |= Visit(floorplan, i - 10);
        if (i < 70)
            createdRoom |= Visit(floorplan, i + 10);
        if (!createdRoom)
            Push(&floorplan->endrooms, i);
    }
}

// Shuffles an array in place, using the floor seed.
static void ShuffleArrayU8(u8* array, u32 size)
{
    u32 i, j, t;
    // Safety check.
    if (size == 0)
        return;

    // Code from https://stackoverflow.com/questions/6127503/shuffle-array-in-c.
    for (i = 0; i < size - 1; ++i) 
    {
        j = i + RandomF() / (UINT32_MAX / (size - i) + 1);
        t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

// Assigns special room types.
static void AssignSpecialRoomTypes(struct Floorplan* floorplan)
{
    // The farthest room is first on the stack and will always be the boss room.
    // floorplan->layout[Pop(&floorplan->endrooms)].type = BOSS_ROOM;

    // Afterwards, we shuffle the remaining endrooms and assign room types.
    ShuffleArrayU8(floorplan->endrooms.arr, floorplan->endrooms.top);
    
    // There should always be a treasure room and shop room.
    // We don't check to make sure something is popped because if
    // the stack is empty, it will pop 0 which is an invalid room coord.
    // This might be bad practice.
    floorplan->layout[Pop(&floorplan->endrooms)].type = TREASURE_ROOM;
    floorplan->layout[Pop(&floorplan->endrooms)].type = SHOP_ROOM;

    // TODO: Add more special rooms.
    floorplan->layout[Pop(&floorplan->endrooms)].type = CHALLENGE_ROOM;
}

const struct TemplateRules* GetCurrentTemplateRules(void)
{
    return &gTemplateRules[gSaveBlock1Ptr->currentTemplateType];
}

// Assigns each room a map ID.
static void AssignRoomMapIds(struct Floorplan* floorplan)
{
    u32 i;
    struct Room* room;
    const struct TemplateRules * const rules = GetCurrentTemplateRules();
    const u8 *normalPool = rules->normalRoomIds;
    u32 poolSize = rules->numNormalRooms;
    u8 *shuffled = AllocZeroed(sizeof(u8) * poolSize);

    // Assign special room types if it hasn't been done yet.
    // if (floorplan->endrooms.top > 0)
    //     AssignSpecialRoomTypes(floorplan);

    // Shuffle the normal map pool.
    memcpy(shuffled, normalPool, sizeof(u8) * poolSize);
    ShuffleArrayU8(shuffled, poolSize);

    // Loop through rooms and assign map IDs.
    for (i = 0; i < floorplan->numRooms; ++i)
    {
        room = &floorplan->layout[floorplan->occupiedRooms[i]];
        switch (room->type)
        {
            // special rooms have their room IDs in a table in gTemplateRules
            default:
                room->mapNum = rules->specialRoomIds[room->type];
                if (room->mapNum != 0)
                    break;
            // fall-through in case special room doesn't have a map assigned to it
            case NORMAL_ROOM:
                room->mapNum = shuffled[i % poolSize]; // in case numRooms > poolSize
                break;
        }
    }

    // Assign starting room as campsite.
    floorplan->layout[STARTING_ROOM].mapNum = rules->specialRoomIds[BOSS_ROOM];

    Free(shuffled);
}

// Clears all loot and encounter flags between floors.
void ClearFloorEventFlags(void)
{
    u32 i;
    for (i = TEMPLATE_EVENT_FLAGS_START; i < TEMPLATE_EVENT_FLAGS_END + 1; ++i)
        FlagClear(i);
}

// Clears all checkpoint flags in a new run.
void ClearCheckpointEventFlags(void)
{
    u32 i;
    for (i = FLAG_BOSS_ENCOUNTER; i < FLAG_UNUSED_0x4EF + 1; ++i)
        FlagClear(i);
}

// Returns whether a room in the layout exists.
bool32 DoesRoomExist(u32 i)
{
    return gFloorplan.layout[i].type >= NORMAL_ROOM;
}

// Returns whether a room in the layout is adjacent to a visited room.
bool32 IsRoomAdjacentToVisited(u32 i)
{
    if (IsRoomVisited(i - 10))
        return TRUE;
    if (IsRoomVisited(i + 10))
        return TRUE;
    if (IsRoomVisited(i - 1))
        return TRUE;
    if (IsRoomVisited(i + 1))
        return TRUE;
    return FALSE;
}

// Returns the room index in the given DIR constant direction.
u32 GetRoomInDirection(u32 dir)
{
    u32 target = 0;
    switch (dir)
    {
        case DIR_NORTH:
            target = gSaveBlock1Ptr->currentRoom - 10;
            break;
        case DIR_SOUTH:
            target = gSaveBlock1Ptr->currentRoom + 10;
            break;
        case DIR_EAST:
            target = gSaveBlock1Ptr->currentRoom + 1;
            break;
        case DIR_WEST:
            target = gSaveBlock1Ptr->currentRoom - 1;
            break;
    }
    return target;
}

// Returns whether the player is inside a template floor.
bool32 IsPlayerInFloorMap(void)
{
    return (gSaveBlock1Ptr->location.mapGroup < TEMPLATE_MAP_GROUP_END);
}


// Sets the warp destination to the room's map ID (given by room index).
void SetWarpDestinationToRoom(u32 index, u32 warpId)
{
    SetWarpDestination(GetCurrentTemplateRules()->mapGroup, gFloorplan.layout[index].mapNum, warpId, -1, -1);
}

// Executes a warp to a given room.
// Warp ID can be given using a directional constant.
bool32 TryWarpToRoom(u32 target, u32 warpId)
{
    u32 warp = warpId;

    // Don't warp if invalid room.
    if (!DoesRoomExist(target))
        return FALSE;

    // Set appropriate variables and flags.
    gSaveBlock1Ptr->currentRoom = target;
    SetRoomAsVisited(target);

    // Check if warping to start from last floor. (inelegant!)
    if (warpId == 0xFF)
    {
        gFieldCallback = FieldCB_FallWarpExit;
        warp = 0;
    }

    // Do warp.
    StoreInitialPlayerAvatarState();
    LockPlayerFieldControls();
    TryFadeOutOldMapMusic();
    WarpFadeOutScreen();
    PlayRainStoppingSoundEffect();
    SetWarpDestinationToRoom(target, warp);
    WarpIntoMap();
    SetMainCallback2(CB2_LoadMap);
    if (warpId == 0)
        gFieldCallback = FieldCB_TeleportWarpIn;
    else if (warpId != 0xFF)
        PlaySE(SE_EXIT);
    return TRUE;
}

void WarpToBossRoomOrTurn(void)
{
    struct ObjectEvent* player = &gObjectEvents[gPlayerAvatar.objectEventId];
    if (gSpecialVar_Result)
        TryWarpToRoom(GetRoomInDirection(player->facingDirection), player->facingDirection);
    else
        ObjectEventFaceOppositeDirection(player, player->facingDirection);
}

// Random loot, shops, etc. are generated using a room seed.
// This seed is currently just based off the room's unique index.
u16 GetRoomSeed(u32 index)
{
    return gSaveBlock1Ptr->floorSeed + index * 1103515245;
}

// Sets the floor RNG state to the room's RNG seed.
void SetRNGToRoomSeed(void)
{
    SeedFloorRng(GetRoomSeed(gSaveBlock1Ptr->currentFloor));
}

// Returns the type of room at a given index.
u32 GetRoomType(u32 index)
{
    return gFloorplan.layout[index].type;
}

// Generates a floorplan using the saveblock seed.
void GenerateFloorplan(void)
{
    u32 attempts = 0;
    SeedFloorRng(gSaveBlock1Ptr->floorSeed);

    // Try to make sure that the floorplan isn't too small.
    do {
        PopulateFloorplan(&gFloorplan);
    } while (gFloorplan.numRooms < MIN_ROOMS && ++attempts < 10);

    // Handle the rest of the floorplan data.
    gFloorplan.templateType = gSaveBlock1Ptr->currentTemplateType;
    AssignRoomMapIds(&gFloorplan);
    gSaveBlock1Ptr->floorSeed = gFloorplan.nextFloorSeed;
    gFloorplan.nextFloorSeed = RandomF();
    GenerateKecleonShopList();
}

// Generates the next floor and warps to its starting room.
void GoToNextFloor(void)
{
    // Update save fields.
    ++gSaveBlock1Ptr->currentFloor;
    gSaveBlock1Ptr->floorSeed = gFloorplan.nextFloorSeed;
    memset(gSaveBlock1Ptr->visitedRooms, 0, sizeof(gSaveBlock1Ptr->visitedRooms));

    // Generate the new floorplan and warp.
    GenerateFloorplan();
    ClearFloorEventFlags();
    FadeOutMapMusic(GetMapMusicFadeoutSpeed());
    SetMainCallback2(CB2_FloorPreview);
}

const struct MapHeader * const GetRoomMapHeader(u32 i)
{
    return Overworld_GetMapHeaderByGroupAndId(GetCurrentTemplateRules()->mapGroup,
                                              gFloorplan.layout[i].mapNum);
}

// Debugging function called by menu scripts.
void FloorDebugFunc(void)
{
    SetMainCallback2(CB2_FloorPreview);
}
