#include "global.h"
#include "battle.h"
#include "bg.h"
#include "title_screen.h"
#include "sprite.h"
#include "gba/m4a_internal.h"
#include "clear_save_data_menu.h"
#include "decompress.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "global.fieldmap.h"
#include "intro.h"
#include "item.h"
#include "list_menu.h"
#include "load_save.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "map_gen.h"
#include "map_preview.h"
#include "menu.h"
#include "naming_screen.h"
#include "overworld.h"
#include "palette.h"
#include "play_time.h"
#include "pokemon.h"
#include "reset_rtc_screen.h"
#include "berry_fix_program.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "start_screen.h"
#include "string.h"
#include "string_util.h"
#include "international_string_util.h"
#include "task.h"
#include "text.h"
#include "trail_interface.h"
#include "gpu_regs.h"
#include "trig.h"
#include "graphics.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/flags.h"

// sprite tags
enum
{
    TAG_NORTH_ARROW = 5000,
    TAG_SOUTH_ARROW,
    TAG_EAST_ARROW,
    TAG_WEST_ARROW,
    TAG_PLAYER_SHADOW,
};

// const rom data
static const struct BgTemplate sTrailMapBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    { // Logo
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    { // Text
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    { // Environment
        .bg = 3,
        .charBaseIndex = 1,
        .mapBaseIndex = 21,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

enum
{
    WIN_TIME,
    WIN_MESSAGE,
    WIN_YESNO,
    WIN_LOCATION,
    WINDOW_COUNT,
};

static const struct WindowTemplate sTrailInterfaceWinTemplates[WINDOW_COUNT + 1] =
{
    [WIN_TIME] =
    {
        .bg = 1,
        .tilemapLeft = 16,
        .tilemapTop = 0,
        .width = 14,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [WIN_MESSAGE] =
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1 + 14*2,
    },
    [WIN_YESNO] =
    {
        .bg = 1,
        .tilemapLeft = 24,
        .tilemapTop = 9,
        .width = 5,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1 + 14*2 + 28*4,
    },
    [WIN_LOCATION] =
    {
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 14,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1 + 14*2 + 28*4 + 5*4,
    },
    DUMMY_WIN_TEMPLATE
};

// graphics data
static const u32 sTrailMapGfx[] = INCBIN_U32("graphics/trail_map/map_tiles.4bpp.lz");
static const u32 sTrailMapTilemap[] = INCBIN_U32("graphics/trail_map/map_tiles.bin.lz");
static const u16 sTrailMapPal[] = INCBIN_U16("graphics/trail_map/map_tiles.gbapal");

static const u8 sNorthArrowGfx[] = INCBIN_U8("graphics/trail_map/north_arrow.4bpp");
static const u8 sSouthArrowGfx[] = INCBIN_U8("graphics/trail_map/south_arrow.4bpp");
static const u8 sEastArrowGfx[] = INCBIN_U8("graphics/trail_map/east_arrow.4bpp");
static const u8 sWestArrowGfx[] = INCBIN_U8("graphics/trail_map/west_arrow.4bpp");
static const u8 sShadowGfx[] = INCBIN_U8("graphics/trail_map/shadow.4bpp");
static const u16 sInterfaceGraphicsPal[] = INCBIN_U16("graphics/trail_map/shadow.gbapal");

static const struct SpriteSheet sArrowSpriteSheets[5] = 
{
    {},
    {sSouthArrowGfx, sizeof(sSouthArrowGfx), TAG_SOUTH_ARROW},
    {sNorthArrowGfx, sizeof(sNorthArrowGfx), TAG_NORTH_ARROW},
    {sWestArrowGfx, sizeof(sWestArrowGfx), TAG_WEST_ARROW},
    {sEastArrowGfx, sizeof(sEastArrowGfx), TAG_EAST_ARROW},
};

static const struct SpriteSheet sShadowSpriteSheet = {sShadowGfx, sizeof(sShadowGfx), TAG_PLAYER_SHADOW};

static const struct SpritePalette sInterfaceGraphicsSpritePalette = {sInterfaceGraphicsPal, TAG_NORTH_ARROW};

static const struct OamData sOAM_8x8 =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 1,
};

static const struct OamData sOAM_16x16 =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_BLEND,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 1,
};

static void SpriteCB_Arrow(struct Sprite *sprite);
static const struct SpriteTemplate sArrowSpriteTemplates[5] =
{
    {},
    {
        .tileTag = TAG_SOUTH_ARROW,
        .paletteTag = TAG_NORTH_ARROW,
        .oam = &sOAM_8x8,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_Arrow,
    },
    {
        .tileTag = TAG_NORTH_ARROW,
        .paletteTag = TAG_NORTH_ARROW,
        .oam = &sOAM_8x8,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_Arrow,
    },
    {
        .tileTag = TAG_WEST_ARROW,
        .paletteTag = TAG_NORTH_ARROW,
        .oam = &sOAM_8x8,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_Arrow,
    },
    {
        .tileTag = TAG_EAST_ARROW,
        .paletteTag = TAG_NORTH_ARROW,
        .oam = &sOAM_8x8,
        .anims = gDummySpriteAnimTable,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_Arrow,
    },
};

static const struct SpriteTemplate sShadowSpriteTemplate =
{
    .tileTag = TAG_PLAYER_SHADOW,
    .paletteTag = TAG_NORTH_ARROW,
    .oam = &sOAM_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// ewram data
EWRAM_DATA static u32 *sTrailMapTilemapPtr = NULL;
EWRAM_DATA struct TrailInterface gTrailInterface = {};

// const data
const u8 gTrailMapCollisionData[TRAIL_MAP_HEIGHT][TRAIL_MAP_WIDTH] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 1, 1, 1, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const u8 gTrailMapCheckpointData[TRAIL_MAP_HEIGHT][TRAIL_MAP_WIDTH] = // TODO: Something readable
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, CHECKPOINT_PEONY_TOWN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, CHECKPOINT_FUNKY_FOREST_NORTH, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, CHECKPOINT_FUNKY_FOREST_WEST, 1, 1, 1, CHECKPOINT_FUNKY_FOREST_EAST, 1, 1, 1, CHECKPOINT_SMELLY_SWAMP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, CHECKPOINT_DEEP_DESERT_WEST, 1, 1, 1, CHECKPOINT_DEEP_DESERT_EAST, 1, 1, 1, 1, 1, 1, CHECKPOINT_COOL_COAST, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, CHECKPOINT_TERRIBLE_TUNNEL_NORTH, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, CHECKPOINT_MOLTEN_MOUNTAIN, 1, 1, 1, 1, 1, CHECKPOINT_TULIP_TOWN, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, CHECKPOINT_ORCHID_CITY, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, CHECKPOINT_TERRIBLE_TUNNEL_SOUTH, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const u8 gTrailMapTemplateData[TRAIL_MAP_HEIGHT][TRAIL_MAP_WIDTH] = // TODO: Something readable; incomplete
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, TEMPLATES_PEONY_TOWN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, TEMPLATES_FRIENDLY_FIELDS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, TEMPLATES_FRIENDLY_FIELDS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, TEMPLATES_FUNKY_FOREST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, TEMPLATES_FUNKY_FOREST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, TEMPLATES_FUNKY_FOREST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, TEMPLATES_FUNKY_FOREST, TEMPLATES_FUNKY_FOREST, TEMPLATES_FUNKY_FOREST, TEMPLATES_FUNKY_FOREST, TEMPLATES_FUNKY_FOREST, TEMPLATES_FUNKY_FOREST, TEMPLATES_FRIENDLY_FIELDS, TEMPLATES_FRIENDLY_FIELDS, TEMPLATES_SMELLY_SWAMP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, TEMPLATES_FRIENDLY_FIELDS, 0, 0, 0, 0, 0, 0, 0, TEMPLATES_POSITIVE_PLAINS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, TEMPLATES_FRIENDLY_FIELDS, 0, 0, 0, 0, 0, 0, 0, TEMPLATES_POSITIVE_PLAINS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, TEMPLATES_DEEP_DESERT, 0, 0, 0, 0, 0, 0, 0, TEMPLATES_POSITIVE_PLAINS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, TEMPLATES_DEEP_DESERT, TEMPLATES_DEEP_DESERT, TEMPLATES_DEEP_DESERT, TEMPLATES_DEEP_DESERT, TEMPLATES_DEEP_DESERT, TEMPLATES_POSITIVE_PLAINS, TEMPLATES_POSITIVE_PLAINS, TEMPLATES_POSITIVE_PLAINS, TEMPLATES_POSITIVE_PLAINS, TEMPLATES_POSITIVE_PLAINS, TEMPLATES_COOL_COAST, TEMPLATES_COOL_COAST, TEMPLATES_COOL_COAST, TEMPLATES_COOL_COAST, TEMPLATES_COOL_COAST, TEMPLATES_COOL_COAST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, TEMPLATES_DEEP_DESERT, 0, 0, 0, 0, 0, TEMPLATES_POSITIVE_PLAINS, 0, 0, 0, 0, 0, 0, TEMPLATES_COOL_COAST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, TEMPLATES_SWEATY_SUMMIT, 0, 0, 0, 0, 0, TEMPLATES_POSITIVE_PLAINS, 0, 0, 0, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, TEMPLATES_SWEATY_SUMMIT, TEMPLATES_SWEATY_SUMMIT, TEMPLATES_DEEP_DESERT, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_TULIP_TOWN, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_TERRIBLE_TUNNEL, 0, 0, 0, 0, 0, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_FROSTY_FLATS, TEMPLATES_ORCHID_CITY, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, TEMPLATES_TERRIBLE_TUNNEL, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const struct CheckpointData gCheckpointData[CHECKPOINT_COUNT] =
{
    [CHECKPOINT_FUNKY_FOREST_NORTH] =
    {
        .name = COMPOUND_STRING("Funky Forest"),
        .mapNum = MAP_NUM(FUNKY_FOREST_NORTH1),
        .warpId = {
            [DIR_NORTH] = 2,
            [DIR_SOUTH] = 0,
        }
    },
    [CHECKPOINT_FUNKY_FOREST_WEST] =
    {
        .name = COMPOUND_STRING("Funky Forest"),
        .mapNum = MAP_NUM(FUNKY_FOREST_WEST1),
        .warpId = {
            [DIR_WEST] = 0,
            [DIR_NORTH] = 2,
        }
    },
};

// forward declarations
static void MainCB2_TrailMap(void);
static void VBlankCB2_TrailMap(void);
static void Task_OpenTrailMap(u8 taskId);
static void Task_TrailMapWaitForKeypress(u8 taskId);
static void Task_SaveAndExit(u8 taskId);
static void Task_GoToOverworldCamp(u8 taskId);
static void Task_GoToCheckpoint(u8 taskId);
static void LoadMapGraphics(u32 characterId);
static void IncrementTime(u32 minutes);
static void PrintTime(void);
static void PrintLocation(void);
static void PrintTextToMessageBox(const u8 *str);
static u32 CreateYesNoBox(void);
static void ClearWindow(u32 windowId);
static bool32 CheckCollisionInDirection(u32 dir);
static bool32 CheckCheckpointTrigger(void);
static bool32 TryMoveInDirection(u32 dir);

// UI functions
static void MainCB2_TrailMap(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB2_TrailMap(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void GoToTrailMap(void) // for callnative testing
{
    FadeScreen(FADE_TO_BLACK, 2);
    SetMainCallback2(CB2_InitTrailInterface);
}

void CB2_InitTrailInterface(void)
{
    switch (gMain.state) {
        default:
        case 0:
            UpdatePaletteFade();
            if (!gPaletteFade.active)
                gMain.state++;
            break;
        case 1:
            SetVBlankCallback(NULL); 
            ClearVramOamPlttRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            gMain.state++;
            break;
        case 2:
            ClearTasksAndGraphicalStructs();
            gMain.state++;
            break;
        case 3:
            sTrailMapTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sTrailMapBgTemplates, ARRAY_COUNT(sTrailMapBgTemplates));
            SetBgTilemapBuffer(3, sTrailMapTilemapPtr);
            gMain.state++;
            break;
        case 4:
            DecompressAndCopyTileDataToVram(3, sTrailMapGfx, 0, 0, 0);
            LZDecompressWram(sTrailMapTilemap, sTrailMapTilemapPtr);
            LoadPalette(sTrailMapPal, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
            Menu_LoadStdPalAt(BG_PLTT_ID(15));
            LoadMessageBoxAndBorderGfx();
            gMain.state++;
            break;
        case 5:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                HideBg(0);
                ShowBg(1);
                ShowBg(2);
                ShowBg(3);
                CopyBgTilemapBufferToVram(3);
                gMain.state++;
            }
            break;
        case 6:
            InitWindows(sTrailInterfaceWinTemplates);
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 7:
            LoadSpritePalette(&gMiscGfxSpritePalette);
            LoadSpriteSheet(&gShadowSpriteSheet); // TODO: Shadow sprite
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 6));
            LoadMapGraphics(gSaveBlock1Ptr->characterId);
            gMain.state++;
            break;
        case 8:
            ResetMapMusic();
            PlayBGM(MUS_ROUTE119);
            ClearContinueGameWarpStatus();
            FadeScreen(FADE_FROM_BLACK, 2);
            SetVBlankCallback(VBlankCB2_TrailMap);
            CreateTask(Task_OpenTrailMap, 0);
            SetMainCallback2(MainCB2_TrailMap);
            break;
    }
}

static void Task_OpenTrailMap(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_TrailMapWaitForKeypress;
    }
}

