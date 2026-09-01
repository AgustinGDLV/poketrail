#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "bg.h"
#include "start_screen.h"
#include "decompress.h"
#include "deck_battle.h"
#include "deck_battle_interface.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "map_preview.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item.h"
#include "item_icon.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "map_gen.h"
#include "menu.h"
#include "money.h"
#include "naming_screen.h"
#include "overworld.h"
#include "palette.h"
#include "play_time.h"
#include "pokemon_icon.h"
#include "pokemon_animation.h"
#include "random.h"
#include "rtc.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "text_window.h"
#include "title_screen.h"
#include "trainer_pokemon_sprites.h"
#include "trail_interface.h"
#include "util.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// const rom data
#define MAIN_MENU_BORDER_TILE   0x1D5

enum Windows
{
    WIN_BUTTONS_RIGHT,
    WIN_INFO,
	WIN_MAIN_TEXT,
    WIN_BUTTONS_LEFT,
	WINDOW_COUNT,
};

static const struct WindowTemplate sStartScreenWinTemplates[WINDOW_COUNT + 1] =
{
    [WIN_BUTTONS_RIGHT] =
    {
        .bg = 1,
		.tilemapLeft = 16,
		.tilemapTop = 0,
		.width = 14,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 1,
    },
    [WIN_INFO] =
	{
		.bg = 1,
		.tilemapLeft = 2,
		.tilemapTop = 8,
		.width = 26,
		.height = 6,
		.paletteNum = 15,
		.baseBlock = 1 + 28,
	},
	[WIN_MAIN_TEXT] =
	{
		.bg = 1,
		.tilemapLeft = 1,
		.tilemapTop = 17,
		.width = 28,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 1 + 28 + 156,
	},
    [WIN_BUTTONS_LEFT] =
    {
        .bg = 1,
		.tilemapLeft = 0,
		.tilemapTop = 0,
		.width = 16,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 1 + 28 + 156 + 56,
    },
	DUMMY_WIN_TEMPLATE,
};

