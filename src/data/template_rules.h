#include "map_gen.h"
#include "item_gen.h"
#include "battle.h"

// This file contains all the data for shop, loot, Pokemon, and room pools
// for each template map group. It also contains offsets for exit covers.

// Items use an array of "pool tables." The array contains ITEM_TIER_COUNT
// different pool tables. These tables contain pointers to medicine, battle item
// hold item, upgrade, and treasure pools. If set to {0}, the code will just
// read from the default pools below.

// These item pools are usually used for all templates, but there is
// more variability in holdItems and treasures.
static const struct WeightedElement sDefaultMedicinePools[ITEM_TIER_COUNT][MAX_WEIGHTED_POOL_SIZE] =
{
    [ITEM_TIER_1] = {
        {ITEM_SUPER_POTION,     100},
        {ITEM_SITRUS_BERRY,     80},
        {ITEM_FULL_HEAL,        40},
        {ITEM_FULL_RESTORE,     35},
    },
    [ITEM_TIER_2] = {
        {ITEM_SUPER_POTION,     80},
        {ITEM_SITRUS_BERRY,     80},
        {ITEM_FULL_HEAL,        50},
        {ITEM_FULL_RESTORE,     45},
    },
    [ITEM_TIER_3] = {
        {ITEM_SUPER_POTION,     40},
        {ITEM_SITRUS_BERRY,     60},
        {ITEM_FULL_HEAL,        60},
        {ITEM_FULL_RESTORE,     95},
    },
    [ITEM_TIER_4] = {
        {ITEM_SUPER_POTION,     20},
        {ITEM_SITRUS_BERRY,     60},
        {ITEM_FULL_HEAL,        80},
        {ITEM_FULL_RESTORE,     95},
    },
    [ITEM_TIER_5] = {
        {ITEM_SITRUS_BERRY,     60},
        {ITEM_FULL_HEAL,        100},
        {ITEM_FULL_RESTORE,     95},
    },
};

static const struct WeightedElement sDefaultBattleItemPools[ITEM_TIER_COUNT][MAX_WEIGHTED_POOL_SIZE] =
{
    [ITEM_TIER_1] = {
        {ITEM_MAX_MUSHROOMS,    5},
    },
    [ITEM_TIER_2] = {
        {ITEM_MAX_MUSHROOMS,    5},
    },
    [ITEM_TIER_3] = {
        {ITEM_MAX_MUSHROOMS,    5},
    },
    [ITEM_TIER_4] = {
        {ITEM_MAX_MUSHROOMS,    5},
    },
    [ITEM_TIER_5] = {
        {ITEM_MAX_MUSHROOMS,    5},
    },
};

static const struct WeightedElement sDefaultHoldItemPools[ITEM_TIER_COUNT][MAX_WEIGHTED_POOL_SIZE] =
{
    [ITEM_TIER_1] = {
        {ITEM_LEFTOVERS,        255},
    },
    [ITEM_TIER_2] = {
        {ITEM_LEFTOVERS,        255},
    },
    [ITEM_TIER_3] = {
        {ITEM_LEFTOVERS,        255},
    },
    [ITEM_TIER_4] = {
        {ITEM_LEFTOVERS,        255},
    },
    [ITEM_TIER_5] = {
        {ITEM_LEFTOVERS,        255},
    },
};

static const struct WeightedElement sDefaultUpgradePools[ITEM_TIER_COUNT][MAX_WEIGHTED_POOL_SIZE] =
{
    [ITEM_TIER_1] = {
        {ITEM_ABILITY_PATCH,    100},
        {ITEM_ABILITY_CAPSULE,  80},
        {ITEM_PP_MAX,           40},
        {ITEM_SHINY_STONE,      35},
    },
    [ITEM_TIER_2] = {
        {ITEM_ABILITY_PATCH,    80},
        {ITEM_ABILITY_CAPSULE,  80},
        {ITEM_PP_MAX,           50},
        {ITEM_SHINY_STONE,      45},
    },
    [ITEM_TIER_3] = {
        {ITEM_ABILITY_PATCH,    40},
        {ITEM_ABILITY_CAPSULE,  60},
        {ITEM_PP_MAX,           60},
        {ITEM_SHINY_STONE,      95},
    },
    [ITEM_TIER_4] = {
        {ITEM_ABILITY_PATCH,    20},
        {ITEM_ABILITY_CAPSULE,  60},
        {ITEM_PP_MAX,           80},
        {ITEM_SHINY_STONE,      95},
    },
    [ITEM_TIER_5] = {
        {ITEM_ABILITY_CAPSULE,  60},
        {ITEM_PP_MAX,           100},
        {ITEM_SHINY_STONE,      95},
    },
};