static void Task_TrailMapWaitForKeypress(u8 taskId)
{
    if (JOY_HELD(DPAD_ANY))
    {
        gTrailInterface.keyHeldTimer += 1;
    }
    else
    {
        gTrailInterface.keyHeldTimer = 0;
    }

    if (JOY_NEW(DPAD_UP) || (JOY_HELD(DPAD_UP) && gTrailInterface.keyHeldTimer % 12 == 0))
    {
        if (TryMoveInDirection(DIR_NORTH))
            IncrementTime(60);
    }
    if (JOY_NEW(DPAD_DOWN) || (JOY_HELD(DPAD_DOWN) && gTrailInterface.keyHeldTimer % 12 == 0))
    {
        if (TryMoveInDirection(DIR_SOUTH))
            IncrementTime(60);
    }
    if (JOY_NEW(DPAD_RIGHT) || (JOY_HELD(DPAD_RIGHT) && gTrailInterface.keyHeldTimer % 12 == 0))
    {
        if (TryMoveInDirection(DIR_EAST))
            IncrementTime(60);
    }
    if (JOY_NEW(DPAD_LEFT) || (JOY_HELD(DPAD_LEFT) && gTrailInterface.keyHeldTimer % 12 == 0))
    {
        if (TryMoveInDirection(DIR_WEST))
            IncrementTime(60);
    }
    if ((JOY_NEW(DPAD_ANY) || JOY_HELD(DPAD_ANY)) && CheckCheckpointTrigger())
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_GoToCheckpoint;
    }

    if (JOY_NEW(START_BUTTON))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_SaveAndExit;
    }
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_GoToOverworldCamp;
    }
}