static const struct BgTemplate sStartScreenBgTemplates[] =
{
    {   // Interface
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {   // Background
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 21,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

// functions
static void LoadMapPreviewGfx(void);
static void LoadScreenGfx(void);
static void Task_StartScreenFadeIn(u8 taskId);
static void Task_StartScreenFadeOutAndExit(u8 taskId);
static void Task_ContinueScreenWaitForKeypress(u8 taskId);
static void DrawBattlerSprites(void);

EWRAM_DATA static u32 * sMapPreviewTilemapPtr = NULL;
EWRAM_DATA static u32 * sOverlayTilemapPtr = NULL;
EWRAM_DATA static MainCallback sExitCallback = NULL;
EWRAM_DATA static bool8 sStartScreenWindowIds[WINDOW_COUNT] = {0};
EWRAM_DATA static u8 sOverworldSpriteId = 0;

static const union AnimCmd sAnim_Paused[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Idle[] =
{
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(16, 12),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_Battler[] =
{
    sAnim_Paused,
    sAnim_Idle,
};

static const struct OamData sOamData_Battler =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const u8 sDummyObjectGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/player_idle.4bpp");
static const u16 sOverlayPalette[] = INCBIN_U16("graphics/deck_battle_backgrounds/overlay.gbapal");
static const u32 sOverlayTiles[] = INCBIN_U32("graphics/deck_battle_backgrounds/overlay.4bpp.lz");
static const u32 sOverlayMap[] = INCBIN_U32("graphics/deck_battle_backgrounds/overlay.bin.lz");

// code
static void MainCB2_StartScreen(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
	UpdatePaletteFade();
}

static void VBlankCB_StartScreen(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

void CB2_StartScreen(void)
{
    u32 i;
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
            sMapPreviewTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            sOverlayTilemapPtr = AllocZeroed(BG_SCREEN_SIZE);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sStartScreenBgTemplates, ARRAY_COUNT(sStartScreenBgTemplates));
            SetBgTilemapBuffer(2, sMapPreviewTilemapPtr);
            SetBgTilemapBuffer(0, sOverlayTilemapPtr);
            gMain.state++;
            break;
        case 4:
            for (i = 0; i < WINDOW_COUNT; ++i)
                sStartScreenWindowIds[i] = WINDOW_NONE;
            InitWindows(sStartScreenWinTemplates);
            LoadMapPreviewGfx();
            gMain.state++;
            break;
        case 5:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                ShowBg(0);
                ShowBg(1);
                ShowBg(2);
                CopyBgTilemapBufferToVram(0);
                CopyBgTilemapBufferToVram(2);
                gMain.state++;
            }
            break;
        case 6:
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 7:
            if (gSaveBlock1Ptr->unlockedCharacters == 0)
            {
                gSaveBlock1Ptr->unlockedCharacters |= 1 << CHAR_BUG_CATCHER;
                gSaveBlock1Ptr->unlockedCharacters |= 1 << CHAR_HIKER;
                gSaveBlock1Ptr->unlockedCharacters |= 1 << CHAR_SWIMMER;
                gSaveBlock1Ptr->unlockedCharacters |= 1 << CHAR_BREEDER;
            }
            gMain.state++;
            break;
        case 8:
            LoadMessageBoxAndBorderGfx();
            LoadScreenGfx();
            gMain.state++;
            break;
        case 9:
            SetVBlankCallback(VBlankCB_StartScreen);
            CreateTask(Task_StartScreenFadeIn, 0);
            SetMainCallback2(MainCB2_StartScreen);
            break;
	}
}

static void ClearWindows(void)
{
    u32 i;
    for (i = 0; i < WINDOW_COUNT; ++i)
    {
        FillWindowPixelBuffer(sStartScreenWindowIds[i], PIXEL_FILL(0));
        ClearWindowTilemap(sStartScreenWindowIds[i]);
        CopyWindowToVram(sStartScreenWindowIds[i], COPYWIN_GFX);
        RemoveWindow(sStartScreenWindowIds[i]);
        sStartScreenWindowIds[i] = WINDOW_NONE;
    }
}

static void Task_StartScreenFadeOutAndExit(u8 taskId)
{
	if (!gPaletteFade.active)
	{
        SetMainCallback2(sExitCallback);
		Free(sMapPreviewTilemapPtr);
        sMapPreviewTilemapPtr = NULL;
		Free(sOverlayTilemapPtr);
        sOverlayTilemapPtr = NULL;
        ClearWindows();
		FreeAllWindowBuffers();
		DestroyTask(taskId);
	}
}

static void Task_StartScreenFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
	{
        gTasks[taskId].func = Task_ContinueScreenWaitForKeypress;
	}
}

static void Task_ContinueScreenWaitForKeypress(u8 taskId)
{
    if (gMain.newKeys & A_BUTTON)
	{
        PlaySE(SE_SELECT);
		BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sExitCallback = CB2_ContinueSavedGame;
		gTasks[taskId].func = Task_StartScreenFadeOutAndExit;
    }
    if (gMain.newKeys & B_BUTTON)
	{
        PlaySE(SE_SELECT);
		BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sExitCallback = CB2_InitTitleScreen;
		gTasks[taskId].func = Task_StartScreenFadeOutAndExit;
    }
    if (gMain.newKeys & START_BUTTON)
	{
        PlaySE(SE_SELECT);
		BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sExitCallback = CB2_StartNewRun;
		gTasks[taskId].func = Task_StartScreenFadeOutAndExit;
    }
}

static void LoadMapPreviewGfx(void)
{
    const struct DeckBattleBackground *bg = GetCurrentDeckBattleBackground();
    DecompressAndCopyTileDataToVram(2, bg->tiles, 0, 0, 0);
	LZDecompressWram(bg->map, sMapPreviewTilemapPtr);
	LoadPalette(bg->palette, BG_PLTT_ID(1), PLTT_SIZE_4BPP);
	Menu_LoadStdPalAt(BG_PLTT_ID(15));

    DecompressAndCopyTileDataToVram(0, sOverlayTiles, 0, 0, 0);
	LZDecompressWram(sOverlayMap, sOverlayTilemapPtr);
	LoadPalette(sOverlayPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
}

// Draw windows for continue and character select screen.
static void DrawContinueAndSelectWindows(void)
{
    u32 i;
    // Load graphics for windows with border graphics and fill.
    for (i = WIN_INFO; i < WIN_MAIN_TEXT; ++i)
    {
        // Avoid allocating space for duplicate windows.
        if (sStartScreenWindowIds[i] == WINDOW_NONE)
            sStartScreenWindowIds[i] = AddWindow(&sStartScreenWinTemplates[i]);
        FillWindowPixelBuffer(sStartScreenWindowIds[i], PIXEL_FILL(1));
        DrawStdWindowFrame(sStartScreenWindowIds[i], FALSE);
        PutWindowTilemap(sStartScreenWindowIds[i]);
        CopyWindowToVram(sStartScreenWindowIds[i], COPYWIN_FULL);
    }
    // Load remaining windows.
    for (; i < WINDOW_COUNT; ++i)
    {
        if (sStartScreenWindowIds[i] == WINDOW_NONE)
            sStartScreenWindowIds[i] = AddWindow(&sStartScreenWinTemplates[i]);
        FillWindowPixelBuffer(sStartScreenWindowIds[i], PIXEL_FILL(0));
        PutWindowTilemap(sStartScreenWindowIds[i]);
        CopyWindowToVram(sStartScreenWindowIds[i], COPYWIN_FULL);
    }
}

static const u8 sTextColor_Instructions[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};

static void DrawStartButtonWindow(void)
{
    u16 palette = RGB(0, 0, 0); // dark gray used in BG top/bottom
    const u8 sText_PressLR[] = _("{START_BUTTON} New Game");
    if (sStartScreenWindowIds[WIN_BUTTONS_RIGHT] == WINDOW_NONE)
        sStartScreenWindowIds[WIN_BUTTONS_RIGHT] = AddWindow(&sStartScreenWinTemplates[WIN_BUTTONS_RIGHT]);
    FillWindowPixelBuffer(sStartScreenWindowIds[WIN_BUTTONS_RIGHT], PIXEL_FILL(0));
    PutWindowTilemap(sStartScreenWindowIds[WIN_BUTTONS_RIGHT]);
    LoadPalette(&palette, BG_PLTT_ID(15) + 11, PLTT_SIZEOF(1));
    AddTextPrinterParameterized3(sStartScreenWindowIds[WIN_BUTTONS_RIGHT], FONT_SMALL, GetStringRightAlignXOffset(FONT_SMALL, sText_PressLR, 110), 0, sTextColor_Instructions, TEXT_SKIP_DRAW, sText_PressLR);
    CopyWindowToVram(sStartScreenWindowIds[WIN_BUTTONS_RIGHT], COPYWIN_FULL);
}

static void DrawContinueScreenText(void)
{
    const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};

    // Set up time of day string.
	StringCopy(gStringVar1, COMPOUND_STRING("DAY "));
	ConvertIntToDecimalStringN(gStringVar2, gSaveBlock1Ptr->day, STR_CONV_MODE_LEFT_ALIGN, 3);
	StringAppend(gStringVar1, gStringVar2);

    StringCopy(gStringVar2, COMPOUND_STRING(", "));
	ConvertIntToDecimalStringN(gStringVar3, gSaveBlock1Ptr->hour, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar2, gStringVar3);
    StringAppend(gStringVar1, gStringVar2);

    StringCopy(gStringVar2, COMPOUND_STRING(":"));
	ConvertIntToDecimalStringN(gStringVar3, 0, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar2, gStringVar3);
    StringAppend(gStringVar1, gStringVar2);

    if ((gSaveBlock1Ptr->halfDay == 0) != (gSaveBlock1Ptr->hour != 12)) // this is an XOR
        StringCopy(gStringVar2, COMPOUND_STRING(" PM"));
    else
        StringCopy(gStringVar2, COMPOUND_STRING(" AM"));
	StringAppend(gStringVar1, gStringVar2);

    // Load text into stats window.
    AddTextPrinterParameterized3(sStartScreenWindowIds[WIN_INFO], FONT_NORMAL, 2, 0, textColor, TEXT_SKIP_DRAW, gSaveBlock2Ptr->playerName);
    AddTextPrinterParameterized3(sStartScreenWindowIds[WIN_INFO], FONT_NORMAL, 2, 16, textColor, TEXT_SKIP_DRAW, GetCurrentTemplateRules()->name);
    AddTextPrinterParameterized3(sStartScreenWindowIds[WIN_INFO], FONT_NORMAL, 2, 32, textColor, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(sStartScreenWindowIds[WIN_INFO], COPYWIN_FULL);

    // Load text into main window.
    AddTextPrinterParameterized3(sStartScreenWindowIds[WIN_MAIN_TEXT], FONT_NORMAL, 2, 2, sTextColor_Instructions, TEXT_SKIP_DRAW, COMPOUND_STRING("Continue run?"));
    CopyWindowToVram(sStartScreenWindowIds[WIN_MAIN_TEXT], COPYWIN_FULL);

    // Load instructions into button window.
    AddTextPrinterParameterized3(sStartScreenWindowIds[WIN_BUTTONS_LEFT], FONT_SMALL, 2, 0, sTextColor_Instructions, TEXT_SKIP_DRAW, COMPOUND_STRING("{A_BUTTON} Continue"));
    CopyWindowToVram(sStartScreenWindowIds[WIN_BUTTONS_LEFT], COPYWIN_FULL);
}

#define dShadow data[0]

static void DrawPlayerObject(u32 characterId)
{
    // Draw the player sprite.
    sOverworldSpriteId = CreateObjectGraphicsSprite(gCharacterInfos[characterId].graphicsId, SpriteCallbackDummy, 32, 38, 0);
    SetAndStartSpriteAnim(&gSprites[sOverworldSpriteId], ANIM_STD_GO_SOUTH, 0);
    gSprites[sOverworldSpriteId].oam.priority = 0;

    // Draw shadow.
    u32 spriteId = CreateSprite(&gShadowSpriteTemplate, 32, 44, 16);
    gSprites[spriteId].callback = SpriteCallbackDummy;
}

#undef dShadow

static void DrawBattlerSprites(void)
{
    u32 species, palIndex, spriteId;

    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (species == SPECIES_NONE)
            continue;

        // Draw battler sprite.
        palIndex = LoadSpritePaletteWithTag(gDeckSpeciesInfo[species].objectPalette, 8001 + i);
        const struct SpriteSheet spriteSheet = {gDeckSpeciesInfo[species].opponentIdle, sizeof(sDummyObjectGfx), 8001 + i};
        const struct SpriteTemplate spriteTemplate =
        {
            .tileTag = 8001+i,
            .paletteTag = 0,
            .oam = &sOamData_Battler,
            .anims = sAnims_Battler,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };
        LoadSpriteSheet(&spriteSheet);
        spriteId = CreateSprite(&spriteTemplate, 68 + 28*i, 44 + gDeckSpeciesInfo[species].opponentYOffset, 0);
        gSprites[spriteId].oam.paletteNum = palIndex;
        StartSpriteAnim(&gSprites[spriteId], ANIM_IDLE);

        // Draw shadow.
        spriteId = CreateSprite(&gShadowSpriteTemplate, 68 + 28*i, 44, 16);
        gSprites[spriteId].callback = SpriteCallbackDummy;
    }
}

static void LoadScreenGfx(void)
{
    // Load party sprites.
    LoadSpritePalette(&gMiscGfxSpritePalette);
    LoadSpriteSheet(&gShadowSpriteSheet);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 6));
    DrawBattlerSprites();

    // Load main graphics.
    DrawStartButtonWindow();
    DrawPlayerObject(gSaveBlock1Ptr->characterId);
    DrawContinueAndSelectWindows();
    DrawContinueScreenText();
}