static const struct WeightedElement sDefaultTreasurePools[ITEM_TIER_COUNT][MAX_WEIGHTED_POOL_SIZE] =
{
    [ITEM_TIER_1] = {
        {ITEM_RELIC_CROWN,        1},
    },
    [ITEM_TIER_2] = {
        {ITEM_RELIC_CROWN,        1},
    },
    [ITEM_TIER_3] = {
        {ITEM_RELIC_CROWN,        1},
    },
    [ITEM_TIER_4] = {
        {ITEM_RELIC_CROWN,        1},
    },
    [ITEM_TIER_5] = {
        {ITEM_RELIC_CROWN,        1},
    },
};

const struct ItemPoolTable gDefaultItemPools[ITEM_TIER_COUNT] =
{
    [ITEM_TIER_1] = {
        .medicines = sDefaultMedicinePools[ITEM_TIER_1],
        .battleItems = sDefaultBattleItemPools[ITEM_TIER_1],
        .holdItems = sDefaultHoldItemPools[ITEM_TIER_1],
        .upgrades = sDefaultUpgradePools[ITEM_TIER_1],
        .treasures = sDefaultTreasurePools[ITEM_TIER_1],
    },
    [ITEM_TIER_2] = {
        .medicines = sDefaultMedicinePools[ITEM_TIER_2],
        .battleItems = sDefaultBattleItemPools[ITEM_TIER_2],
        .holdItems = sDefaultHoldItemPools[ITEM_TIER_2],
        .upgrades = sDefaultUpgradePools[ITEM_TIER_2],
        .treasures = sDefaultTreasurePools[ITEM_TIER_2],
    },
    [ITEM_TIER_3] = {
        .medicines = sDefaultMedicinePools[ITEM_TIER_3],
        .battleItems = sDefaultBattleItemPools[ITEM_TIER_3],
        .holdItems = sDefaultHoldItemPools[ITEM_TIER_3],
        .upgrades = sDefaultUpgradePools[ITEM_TIER_3],
        .treasures = sDefaultTreasurePools[ITEM_TIER_3],
    },
    [ITEM_TIER_4] = {
        .medicines = sDefaultMedicinePools[ITEM_TIER_4],
        .battleItems = sDefaultBattleItemPools[ITEM_TIER_4],
        .holdItems = sDefaultHoldItemPools[ITEM_TIER_4],
        .upgrades = sDefaultUpgradePools[ITEM_TIER_4],
        .treasures = sDefaultTreasurePools[ITEM_TIER_4],
    },
    [ITEM_TIER_5] = {
        .medicines = sDefaultMedicinePools[ITEM_TIER_5],
        .battleItems = sDefaultBattleItemPools[ITEM_TIER_5],
        .holdItems = sDefaultHoldItemPools[ITEM_TIER_5],
        .upgrades = sDefaultUpgradePools[ITEM_TIER_5],
        .treasures = sDefaultTreasurePools[ITEM_TIER_5],
    },
};

// Underwater Template Pools
static const u8 sUnderwaterNormalRooms[] =
{
    MAP_NUM(UNDERWATER_TEMPLATES_ROOM1),
    MAP_NUM(UNDERWATER_TEMPLATES_ROOM2),
    MAP_NUM(UNDERWATER_TEMPLATES_ROOM3),
    MAP_NUM(UNDERWATER_TEMPLATES_ROOM4),
    MAP_NUM(UNDERWATER_TEMPLATES_ROOM5),
};

