#ifndef GUARD_MAP_GEN_H
#define GUARD_MAP_GEN_H

#include "data_util.h"
#include "item_gen.h"

#define MAP_DEBUG   TRUE

// Floor Constants
#define LAYOUT_SIZE                 90
#define MAX_LAYOUT_WIDTH            9
#define MAX_LAYOUT_HEIGHT           8
#define MIN_ROOMS                   7
#define MAX_ROOMS                   20
#define STARTING_ROOM               45

// Template Constants
#define TEMPLATE_MAP_GROUP_END      (MAP_GROUP(INTRO_SEQUENCE) - 1) // *TODO

// Room Constants
#define ROOM_COORD(x, y)            ((x+1) + (y+1)*10)  // gFloorplan.layout is not zero-indexed

enum RoomTypes {
    NORMAL_ROOM = 1,
    BOSS_ROOM,
    TREASURE_ROOM,
    SHOP_ROOM,
    CHALLENGE_ROOM,
    NUM_ROOM_TYPES,
};

struct Room {
    enum RoomTypes type:8;
    u8 mapNum;
};

enum TemplateTypes {
    TEMPLATES_ORCHID_CITY,      // Orchid City
    TEMPLATES_UNDERWATER,       // Unused
    TEMPLATES_ICE_PATH,         // Unused
    TEMPLATES_SWEATY_SUMMIT,    // Sweaty Summit
    TEMPLATES_TERRIBLE_TUNNEL,  // Terrible Tunnel
    TEMPLATES_DEEP_DESERT,      // Deep Desert
    TEMPLATES_FUNKY_FOREST,     // Funky Forest
    TEMPLATES_PEONY_TOWN,       // Peony Town
    TEMPLATES_FRIENDLY_FIELDS,  // Friendly Fields
    TEMPLATES_COOL_COAST,       // Cool Coast
    TEMPLATES_SMELLY_SWAMP,     // Smelly Swamp
    TEMPLATES_TULIP_TOWN,       // Tulip Town
    TEMPLATES_POSITIVE_PLAINS,  // Positive Plains
    TEMPLATES_FROSTY_FLATS,     // Frosty Flats
    TEMPLATE_TYPES_COUNT,
};

enum ConnectionTypes {
    CONNECTION_TYPE_WARP,
    CONNECTION_TYPE_SEAMLESS,
};

struct Floorplan {
    u8 numRooms;
    u8 maxRooms;
    struct Room layout[LAYOUT_SIZE];
    struct Queue queue;                     // the queue of rooms to visit during algorithm
    struct Stack endrooms;                  // stores the indices of endrooms in order of decr. distance
    u8 occupiedRooms[20];                   // stores the indices of occupied rooms
    enum TemplateTypes templateType;
    u16 nextFloorSeed;
};

struct TemplateRules {
    const u8 *name;
    u8 mapGroup;
    u16 bgm;
    u8 background;
    u8 connectionType;
    s8 offsets[5][4]; // (x, y) entrance cover offsets for each direction
    u8 numNormalRooms;
    const u8* normalRoomIds;
    u8 specialRoomIds[NUM_ROOM_TYPES];
    const struct ItemPoolTable * const itemPools;
    const struct WeightedElement encounterPool[MAX_WEIGHTED_POOL_SIZE];    
};

enum Character
{
    CHAR_BUG_CATCHER,
    CHAR_BREEDER,
    CHAR_SWIMMER,
    CHAR_HIKER,
    CHARACTERS_COUNT,
};

struct CharacterInfo { // TODO: Clean up data
    const u8 * name;
    const u8 * desc;
    const u8 * unlockDesc;
    u16 item;
    u16 starters[3];
    const u16 color;
    u16 graphicsId;
    const struct SpriteTemplate * mugshotTemplate;
    const struct CompressedSpriteSheet mugshotSheet;
    const struct SpritePalette mugshotPal;
};

extern struct Floorplan gFloorplan;
extern const struct TemplateRules gTemplateRules[TEMPLATE_TYPES_COUNT];
extern const struct CharacterInfo gCharacterInfos[CHARACTERS_COUNT];

void SetRoomAsVisited(u32 i);
bool32 IsRoomVisited(u32 i);
void ClearFloorEventFlags(void);
void ClearCheckpointEventFlags(void);
bool32 DoesRoomExist(u32 i);
bool32 IsRoomAdjacentToVisited(u32 i);
u32 GetRoomInDirection(u32 dir);
bool32 IsPlayerInFloorMap(void);
void SetWarpDestinationToRoom(u32 index, u32 warpId);
bool32 TryWarpToRoom(u32 target, u32 warpId);
u32 GetRoomType(u32 index);
const struct TemplateRules* GetCurrentTemplateRules(void);
const struct MapHeader * const GetRoomMapHeader(u32 i);
u16 GetRoomSeed(u32 index);
void SetRNGToRoomSeed(void);
void GenerateFloorplan(void);
void GoToNextFloor(void);
void FloorDebugFunc(void);

#endif