// Set the starting seed for a new run (from SeedRNGWithRTC, but custom seeds may be implemented)
static void SeedNewRunRNG(void)
{
    #define BCD8(x) ((((x) >> 4) & 0xF) * 10 + ((x) & 0xF))
    u32 seconds;
    struct SiiRtcInfo rtc;
    RtcGetInfo(&rtc);
    seconds =
        ((HOURS_PER_DAY * RtcGetDayCount(&rtc) + BCD8(rtc.hour))
        * MINUTES_PER_HOUR + BCD8(rtc.minute))
        * SECONDS_PER_MINUTE + BCD8(rtc.second);
    gFloorplan.nextFloorSeed = seconds;
    #undef BCD8
}


// Clears run-specific save data to start a new run or end a run.
static void ResetRunSaveData(void)
{
    gSaveBlock1Ptr->characterId = 0;
    gSaveBlock1Ptr->currentFloor = 0;
    gSaveBlock1Ptr->currentRoom = 0;
    gSaveBlock1Ptr->currentTemplateType = 0;
}

// Sets flags for an entirely new save.
static void NewSaveInitData(void)
{
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_SYS_POKEDEX_GET);
    FlagSet(FLAG_RECEIVED_POKEDEX_FROM_BIRCH);
    FlagSet(FLAG_RECEIVED_RUNNING_SHOES);
    FlagSet(FLAG_SYS_B_DASH);
    StringCopy(gSaveBlock2Ptr->playerName, COMPOUND_STRING("Player"));
    EnableNationalPokedex();
}