// City Template Pools
static const u8 sCityNormalRooms[] =
{
    MAP_NUM(CITY_TEMPLATES_ROOM1),
    MAP_NUM(CITY_TEMPLATES_ROOM2),
    MAP_NUM(CITY_TEMPLATES_ROOM3),
    MAP_NUM(CITY_TEMPLATES_ROOM4),
    MAP_NUM(CITY_TEMPLATES_ROOM5),
};

// Ice Path Template Pools
static const u8 sIcePathNormalRooms[] =
{
    MAP_NUM(ICE_CAVE_TEMPLATES_ROOM1),
    MAP_NUM(ICE_CAVE_TEMPLATES_ROOM2),
    MAP_NUM(ICE_CAVE_TEMPLATES_ROOM3),
};

// Volcano Template Pools
static const u8 sVolcanoNormalRooms[] =
{
    MAP_NUM(VOLCANO_TEMPLATES_ROOM1),
};

// Cave Template Pools
static const u8 sCaveNormalRooms[] =
{
    MAP_NUM(CAVE_TEMPLATES_ROOM1),
};

// Desert Template Pools
static const u8 sDesertNormalRooms[] =
{
    MAP_NUM(DESERT_TEMPLATES_ROOM1),
    MAP_NUM(DESERT_TEMPLATES_ROOM2),
};

// Forest Template Pools
static const u8 sForestNormalRooms[] =
{
    MAP_NUM(FOREST_TEMPLATES_ROOM1),
};

// Fields Template Pools
static const u8 sFieldsNormalRooms[] =
{
    MAP_NUM(FIELDS_TEMPLATES_ROOM1),
    MAP_NUM(FIELDS_TEMPLATES_ROOM2),
    MAP_NUM(FIELDS_TEMPLATES_ROOM3),
    MAP_NUM(FIELDS_TEMPLATES_ROOM4),
    MAP_NUM(FIELDS_TEMPLATES_ROOM5),
    MAP_NUM(FIELDS_TEMPLATES_ROOM6),
    MAP_NUM(FIELDS_TEMPLATES_ROOM7),
};