// Trigger save sequence from trail map.
static void Task_SaveAndExit(u8 taskId)
{
    switch (gTasks[taskId].data[0])
    {
        case 0: // Print message and yes no box.
            PrintTextToMessageBox(COMPOUND_STRING("Save and exit?"));
            gTasks[taskId].data[2] = CreateYesNoBox();
            ++gTasks[taskId].data[0];
            break;
        case 1: // Process menu input.
        {
            u32 input = ListMenu_ProcessInput(gTasks[taskId].data[2]);
            if (gMain.newKeys & A_BUTTON)
            {
                PlaySE(SE_SELECT);
                DestroyTask(gTasks[taskId].data[2]);
                if (input == 0) gTasks[taskId].data[0] += 1;
                else gTasks[taskId].data[0] = 5;
            }
            else if (gMain.newKeys & B_BUTTON)
            {
                PlaySE(SE_SELECT);
                DestroyTask(gTasks[taskId].data[2]);
                gTasks[taskId].data[0] = 5;
            }
            break;
        }
        case 2: // Do save.
            TrySavingData(SAVE_NORMAL);
            ++gTasks[taskId].data[0];
        case 3: // Print confirmation.
            ClearWindow(WIN_YESNO);
            PlaySE(SE_SAVE);
            PrintTextToMessageBox(COMPOUND_STRING("Save complete!\nPress A to exit."));
            ++gTasks[taskId].data[0];
            break;
        case 4: // Exit to title.
            if (JOY_NEW(A_BUTTON))
                DoSoftReset();
            break;
        case 5: // Return to trail map.
            ClearWindow(WIN_MESSAGE);
            ClearWindow(WIN_YESNO);
            gTasks[taskId].func = Task_TrailMapWaitForKeypress;
            gTasks[taskId].data[0] = 0;
            break;
    }
}