// Clear party, bag, and playtime.
static void NewRunInitData(void)
{
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    gPlayerPartyCount = 0;
    gSaveBlock1Ptr->checkpoints = (1 << CHECKPOINT_PEONY_TOWN);
    gSaveBlock1Ptr->trailX = 16;
    gSaveBlock1Ptr->trailY = 8;
    gSaveBlock1Ptr->hour = 9;
    gSaveBlock1Ptr->minute = 0;
    gSaveBlock1Ptr->halfDay = 0;
    gSaveBlock1Ptr->day = 1;
    gSaveBlock1Ptr->facing = DIR_SOUTH;
    StringCopy(gSaveBlock2Ptr->playerName, COMPOUND_STRING("Player"));
    gSaveBlock1Ptr->currentTemplateType = TEMPLATES_PEONY_TOWN;
    ClearBag();
    ClearFloorEventFlags();
    ClearCheckpointEventFlags();
    PlayTimeCounter_Reset();

    REG_TM1CNT_H = 0;
    REG_TM2CNT_H = 0;
    u32 val = ((u32)REG_TM2CNT_L) << 16;
    val |= REG_TM1CNT_L;
    SeedRng(val);
    gFloorplan.nextFloorSeed = val;
}

// Go to intro sequence.
void CB2_StartNewRun(void)
{
    StopMapMusic();
    ResetInitialPlayerAvatarState();
    PlayTimeCounter_Start();
    ScriptContext_Init();
    UnlockPlayerFieldControls();

    // TODO: Proper new game check
    if (!FlagGet(FLAG_RECEIVED_RUNNING_SHOES))
        NewSaveInitData();

    ResetRunSaveData();
    NewRunInitData();

    StoreInitialPlayerAvatarState();
    LockPlayerFieldControls();
    TryFadeOutOldMapMusic();
    WarpFadeOutScreen();
    PlayRainStoppingSoundEffect();
    SetWarpDestination(MAP_GROUP(INTRO_SEQUENCE), MAP_NUM(INTRO_SEQUENCE), WARP_ID_NONE, 7, 5);
    WarpIntoMap();
    SetMainCallback2(CB2_LoadMap);
}