const struct TemplateRules gTemplateRules[TEMPLATE_TYPES_COUNT] = 
{
    [TEMPLATES_UNDERWATER] =
    {
        .name = COMPOUND_STRING("Corsola Reefs"),
        .mapGroup = MAP_GROUP(UNDERWATER_TEMPLATES_ROOM1),
        .bgm = MUS_UNDERWATER,
        .background = BG_UNDERWATER,
        .connectionType = CONNECTION_TYPE_WARP,
        .offsets = {
            [DIR_NORTH] = {0, -1, 1, 1},
            [DIR_SOUTH] = {0, 1, 1, 1},
            [DIR_EAST] = {0, 0, 2, 1},
            [DIR_WEST] = {-1, 0, 2, 1},
        },
        .numNormalRooms = ARRAY_COUNT(sUnderwaterNormalRooms),
        .normalRoomIds = sUnderwaterNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(UNDERWATER_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(UNDERWATER_TEMPLATES_TREASURE_ROOM),
            [SHOP_ROOM] = MAP_NUM(UNDERWATER_TEMPLATES_SHOP_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_KRABBY, 100},
            {SPECIES_SLOWPOKE, 100},
            {SPECIES_CLAMPERL, 100},
            {SPECIES_CHINCHOU, 100},
            {SPECIES_CORSOLA, 100},
        }
    },

    [TEMPLATES_ORCHID_CITY] =
    {
        .name = COMPOUND_STRING("Orchid City"),
        .mapGroup = MAP_GROUP(CITY_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_CELADON,
        .background = BG_CITY,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-1, -5, 4, 5},
            [DIR_SOUTH] = {-1, 1, 4, 2},
            [DIR_EAST] = {1, -3, 2, 6},
            [DIR_WEST] = {-2, -3, 2, 6},
        },
        .numNormalRooms = ARRAY_COUNT(sCityNormalRooms),
        .normalRoomIds = sCityNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(CITY_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(CITY_TEMPLATES_TREASURE_ROOM),
            [SHOP_ROOM] = MAP_NUM(CITY_TEMPLATES_SHOP_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {}
    },

    [TEMPLATES_ICE_PATH] =
    {
        .name = COMPOUND_STRING("Vanillite Path"),
        .mapGroup = MAP_GROUP(ICE_CAVE_TEMPLATES_CONNECTIONS),
        .bgm = MUS_HG_ICE_PATH,
        .background = BG_ICE_PATH,
        .connectionType = CONNECTION_TYPE_WARP,
        .offsets = {
            [DIR_NORTH] = {0, -1, 1, 1},
            [DIR_SOUTH] = {0, 1, 1, 1},
            [DIR_EAST] = {0, 0, 2, 1},
            [DIR_WEST] = {-1, 0, 2, 1},
        },
        .numNormalRooms = ARRAY_COUNT(sIcePathNormalRooms),
        .normalRoomIds = sIcePathNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(ICE_CAVE_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(ICE_CAVE_TEMPLATES_TREASURE_ROOM),
            [SHOP_ROOM] = MAP_NUM(ICE_CAVE_TEMPLATES_SHOP_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_SWINUB, 100},
            {SPECIES_DELIBIRD, 100},
            {SPECIES_SNOVER, 100},
        }
    },

    [TEMPLATES_SWEATY_SUMMIT] =
    {
        .name = COMPOUND_STRING("Sweaty Summit"),
        .mapGroup = MAP_GROUP(VOLCANO_TEMPLATES_CONNECTIONS),
        .bgm = MUS_MT_CHIMNEY,
        .background = BG_VOLCANO,
        .connectionType = CONNECTION_TYPE_WARP,
        .offsets = {
            [DIR_NORTH] = {0, -1, 1, 1},
            [DIR_SOUTH] = {0, 1, 1, 1},
            [DIR_EAST] = {0, 0, 2, 1},
            [DIR_WEST] = {-1, 0, 2, 1},
        },
        .numNormalRooms = ARRAY_COUNT(sVolcanoNormalRooms),
        .normalRoomIds = sVolcanoNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(VOLCANO_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(VOLCANO_TEMPLATES_TREASURE_ROOM),
            [SHOP_ROOM] = MAP_NUM(VOLCANO_TEMPLATES_SHOP_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_VULPIX, 100},
            {SPECIES_SLUGMA, 100},
        }
    },

    [TEMPLATES_TERRIBLE_TUNNEL] =
    {
        .name = COMPOUND_STRING("Terrible Tunnel"),
        .mapGroup = MAP_GROUP(CAVE_TEMPLATES_CONNECTIONS),
        .bgm = MUS_DP_MT_CORONET,
        .background = BG_CAVE,
        .connectionType = CONNECTION_TYPE_WARP,
        .offsets = {
            [DIR_NORTH] = {0, -1, 1, 1},
            [DIR_SOUTH] = {0, 1, 1, 1},
            [DIR_EAST] = {0, 0, 2, 1},
            [DIR_WEST] = {-1, 0, 2, 1},
        },
        .numNormalRooms = ARRAY_COUNT(sCaveNormalRooms),
        .normalRoomIds = sCaveNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(CAVE_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(CAVE_TEMPLATES_TREASURE_ROOM),
            [SHOP_ROOM] = MAP_NUM(CAVE_TEMPLATES_SHOP_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_CLEFFA, 100},
            {SPECIES_ZUBAT, 100},
            {SPECIES_DIGLETT, 100},
        }
    },

    [TEMPLATES_DEEP_DESERT] =
    {
        .name = COMPOUND_STRING("Deep Desert"),
        .mapGroup = MAP_GROUP(DESERT_TEMPLATES_CONNECTIONS),
        .bgm = MUS_ROUTE113,
        .background = BG_DESERT,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-1, -2, 4, 2},
            [DIR_SOUTH] = {-1, 1, 4, 2},
            [DIR_EAST] = {1, -2, 2, 4},
            [DIR_WEST] = {-2, -2, 2, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sDesertNormalRooms),
        .normalRoomIds = sDesertNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(DESERT_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(DESERT_TEMPLATES_TREASURE_ROOM),
            [SHOP_ROOM] = MAP_NUM(DESERT_TEMPLATES_SHOP_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_BALTOY, 100},
            {SPECIES_CACNEA, 100},
            {SPECIES_HIPPOPOTAS, 100},
            {SPECIES_SKORUPI, 100},
            {SPECIES_TRAPINCH, 100},
        }
    },

    [TEMPLATES_FUNKY_FOREST] =
    {
        .name = COMPOUND_STRING("Funky Forest"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_FOREST,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_APPLIN, 100},
            {SPECIES_SPEAROW, 100},
        }
    },

    [TEMPLATES_FRIENDLY_FIELDS] =
    {
        .name = COMPOUND_STRING("Friendly Fields"),
        .mapGroup = MAP_GROUP(FIELDS_TEMPLATES_CONNECTIONS),
        .bgm = MUS_ROUTE101,
        .background = BG_GRASS,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {0, -1, 2, 1},
            [DIR_SOUTH] = {0, 1, 2, 1},
            [DIR_EAST] = {1, -1, 1, 4},
            [DIR_WEST] = {-1, -1, 1, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sFieldsNormalRooms),
        .normalRoomIds = sFieldsNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FIELD_TEMPLATES_CAMPSITE),
            [TREASURE_ROOM] = MAP_NUM(FIELD_TEMPLATES_CAMPSITE),
            [SHOP_ROOM] = MAP_NUM(FIELD_TEMPLATES_CAMPSITE),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_RATTATA,       100},
            {SPECIES_POOCHYENA,     100},
            {SPECIES_PONYTA,        100},
            {SPECIES_SPEAROW,       100},
            {SPECIES_BELLSPROUT,    100},
            {SPECIES_EKANS,         100},
            {SPECIES_HOPPIP,        100},
            {SPECIES_SWABLU,        50},
            {SPECIES_MAREEP,        50},
            {SPECIES_CHINGLING,     50},
        }
    },

    [TEMPLATES_PEONY_TOWN] =
    {
        .name = COMPOUND_STRING("Peony Town"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_CITY,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {}
    },

    [TEMPLATES_COOL_COAST] =
    {
        .name = COMPOUND_STRING("Cool Coast"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_GRASS,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_APPLIN, 100},
            {SPECIES_SPEAROW, 100},
        }
    },

    [TEMPLATES_SMELLY_SWAMP] =
    {
        .name = COMPOUND_STRING("Smelly Swamp"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_FOREST,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_APPLIN, 100},
            {SPECIES_SPEAROW, 100},
        }
    },

    [TEMPLATES_TULIP_TOWN] =
    {
        .name = COMPOUND_STRING("Tulip Town"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_CITY,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {}
    },

    [TEMPLATES_POSITIVE_PLAINS] =
    {
        .name = COMPOUND_STRING("Positive Plains"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_GRASS,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_APPLIN, 100},
            {SPECIES_SPEAROW, 100},
        }
    },

    [TEMPLATES_FROSTY_FLATS] =
    {
        .name = COMPOUND_STRING("Frosty Flats"),
        .mapGroup = MAP_GROUP(FOREST_TEMPLATES_CONNECTIONS),
        .bgm = MUS_RG_VIRIDIAN_FOREST,
        .background = BG_GRASS,
        .connectionType = CONNECTION_TYPE_SEAMLESS,
        .offsets = {
            [DIR_NORTH] = {-2, -3, 6, 3},
            [DIR_SOUTH] = {-2, 1, 6, 3},
            [DIR_EAST] = {1, -2, 3, 4},
            [DIR_WEST] = {-3, -1, 3, 4},
        },
        .numNormalRooms = ARRAY_COUNT(sForestNormalRooms),
        .normalRoomIds = sForestNormalRooms,
        .specialRoomIds = {
            [BOSS_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [TREASURE_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
            [SHOP_ROOM] = MAP_NUM(FOREST_TEMPLATES_BOSS_ROOM),
        },
        .itemPools = gDefaultItemPools,
        .encounterPool = {
            {SPECIES_APPLIN, 100},
            {SPECIES_SPEAROW, 100},
        }
    },
};