static enum TemplateTypes GetTemplateTypeFromTrailPos(void)
{
    return gTrailMapTemplateData[gSaveBlock1Ptr->trailY / 8][gSaveBlock1Ptr->trailX / 8];
}

// Trigger map generation and warp to overworld.
static void Task_GoToOverworldCamp(u8 taskId)
{
    switch (gTasks[taskId].data[0])
    {
        case 0: // Print message and yes no box.
            PrintTextToMessageBox(COMPOUND_STRING("Stop to camp?"));
            gTasks[taskId].data[2] = CreateYesNoBox();
            ++gTasks[taskId].data[0];
            break;
        case 1: // Process menu input.
        {
            u32 input = ListMenu_ProcessInput(gTasks[taskId].data[2]);
            if (gMain.newKeys & A_BUTTON)
            {
                PlaySE(SE_SELECT);
                DestroyTask(gTasks[taskId].data[2]);
                if (input == 0) gTasks[taskId].data[0] += 1;
                else gTasks[taskId].data[0] = 5;
            }
            else if (gMain.newKeys & B_BUTTON)
            {
                PlaySE(SE_SELECT);
                DestroyTask(gTasks[taskId].data[2]);
                gTasks[taskId].data[0] = 5;
            }
            break;
        }
        case 2: // Do map generation.
            // Update save fields.
            ++gSaveBlock1Ptr->currentFloor;
            gSaveBlock1Ptr->floorSeed = gFloorplan.nextFloorSeed;
            memset(gSaveBlock1Ptr->visitedRooms, 0, sizeof(gSaveBlock1Ptr->visitedRooms));

            // Generate the new floorplan and warp.
            GenerateFloorplan();
            ClearFloorEventFlags();
            SetContinueGameWarpStatus();
            SetWarpData(&gSaveBlock1Ptr->continueGameWarp, GetCurrentTemplateRules()->mapGroup,
                        gFloorplan.layout[STARTING_ROOM].mapNum, 0, -1, -1);
            gSaveBlock1Ptr->currentRoom = STARTING_ROOM;

            // Autosave.
            TrySavingData(SAVE_LINK);
            ++gTasks[taskId].data[0];
            break;
        case 3: // Confirm save and begin warp.
            PlaySE(SE_SAVE);
            FadeScreen(FADE_TO_BLACK, 0);
            ++gTasks[taskId].data[0];
            break;
        case 4: // Warp.
            if (!gPaletteFade.active)
            {
                TryWarpToRoom(STARTING_ROOM, 0xFF);
                Free(sTrailMapTilemapPtr);
                sTrailMapTilemapPtr = NULL;
                FreeAllWindowBuffers();
                ResetSpriteData();
                UnlockPlayerFieldControls();
                UnfreezeObjectEvents();
                DestroyTask(taskId);
            }
            break;
        case 5: // Return to trail map.
            ClearWindow(WIN_MESSAGE);
            ClearWindow(WIN_YESNO);
            gTasks[taskId].func = Task_TrailMapWaitForKeypress;
            gTasks[taskId].data[0] = 0;
            break;
    }
}