// Assigns player character and refreshes graphics for intro sequence.
void AssignPlayerCharacter(void)
{
    struct SpriteTemplate spriteTemplate;
    struct SpriteFrameImage spriteFrameImage;
    const struct SubspriteTable *subspriteTables;
    const struct ObjectEventGraphicsInfo *graphicsInfo;

    // Update character ID and change sprite template.
    gSaveBlock1Ptr->characterId = gSpecialVar_Result;
    graphicsInfo = GetObjectEventGraphicsInfo(gCharacterInfos[gSpecialVar_Result].graphicsId);
    CopyObjectGraphicsInfoToSpriteTemplate_WithMovementType(gCharacterInfos[gSpecialVar_Result].graphicsId, MOVEMENT_TYPE_WALK_LEFT_AND_RIGHT, &spriteTemplate, &subspriteTables);
    spriteFrameImage.size = graphicsInfo->size;
    spriteTemplate.images = &spriteFrameImage;
    
    gSprites[gObjectEvents[gPlayerAvatar.objectEventId].spriteId].images = graphicsInfo->images;
}

// Trigger naming screen for intro sequence.
void AssignPlayerName(void)
{
    DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnToFieldContinueScript);
}

// Set VAR_RESULT to equal player's character ID.
void GetCharacterId(void)
{
    gSpecialVar_Result = gSaveBlock1Ptr->characterId;
}

// Assigns starter to player and stores in gStringVar1.
void AssignStarterToPlayer(void)
{
    u32 species = gCharacterInfos[gSaveBlock1Ptr->characterId].starters[gSpecialVar_Result];
    StringCopy(gStringVar1, GetSpeciesName(species));
    CreateMon(&gPlayerParty[0], species, 10, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, TRUE);
}
