#include "global.h"
#include "bg.h"
#include "sprite.h"
#include "main.h"
#include "battle.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "sound.h"
#include "m4a.h"
#include "map_gen.h"
#include "map_screen.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "random.h"
#include "palette.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sprite.h"
#include "string_util.h"
#include "trig.h"
#include "util.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// sprite tags
enum {
    TAG_BOSS_ROOM = 10000,
    TAG_TREASURE_ROOM,
    TAG_SHOP_ROOM,
    TAG_CHALLENGE_ROOM,
    TAG_CURRENT_ROOM,
    TAG_VISITED_ROOM,
    TAG_UNVISITED_ROOM,
    TAG_MINIMAP_BORDER,
};

#define TAG_ROOM_PAL 10000

enum Windows
{
	WIN_FLOOR,
	WINDOW_COUNT,
};

// credit to Sbird for dynamic BG code
#define POS_TO_SCR_ADDR(x,y) (32*(y) + (x))
#define SCR_MAP_ENTRY(tile, pal, hflip, vflip) ((tile) | (hflip ? (1<<10) : 0) | (vflip ? (1 << 11) : 0) | (pal << 12))

// const rom data
static const u32 sMapScreenBgGfx[]      = INCBIN_U32("graphics/interface/map_screen.4bpp.lz");
static const u16 sMapScreenBgPal[]      = INCBIN_U16("graphics/interface/map_screen.gbapal");
static const u32 sMapScreenBgMap[]      = INCBIN_U32("graphics/interface/map_screen.bin.lz");
// shared
static const u8 sBossRoomGfx[]          = INCBIN_U8("graphics/interface/boss_room.4bpp");
static const u8 sTreasureRoomGfx[]      = INCBIN_U8("graphics/interface/treasure_room.4bpp");
static const u8 sShopRoomGfx[]          = INCBIN_U8("graphics/interface/shop_room.4bpp");
static const u8 sChallengeRoomGfx[]     = INCBIN_U8("graphics/interface/challenge_room.4bpp");
static const u16 sRoomsPal[]            = INCBIN_U16("graphics/interface/minimap_border.gbapal");
// minimap
static const u8 sCurrentRoomGfx[]       = INCBIN_U8("graphics/interface/current_room.4bpp");
static const u8 sVisitedRoomGfx[]       = INCBIN_U8("graphics/interface/visited_room.4bpp");
static const u8 sUnvisitedRoomGfx[]     = INCBIN_U8("graphics/interface/unvisited_room.4bpp");
static const u8 sMinimapBorderGfx[]     = INCBIN_U8("graphics/interface/minimap_border.4bpp");

static const struct WindowTemplate sMapScreenWinTemplates[WINDOW_COUNT + 1] =
{
	[WIN_FLOOR] =
	{
		.bg = 1,
		.tilemapLeft = 0,
		.tilemapTop = 0,
		.width = 30,
		.height = 2,
		.paletteNum = 0,
		.baseBlock = 1,
	},
	DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sMapScreenBgTemplates[] =
{
	{ // Map Background
		.bg = 2,
		.charBaseIndex = 0,
		.mapBaseIndex = 31,
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
		.priority = 1,
		.baseTile = 0,
	},
	{ // Floor
		.bg = 0,
		.charBaseIndex = 1,
		.mapBaseIndex = 24,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 0,
	}
};

static const struct SpritePalette sRoomsSpritePalette = {sRoomsPal, TAG_ROOM_PAL};

static const struct OamData sSpecialRoomOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 0,
};

static const struct OamData sMiniRoomOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(8x8),
	.size = SPRITE_SIZE(8x8),
	.priority = 0,
};