// Trigger warp to checkpoint.
static void Task_GoToCheckpoint(u8 taskId)
{
    enum Checkpoint checkpoint = gTrailMapCheckpointData[gSaveBlock1Ptr->trailY/8][gSaveBlock1Ptr->trailX/8];

    switch (gTasks[taskId].data[0])
    {
        case 0: // Route to correct step.
            if (gCheckpointData[checkpoint].mapNum == 0) // Safety check
            {
                gTasks[taskId].func = Task_TrailMapWaitForKeypress;
                return;
            }
            else if (gSaveBlock1Ptr->checkpoints & (1 << checkpoint)) // Yes no
            {
                ++gTasks[taskId].data[0];
            }
            else // No choice
            {
                gTasks[taskId].data[0] = 6;
            }
            break;
        case 1: // Print message and yes no box.
            StringCopy(gStringVar1, COMPOUND_STRING("Stop at "));
            StringAppend(gStringVar1, gCheckpointData[checkpoint].name);
            StringAppend(gStringVar1, COMPOUND_STRING("?"));
            PrintTextToMessageBox(gStringVar1);
            gTasks[taskId].data[2] = CreateYesNoBox();
            ++gTasks[taskId].data[0];
            break;
        case 2: // Process menu input.
        {
            u32 input = ListMenu_ProcessInput(gTasks[taskId].data[2]);
            if (gMain.newKeys & A_BUTTON)
            {
                PlaySE(SE_SELECT);
                DestroyTask(gTasks[taskId].data[2]);
                if (input == 0) gTasks[taskId].data[0] += 1;
                else gTasks[taskId].data[0] = 8;
            }
            else if (gMain.newKeys & B_BUTTON)
            {
                PlaySE(SE_SELECT);
                DestroyTask(gTasks[taskId].data[2]);
                gTasks[taskId].data[0] = 8;
            }
            break;
        }
        case 3: // Autosave.
            SetContinueGameWarpStatus();
            SetWarpData(&gSaveBlock1Ptr->continueGameWarp, MAP_GROUP(INTRO_SEQUENCE), gCheckpointData[checkpoint].mapNum, gCheckpointData[checkpoint].warpId[gSaveBlock1Ptr->facing], 0, 0);
            TrySavingData(SAVE_LINK);
            ++gTasks[taskId].data[0];
            break;
        case 4: // Confirm save and begin warp.
            PlaySE(SE_SAVE);
            FadeScreen(FADE_TO_BLACK, 0);
            ++gTasks[taskId].data[0];
            break;
        case 5: // Go to map.
            if (!gPaletteFade.active)
            {
                StoreInitialPlayerAvatarState();
                LockPlayerFieldControls();
                PlayBGM(GetCurrentTemplateRules()->bgm);
                WarpFadeOutScreen();
                PlayRainStoppingSoundEffect();
                SetWarpDestination(MAP_GROUP(INTRO_SEQUENCE), gCheckpointData[checkpoint].mapNum, gCheckpointData[checkpoint].warpId[gSaveBlock1Ptr->facing], 0, 0);
                WarpIntoMap();
                SetMainCallback2(CB2_LoadMap);

                // Clean up data.
                Free(sTrailMapTilemapPtr);
                sTrailMapTilemapPtr = NULL;
                FreeAllWindowBuffers();
                ResetSpriteData();
                UnlockPlayerFieldControls();
                UnfreezeObjectEvents();
                DestroyTask(taskId);
            }
            break;
        case 6: // Print message with no yes no box.
            StringCopy(gStringVar1, COMPOUND_STRING("Stopping at "));
            StringAppend(gStringVar1, gCheckpointData[checkpoint].name);
            StringAppend(gStringVar1, COMPOUND_STRING("…"));
            PrintTextToMessageBox(gStringVar1);
            ++gTasks[taskId].data[0];
            break;
        case 7: // Wait for input.
            if (JOY_NEW(A_BUTTON))
                gTasks[taskId].data[0] = 3;
            break;
        case 8: // Return to trail map.
            ClearWindow(WIN_MESSAGE);
            ClearWindow(WIN_YESNO);
            gTasks[taskId].func = Task_TrailMapWaitForKeypress;
            gTasks[taskId].data[0] = 0;
            break;
    }
}

