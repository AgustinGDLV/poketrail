#ifndef GUARD_TRAIL_INTERFACE_H
#define GUARD_TRAIL_INTERFACE_H

void CB2_InitTrailInterface(void);
void IncrementTrailTime(u32 minutes);

#define TRAIL_MAP_WIDTH     28
#define TRAIL_MAP_HEIGHT    17

enum Checkpoint
{
    CHECKPOINT_PEONY_TOWN,
    CHECKPOINT_FUNKY_FOREST_NORTH,
    CHECKPOINT_FUNKY_FOREST_EAST,
    CHECKPOINT_FUNKY_FOREST_WEST,
    CHECKPOINT_DEEP_DESERT_WEST,
    CHECKPOINT_DEEP_DESERT_EAST,
    CHECKPOINT_MOLTEN_MOUNTAIN,
    CHECKPOINT_SMELLY_SWAMP,
    CHECKPOINT_COOL_COAST,
    CHECKPOINT_TULIP_TOWN,
    CHECKPOINT_TERRIBLE_TUNNEL_NORTH,
    CHECKPOINT_TERRIBLE_TUNNEL_SOUTH,
    CHECKPOINT_ORCHID_CITY,
    CHECKPOINT_COUNT,
};

struct CheckpointData
{
    const u8 *name;
    u8 mapNum;
    u8 warpId[5]; // facing direction
};

struct TrailInterface
{
    u8 playerSpriteId;
    u8 arrowSpriteIds[5]; // for each direction
    u16 keyHeldTimer;
};

extern const u8 gTrailMapData[TRAIL_MAP_HEIGHT][TRAIL_MAP_WIDTH];
extern struct TrailInterface gTrailInterface;
extern const struct CheckpointData gCheckpointData[CHECKPOINT_COUNT];

#endif