// Boss Room sprite data
static void SpriteCB_Dummy(struct Sprite *sprite) {}
static const struct SpriteTemplate sBossRoomSpriteTemplate =
{
	.tileTag = TAG_BOSS_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sSpecialRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sBossRoomSpriteSheet = {sBossRoomGfx, sizeof(sBossRoomGfx), TAG_BOSS_ROOM};

// Treasure Room sprite data
static const struct SpriteTemplate sTreasureRoomSpriteTemplate =
{
	.tileTag = TAG_TREASURE_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sSpecialRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sTreasureRoomSpriteSheet = {sTreasureRoomGfx, sizeof(sTreasureRoomGfx), TAG_TREASURE_ROOM};

// Shop Room sprite data
static const struct SpriteTemplate sShopRoomSpriteTemplate =
{
	.tileTag = TAG_SHOP_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sSpecialRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sShopRoomSpriteSheet = {sShopRoomGfx, sizeof(sShopRoomGfx), TAG_SHOP_ROOM};

// Challenge Room sprite data
static const struct SpriteTemplate sChallengeRoomSpriteTemplate =
{
	.tileTag = TAG_CHALLENGE_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sSpecialRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sChallengeRoomSpriteSheet = {sChallengeRoomGfx, sizeof(sChallengeRoomGfx), TAG_CHALLENGE_ROOM};

// Current Room sprite data
static const struct SpriteTemplate sCurrentRoomSpriteTemplate =
{
	.tileTag = TAG_CURRENT_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sMiniRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sCurrentRoomSpriteSheet = {sCurrentRoomGfx, sizeof(sCurrentRoomGfx), TAG_CURRENT_ROOM};

// Visited Room sprite data
static const struct SpriteTemplate sVisitedRoomSpriteTemplate =
{
	.tileTag = TAG_VISITED_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sMiniRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sVisitedRoomSpriteSheet = 
{
    sVisitedRoomGfx, sizeof(sVisitedRoomGfx), TAG_VISITED_ROOM
};

// Unvisited Room sprite data
static const struct SpriteTemplate sUnvisitedRoomSpriteTemplate =
{
	.tileTag = TAG_UNVISITED_ROOM,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sMiniRoomOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sUnvisitedRoomSpriteSheet = {sUnvisitedRoomGfx, sizeof(sUnvisitedRoomGfx), TAG_UNVISITED_ROOM};

// Minimap Border sprite data
static const struct OamData sMinimapBorderOAM =
{
	.affineMode = ST_OAM_AFFINE_OFF,
	.objMode = ST_OAM_OBJ_NORMAL,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 0,
};

static const struct SpriteTemplate sMinimapBorderSpriteTemplate =
{
	.tileTag = TAG_MINIMAP_BORDER,
	.paletteTag = TAG_ROOM_PAL,
	.oam = &sMinimapBorderOAM,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Dummy,
};

static const struct SpriteSheet sMinimapBorderSpriteSheet = {sMinimapBorderGfx, sizeof(sMinimapBorderGfx), TAG_MINIMAP_BORDER};

// functions
static void PrintFloorText(void);
static void CleanWindows(void);
static void CommitWindows(void);
static void LoadMapScreenGfx(void);
static void Task_MapScreenFadeOutAndExit(u8 taskId);
static void Task_MapScreenWaitForKeypress(u8 taskId);
static void Task_MapScreenFadeIn(u8 taskId);
static void InitMapScreen(void);
static void ShowRooms(void);

EWRAM_DATA static u32 sRoomBuffer = 0;
EWRAM_DATA static u32* sMapScreenTilemapPtr = NULL;
EWRAM_DATA static u8 sMinimapSpriteIds[10] = {0}; // index 9 used for border

// code
static void MainCB2_MapScreen(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
	UpdatePaletteFade();
}

static void VBlankCB_MapScreen(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

void CB2_MapScreen(void)
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
            sMapScreenTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sMapScreenBgTemplates, 3);
            SetBgTilemapBuffer(2, sMapScreenTilemapPtr);
            gMain.state++;
            break;
        case 4:
        {
            LoadMapScreenGfx();
            u16 palette = RGB(0,0,0); // for cleaner fadeout
            LoadPalette(&palette, BG_PLTT_ID(0), PLTT_SIZEOF(1));
            LoadPalette(&palette, BG_PLTT_ID(1), PLTT_SIZEOF(1));
            gMain.state++;
            break;
        }
        case 5:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                ShowBg(0);
                ShowBg(1);
                ShowBg(2);
                CopyBgTilemapBufferToVram(2);
                gMain.state++;
            }
            break;
        case 6:
            InitWindows(sMapScreenWinTemplates);
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 7:
            BlendPalettes(PALETTES_ALL, 16, 0);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            gMain.state++;
            break;
        case 8:
            SetVBlankCallback(VBlankCB_MapScreen);
            InitMapScreen();
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x80);
            CreateTask(Task_MapScreenFadeIn, 0);
            SetMainCallback2(MainCB2_MapScreen);
            break;
	}
}

static void Task_MapScreenFadeOutAndExit(u8 taskId)
{
	if (!gPaletteFade.active)
	{
        SetMainCallback2(CB2_ReturnToField);
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
		Free(sMapScreenTilemapPtr);
        sMapScreenTilemapPtr = NULL;
		FreeAllWindowBuffers();
        ResetSpriteData();
        UnlockPlayerFieldControls();
        UnfreezeObjectEvents();
		DestroyTask(taskId);
	}
}

static void Task_MapScreenFadeOutAndWarp(u8 taskId)
{
    Free(sMapScreenTilemapPtr);
    sMapScreenTilemapPtr = NULL;
    FreeAllWindowBuffers();
    ResetSpriteData();
    TryWarpToRoom(gSaveBlock1Ptr->currentRoom, 0);
    DestroyTask(taskId);
}

static void Task_MapScreenWaitForKeypress(u8 taskId)
{
    // Check if player is selecting a new room.
    u32 target = gSaveBlock1Ptr->currentRoom;
    if (gMain.newKeys & DPAD_UP)
        target = GetRoomInDirection(DIR_NORTH);
    if (gMain.newKeys & DPAD_DOWN)
        target = GetRoomInDirection(DIR_SOUTH);
    if (gMain.newKeys & DPAD_RIGHT)
        target = GetRoomInDirection(DIR_EAST);
    if (gMain.newKeys & DPAD_LEFT)
        target = GetRoomInDirection(DIR_WEST);

    #if MAP_DEBUG == TRUE
    // In Debug mode, you can reveal the map by navigating in this screen.
    if (target != gSaveBlock1Ptr->currentRoom && DoesRoomExist(target))
    {
        PlaySE(SE_SELECT);
        SetRoomAsVisited(target);
        gSaveBlock1Ptr->currentRoom = target;
        ShowRooms();
    }
    #else
    if (target != gSaveBlock1Ptr->currentRoom && DoesRoomExist(target) && IsRoomVisited(target))
    {
        PlaySE(SE_SELECT);
        gSaveBlock1Ptr->currentRoom = target;
        ShowRooms();
    }
    #endif

    // Try to warp to new room.
    if (gMain.newKeys & A_BUTTON && gSaveBlock1Ptr->currentRoom != sRoomBuffer)
	{
		gTasks[taskId].func = Task_MapScreenFadeOutAndWarp;
    }

    // Exit map menu.
    if (gMain.newKeys & B_BUTTON)
	{
		PlaySE(SE_RG_CARD_FLIP);
        gSaveBlock1Ptr->currentRoom = sRoomBuffer;
		BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_MapScreenFadeOutAndExit;
    }
}

static void Task_MapScreenFadeIn(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		gTasks[taskId].func = Task_MapScreenWaitForKeypress;
	}
}

static void PrintFloorText(void)
{
    // Print floor number.
	const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, 9, 8};
	// StringCopy(gStringVar1, COMPOUND_STRING("FLOOR "));
	// ConvertIntToDecimalStringN(gStringVar2, gSaveBlock1Ptr->currentFloor, STR_CONV_MODE_LEFT_ALIGN, 3);
	// StringAppend(gStringVar1, gStringVar2);
    // u32 offset = GetStringRightAlignXOffset(FONT_NORMAL, gStringVar1, 80);
    // AddTextPrinterParameterized3(WIN_FLOOR, FONT_NORMAL, 152 + offset, 0, textColor, TEXT_SKIP_DRAW, gStringVar1);

    // Print template name.
	AddTextPrinterParameterized3(WIN_FLOOR, FONT_NORMAL, 4, 0, textColor, TEXT_SKIP_DRAW, GetCurrentTemplateRules()->name);
}

struct SpriteTable {
    const struct SpritePalette* spritePalette;
    const struct SpriteSheet* spriteSheet;
    const struct SpriteTemplate* spriteTemplate;
};

static const struct SpriteTable sRoomTypeSpriteTable[NUM_ROOM_TYPES] =
{
    [BOSS_ROOM] = {&sRoomsSpritePalette, &sBossRoomSpriteSheet, &sBossRoomSpriteTemplate},
    [TREASURE_ROOM] = {&sRoomsSpritePalette, &sTreasureRoomSpriteSheet, &sTreasureRoomSpriteTemplate},
    [SHOP_ROOM] = {&sRoomsSpritePalette, &sShopRoomSpriteSheet, &sShopRoomSpriteTemplate},
    [CHALLENGE_ROOM] = {&sRoomsSpritePalette, &sChallengeRoomSpriteSheet, &sChallengeRoomSpriteTemplate},
};

static void DrawRoomIcon(u32 x, u32 y)
{
    // Assumes room itself is drawn, but still do safety check on types.
    u32 type = gFloorplan.layout[ROOM_COORD(x, y)].type;
    if (type == NORMAL_ROOM || type >= NUM_ROOM_TYPES)
        return;

    LoadSpritePalette(sRoomTypeSpriteTable[type].spritePalette);
    LoadSpriteSheet(sRoomTypeSpriteTable[type].spriteSheet);
    CreateSprite(sRoomTypeSpriteTable[type].spriteTemplate, 8*(5 + x*2) + 9, 8*(2 + y*2) + 8, 0);
}

static void DrawRoomOnBg(u32 x, u32 y)
{
    u16 *tilemapPtr = GetBgTilemapBuffer(2);
    u32 tileId, bgX, bgY;

    if (ROOM_COORD(x, y) == gSaveBlock1Ptr->currentRoom)
        tileId = 0x09;
    else if (IsRoomVisited(ROOM_COORD(x, y)))
        tileId = 0x05;
    else if (IsRoomAdjacentToVisited(ROOM_COORD(x, y)))
        tileId = 0x01;
    else // Room is empty or unvisited and not adjacent: don't draw.
        return;
    
    bgX = 5 + x*2;
    bgY = 2 + y*2;

    // The rooms are drawn directly on the background using tiles from the tilemap.
    tilemapPtr[POS_TO_SCR_ADDR(bgX, bgY)] = SCR_MAP_ENTRY(tileId, 0, FALSE, FALSE);
    tilemapPtr[POS_TO_SCR_ADDR(bgX + 1, bgY)] = SCR_MAP_ENTRY(tileId + 1, 0, FALSE, FALSE);
    tilemapPtr[POS_TO_SCR_ADDR(bgX, bgY + 1)] = SCR_MAP_ENTRY(tileId + 2, 0, FALSE, FALSE);
    tilemapPtr[POS_TO_SCR_ADDR(bgX + 1, bgY + 1)] = SCR_MAP_ENTRY(tileId + 3, 0, FALSE, FALSE);

    // Draw appropriate room icon.
    DrawRoomIcon(x, y);
}

static void ShowRooms(void)
{
    u32 x, y;
	for (x = 0; x < MAX_LAYOUT_WIDTH; ++x)
    {
        for (y = 0; y < MAX_LAYOUT_HEIGHT; ++y)
        {
            if (DoesRoomExist(ROOM_COORD(x, y)))
                DrawRoomOnBg(x, y);
        }
    }
    ScheduleBgCopyTilemapToVram(2);
}

// Cleans the windows
static void CleanWindows(void)
{
    u32 i;
	for (i = 0; i < WINDOW_COUNT; ++i)
		FillWindowPixelBuffer(i, PIXEL_FILL(0));
}

// Display commited windows
static void CommitWindows(void)
{
    u32 i;
	for (i = 0; i < WINDOW_COUNT; ++i)
	{
		CopyWindowToVram(i, 3);
		PutWindowTilemap(i);
	}
}

static void InitMapScreen(void)
{
    // Clean windows.
	CleanWindows();
	CommitWindows();

    ShowRooms();
    PrintFloorText();
    
	// Display newly commited windows.
	CommitWindows();
}

static void LoadMapScreenGfx(void)
{	
    DecompressAndCopyTileDataToVram(2, &sMapScreenBgGfx, 0, 0, 0);
	LZDecompressWram(sMapScreenBgMap, sMapScreenTilemapPtr);
	LoadPalette(sMapScreenBgPal, 0, 0x20);
	ListMenuLoadStdPalAt(0xC0, 1);
	Menu_LoadStdPalAt(0xF0);
}

void ShowMapScreen(void)
{
    if (gFloorplan.numRooms != 0)
	{
        sRoomBuffer = gSaveBlock1Ptr->currentRoom;
        PlaySE(SE_RG_CARD_FLIPPING);
        PlayRainStoppingSoundEffect();
		SetMainCallback2(CB2_MapScreen);
	}
    else
    {
		SetMainCallback2(CB2_ReturnToField);
    }
}

void InitMinimap(void)
{
    u32 i;
    // If palette tag is unloaded, assume all tiles are unloaded.
    if (IndexOfSpritePaletteTag(TAG_ROOM_PAL) == 0xFF)
    {
        for (i = 0; i < ARRAY_COUNT(sMinimapSpriteIds); ++i)
            sMinimapSpriteIds[i] = 0xFF;

        LoadSpritePalette(&sRoomsSpritePalette);
        LoadSpriteSheet(&sCurrentRoomSpriteSheet);
        LoadSpriteSheet(&sVisitedRoomSpriteSheet);
        LoadSpriteSheet(&sUnvisitedRoomSpriteSheet);
        LoadSpriteSheet(&sMinimapBorderSpriteSheet);
        LoadSpriteSheet(&sBossRoomSpriteSheet);
        LoadSpriteSheet(&sTreasureRoomSpriteSheet);
        LoadSpriteSheet(&sShopRoomSpriteSheet);
        LoadSpriteSheet(&sChallengeRoomSpriteSheet);
    }
}

void DrawMinimap(bool32 refresh)
{
    u32 i, room, type;
    s32 xOffset, yOffset;

    // Don't display outside of floors.
    if (!IsPlayerInFloorMap())
        return;

    // Refresh if required (i.e. CONNECTION_TYPE_SEAMLESS).
    if (IndexOfSpritePaletteTag(TAG_ROOM_PAL) != 0xFF)
    {
        if (!refresh)
            return;

        for (i = 0; i < ARRAY_COUNT(sMinimapSpriteIds); ++i)
        {
            DestroySprite(&gSprites[sMinimapSpriteIds[i]]);
            sMinimapSpriteIds[i] = 0xFF;
        }
    }

    // Load graphics data if needed.
    InitMinimap();

    // Draw border.
    if (sMinimapSpriteIds[9] == 0xFF) // avoid redrawing border to minimize flicker with alpha
        sMinimapSpriteIds[9] = CreateSprite(&sMinimapBorderSpriteTemplate, 240 - 13, 13, 1);

    // Draw rooms.
    for (i = 0; i < 9; ++i)
    {
        xOffset = (i%3)-1;
        yOffset = (i/3)-1;
        room = gSaveBlock1Ptr->currentRoom + xOffset + 10*yOffset;
        type = GetRoomType(room);
        if (type != NORMAL_ROOM && DoesRoomExist(room) && IsRoomAdjacentToVisited(room))
            sMinimapSpriteIds[i] = CreateSprite(sRoomTypeSpriteTable[type].spriteTemplate, 240 - 16 + 8*xOffset, 16 + 8*yOffset, 0);
        else if (room == gSaveBlock1Ptr->currentRoom)
            sMinimapSpriteIds[i] = CreateSprite(&sCurrentRoomSpriteTemplate, 240 - 16 + 8*xOffset, 16 + 8*yOffset, 0);
        else if (IsRoomVisited(room))
            sMinimapSpriteIds[i] = CreateSprite(&sVisitedRoomSpriteTemplate, 240 - 16 + 8*xOffset, 16 + 8*yOffset, 0);
        else if (DoesRoomExist(room) && IsRoomAdjacentToVisited(room))
            sMinimapSpriteIds[i] = CreateSprite(&sUnvisitedRoomSpriteTemplate, 240 - 16 + 8*xOffset, 16 + 8*yOffset, 0);
    }
    
    // Make sprites transparent.
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(14, 8));
    for (i = 0; i < ARRAY_COUNT(sMinimapSpriteIds); ++i)
    {
        if (sMinimapSpriteIds[i] != 0xFF)
            gSprites[sMinimapSpriteIds[i]].oam.objMode = ST_OAM_OBJ_BLEND;
    }
}

void SetMinimapVisibility(bool32 invisible)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sMinimapSpriteIds); ++i)
    {
        if (sMinimapSpriteIds[i] != 0xFF)
            gSprites[sMinimapSpriteIds[i]].invisible = invisible;
    }
}