static void SpriteCB_PlayerSprite(struct Sprite *sprite)
{
    sprite->x = gSaveBlock1Ptr->trailX;
    sprite->y = gSaveBlock1Ptr->trailY;
}

static const u8 sArrowSpriteOffsets[5][2] =
{
    [DIR_NORTH] =   {12, 3},
    [DIR_SOUTH] =   {12, 29},
    [DIR_EAST] =    {24, 16},
    [DIR_WEST] =    {0,  16},
};

static void SpriteCB_Arrow(struct Sprite *sprite)
{
    u8 dir = sprite->data[0];
    sprite->invisible = CheckCollisionInDirection(sprite->data[0]); // TODO: trigger only after movement
    sprite->x = gSaveBlock1Ptr->trailX + sArrowSpriteOffsets[dir][0];
    sprite->y = gSaveBlock1Ptr->trailY + sArrowSpriteOffsets[dir][1];
}

static void LoadMapGraphics(u32 characterId)
{
    // Draw player sprite.
    gTrailInterface.playerSpriteId = CreateObjectGraphicsSprite(gCharacterInfos[characterId].graphicsId, SpriteCallbackDummy, 28, 18, 0);
    SetAndStartSpriteAnim(&gSprites[gTrailInterface.playerSpriteId], 3 + gSaveBlock1Ptr->facing, 0);
    gSprites[gTrailInterface.playerSpriteId].oam.priority = 1;
    gSprites[gTrailInterface.playerSpriteId].callback = SpriteCB_PlayerSprite;
    gSprites[gTrailInterface.playerSpriteId].x2 = 12;
    gSprites[gTrailInterface.playerSpriteId].y2 = 9;

    // Draw arrow sprites.
    LoadSpritePalette(&sInterfaceGraphicsSpritePalette);
    for (u32 dir = DIR_SOUTH; dir <= DIR_EAST; ++dir)
    {
        LoadSpriteSheet(&sArrowSpriteSheets[dir]);
        gTrailInterface.arrowSpriteIds[dir] = CreateSprite(&sArrowSpriteTemplates[dir], 16+dir*16, 16, 0);
        gSprites[gTrailInterface.arrowSpriteIds[dir]].data[0] = dir;
    }

    // Draw shadow.
    // LoadSpriteSheet(&sShadowSpriteSheet);
    // u32 spriteId = CreateSprite(&sShadowSpriteTemplate, 0, 0, 16);
    // gSprites[spriteId].callback = SpriteCB_PlayerSprite;
    // gSprites[spriteId].x2 = 12;
    // gSprites[spriteId].y2 = 17;
    // gSprites[gTrailInterface.playerSpriteId].data[0] = spriteId;

    // Update palette blend.
    UpdateTimeOfDay();
    u32 palettes = PALETTES_ALL & ~((1 << 15) | (1 << 14));
    TimeMixPalettes(palettes, gPlttBufferUnfaded, gPlttBufferFaded, &gTimeBlend.startBlend, &gTimeBlend.endBlend, gTimeBlend.weight);

    // Print time.
    PrintTime();

    // Print location.
    PrintLocation();
}

void IncrementTrailTime(u32 minutes)
{
    gSaveBlock1Ptr->minute += minutes;
    while (gSaveBlock1Ptr->minute >= 60)
    {
        gSaveBlock1Ptr->minute -= 60;
        gSaveBlock1Ptr->hour += 1;
        if (gSaveBlock1Ptr->hour >= 13)
        {
            gSaveBlock1Ptr->hour -= 12;
            gSaveBlock1Ptr->halfDay += 1;
        }
    }

    while (gSaveBlock1Ptr->halfDay >= 2) // AM/PM
    {
        gSaveBlock1Ptr->halfDay -= 2;
        gSaveBlock1Ptr->day += 1;
    }    
}

// Increment time by set amount of hours and update text.
static void IncrementTime(u32 minutes)
{
    // Update trail time.
    IncrementTrailTime(minutes);

    // Update UI.
    PrintTime();

    // Update palette blend.
    UpdateTimeOfDay();
    u32 palettes = PALETTES_ALL & ~(1 << 15);
    TimeMixPalettes(palettes, gPlttBufferUnfaded, gPlttBufferFaded, &gTimeBlend.startBlend, &gTimeBlend.endBlend, gTimeBlend.weight);
}

// Print time to top right.
static void PrintTime(void)
{
	const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, 1, 8};
	StringCopy(gStringVar1, COMPOUND_STRING("DAY "));
	ConvertIntToDecimalStringN(gStringVar2, gSaveBlock1Ptr->day, STR_CONV_MODE_LEFT_ALIGN, 3);
	StringAppend(gStringVar1, gStringVar2);

    StringCopy(gStringVar2, COMPOUND_STRING(", "));
	ConvertIntToDecimalStringN(gStringVar3, gSaveBlock1Ptr->hour, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar2, gStringVar3);
    StringAppend(gStringVar1, gStringVar2);

    StringCopy(gStringVar2, COMPOUND_STRING(":"));
	ConvertIntToDecimalStringN(gStringVar3, gSaveBlock1Ptr->minute, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar2, gStringVar3);
    StringAppend(gStringVar1, gStringVar2);

    if ((gSaveBlock1Ptr->halfDay == 0) != (gSaveBlock1Ptr->hour != 12)) // this is an XOR
        StringCopy(gStringVar2, COMPOUND_STRING(" PM"));
    else
        StringCopy(gStringVar2, COMPOUND_STRING(" AM"));
	StringAppend(gStringVar1, gStringVar2);

    u32 offset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar1, 107);
    FillWindowPixelBuffer(WIN_TIME, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_TIME, FONT_NORMAL, 0 + offset, 0, textColor, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WIN_TIME, COPYWIN_FULL);
    PutWindowTilemap(WIN_TIME);
}

// Print location to top left.
static void PrintLocation(void)
{
	const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, 1, 8};
    FillWindowPixelBuffer(WIN_LOCATION, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_LOCATION, FONT_NORMAL, 7, 0, textColor, TEXT_SKIP_DRAW, GetCurrentTemplateRules()->name);
    CopyWindowToVram(WIN_LOCATION, COPYWIN_FULL);
    PutWindowTilemap(WIN_LOCATION);
}

// Draw message box and print text.
static void PrintTextToMessageBox(const u8 *str)
{
	const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, 1, 8};
    FillWindowPixelBuffer(WIN_MESSAGE, PIXEL_FILL(0));
    DrawStdWindowFrame(WIN_MESSAGE, FALSE);

    AddTextPrinterParameterized3(WIN_MESSAGE, FONT_NORMAL, 6, 0, textColor, TEXT_SKIP_DRAW, str);
    CopyWindowToVram(WIN_MESSAGE, COPYWIN_FULL);
    CopyBgTilemapBufferToVram(1);
}

static const struct ListMenuItem sYesNoMenuItems[] = 
{
    { COMPOUND_STRING("YES"),       0 },
    { COMPOUND_STRING("NO"),        1 },
};

// Create yes no box and return input task ID.
static u32 CreateYesNoBox(void)
{
    struct ListMenuTemplate menuTemplate = {0};
    LoadMessageBoxAndBorderGfx();
    DrawStdWindowFrame(WIN_YESNO, FALSE);

    menuTemplate.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
    menuTemplate.items = sYesNoMenuItems;
    menuTemplate.totalItems = 2;
    menuTemplate.maxShowed = 2;
    menuTemplate.windowId = WIN_YESNO;
    menuTemplate.item_X = 8;
    menuTemplate.upText_Y = 1;
    menuTemplate.cursorPal = 1;
    menuTemplate.fillValue = 15;
    menuTemplate.cursorShadowPal = 15;
    menuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    menuTemplate.fontId = FONT_NORMAL;
    u32 taskId = ListMenuInit(&menuTemplate, 0, 0);
    CopyWindowToVram(WIN_YESNO, COPYWIN_FULL);
    CopyBgTilemapBufferToVram(1);

    return taskId;
}

// Clear window and border.
static void ClearWindow(u32 windowId)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    CopyWindowToVram(windowId, COPYWIN_FULL);
    if (windowId > WIN_TIME)
        ClearStdWindowAndFrame(windowId, FALSE);
}

// Movement functions
static bool32 CheckCollisionInDirection(u32 dir)
{
    u8 x = gSaveBlock1Ptr->trailX / 8;
    u8 y = gSaveBlock1Ptr->trailY / 8;

    if (gSaveBlock1Ptr->trailX % 8 == 0)
    {
        if (dir == DIR_EAST && gTrailMapCollisionData[y][x+1] == 0)
            return TRUE;
        if (dir == DIR_WEST && gTrailMapCollisionData[y][x-1] == 0)
            return TRUE;
    }
    else
    {
        if (dir == DIR_NORTH)
            return TRUE;
        if (dir == DIR_SOUTH)
            return TRUE;
    }

    if (gSaveBlock1Ptr->trailY % 8 == 0)
    {
        if (dir == DIR_NORTH && gTrailMapCollisionData[y-1][x] == 0)
            return TRUE;
        if (dir == DIR_SOUTH && gTrailMapCollisionData[y+1][x] == 0)
            return TRUE;
    }
    else
    {
        if (dir == DIR_EAST)
            return TRUE;
        if (dir == DIR_WEST)
            return TRUE;
    }

    return FALSE;
}

static bool32 CheckCheckpointTrigger(void)
{    
    u8 x = gSaveBlock1Ptr->trailX / 8;
    u8 y = gSaveBlock1Ptr->trailY / 8;

    if ((gSaveBlock1Ptr->trailX % 8 == 0)
        && (gSaveBlock1Ptr->trailY % 8 == 0)
        && (gTrailMapCollisionData[y][x] == 2))
    {
        return TRUE;
    }
    return FALSE;
}

static bool32 TryMoveInDirection(u32 dir)
{
    // Check collision.
    if (CheckCollisionInDirection(dir))
        return FALSE;

    // Update facing direction.
    if (gSaveBlock1Ptr->facing != dir)
    {
        SetAndStartSpriteAnim(&gSprites[gTrailInterface.playerSpriteId], dir + 3, 0); // anim constant jank
        gSaveBlock1Ptr->facing = dir;
    }

    // Update position.
    switch (dir)
    {
        case DIR_NORTH:
            gSaveBlock1Ptr->trailY -= 1;
            break;
        case DIR_SOUTH:
            gSaveBlock1Ptr->trailY += 1;
            break;
        case DIR_EAST:
            gSaveBlock1Ptr->trailX += 1;
            break;
        case DIR_WEST:
            gSaveBlock1Ptr->trailX -= 1;    
            break;
    }

    // Update template type.
    u8 templateType = gSaveBlock1Ptr->currentTemplateType;
    gSaveBlock1Ptr->currentTemplateType = GetTemplateTypeFromTrailPos();
    if (templateType != gSaveBlock1Ptr->currentTemplateType)
        PrintLocation();

    return TRUE;
}
