#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "deck_battle.h"
#include "deck_battle_effects.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "deck_battle_controller.h"
#include "deck_battle_ai.h"
#include "decompress.h"
#include "event_object_movement.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "line_break.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "map_gen.h"
#include "menu.h"
#include "overworld.h"
#include "option_menu.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "random.h"
#include "region_map.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "international_string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "title_screen.h"
#include "util.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

/* deck_battle_interface.c
 *
 * This file handles all the graphics for deck battles, i.e.
 * battle boxes and background, player and opponent 'object' sprites,
 * portrait and information displays, and text printing.
 * 
*/

// forward declarations
static void LoadDummySpriteTiles(void);
static void SpriteCB_Cursor(struct Sprite *sprite);
static void SpriteCB_Shadow(struct Sprite *sprite);
static void SpriteCB_DamageNumber(struct Sprite *sprite);
static void SpriteCB_BattlerAttack(struct Sprite *sprite);
static void SpriteCB_BattlerHurt(struct Sprite *sprite);

// windows and bgs
enum Windows
{
    WINDOW_BATTLER_INFO,
    WINDOW_MESSAGE,
    WINDOW_COUNT,
};

static const struct WindowTemplate sDeckBattleWinTemplates[WINDOW_COUNT + 1] =
{
    [WINDOW_BATTLER_INFO] =
    {
        .bg = 1,
        .tilemapLeft = 6,
        .tilemapTop = 15,
        .width = 21,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    [WINDOW_MESSAGE] =
    {
        .bg = 1,
        .tilemapLeft = 3,
        .tilemapTop = 35,
        .width = 24,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 1 + 21*4,
    },
    DUMMY_WIN_TEMPLATE,
};

// BG handling is ripped almost exactly from the existing battle engine.
static const struct BgTemplate sDeckBattleBgTemplates[] =
{
    {   // Interface
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {   // Background
        .bg = 3,
        .charBaseIndex = 1,
        .mapBaseIndex = 21,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

// graphics data
#include "data/graphics/deck_battle.h"
#include "data/graphics/deck_battle_background.h"

const u8 sDummyObjectGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/player_idle.4bpp");
const u8 sDummyPortraitGfx[] = INCBIN_U8("graphics/deck_pokemon/slowpoke/portrait_normal.4bpp");
const u16 sDummyPal[] = INCBIN_U16("graphics/deck_pokemon/slowpoke/portrait_normal.gbapal");

static const struct SpriteSheet sDummyPortraitSpriteSheet = { sDummyPortraitGfx, sizeof(sDummyPortraitGfx), TAG_PORTRAIT };

static const struct SpriteTemplate sPortraitSpriteTemplate =
{
	.tileTag = TAG_PORTRAIT,
	.paletteTag = TAG_PORTRAIT,
	.oam = &sOamData_Portrait,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCallbackDummy,
};

static const struct SpriteSheet sDummyObjectSpriteSheets[MAX_DECK_BATTLERS_COUNT] = 
{
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 1 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 2 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 3 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 4 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 5 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 6 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 7 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 8 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 9 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 10 },
    { sDummyObjectGfx, sizeof(sDummyObjectGfx), TAG_BATTLER_OBJ + 11 },
};

static const struct SpriteTemplate sBattlerSpriteTemplates[MAX_DECK_BATTLERS_COUNT] =
{
    [B_PLAYER_0] =
    {
        .tileTag = TAG_BATTLER_OBJ,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 1,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 2,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 3,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 4,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_PLAYER_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 5,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_0] =
    {
        .tileTag = TAG_BATTLER_OBJ + 6,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_1] =
    {
        .tileTag = TAG_BATTLER_OBJ + 7,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_2] =
    {
        .tileTag = TAG_BATTLER_OBJ + 8,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_3] =
    {
        .tileTag = TAG_BATTLER_OBJ + 9,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_4] =
    {
        .tileTag = TAG_BATTLER_OBJ + 10,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
    [B_OPPONENT_5] =
    {
        .tileTag = TAG_BATTLER_OBJ + 11,
        .paletteTag = 0,
        .oam = &sOamData_Battler,
        .anims = sAnims_Battler,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
    },
};

// ewram data
EWRAM_DATA struct DeckBattleGraphics gDeckGraphics = {0};

// code
void ClearDeckBattleGraphicsStruct(void)
{
    for (u32 i = 0; i < POSITIONS_COUNT; ++i)
    {
        gDeckGraphics.battlerSpriteIds[B_SIDE_PLAYER] = SPRITE_NONE;
        gDeckGraphics.battlerSpriteIds[B_SIDE_OPPONENT] = SPRITE_NONE;
    }
    gDeckGraphics.swapCursorSpriteId = SPRITE_NONE;
    gDeckGraphics.portraitSpriteId = SPRITE_NONE;
}

const struct DeckBattleBackground * GetCurrentDeckBattleBackground(void)
{
    // Get time of day.
    u32 hours = gSaveBlock1Ptr->hour + gSaveBlock1Ptr->halfDay * 12;
    u32 timeOfDayOffset = 0;
    if (hours > 20 || hours < 6)
        timeOfDayOffset = 2; // Night
    else if (hours > 18 || hours < 8)
        timeOfDayOffset = 1; // Eve

    // Load battle background.
    const struct DeckBattleBackground *bg = &gDeckBackgrounds[GetCurrentTemplateRules()->background + timeOfDayOffset];
    return bg;
}

void LoadBattleBoxesAndBackground(void)
{
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sDeckBattleBgTemplates, ARRAY_COUNT(sDeckBattleBgTemplates));

    LZDecompressVram(sDeckBattleInterfaceTiles, (void *)(BG_CHAR_ADDR(0)));
    LZDecompressVram(sDeckBattleInterfaceTilemap, (void *)(BG_SCREEN_ADDR(14)));
    LoadPalette(sDeckBattleInterfacePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);

    const struct DeckBattleBackground *bg = GetCurrentDeckBattleBackground();
    LZDecompressVram(bg->tiles, (void *)(BG_CHAR_ADDR(1)));
    LZDecompressVram(bg->map, (void *)(BG_SCREEN_ADDR(21)));
    LoadPalette(bg->palette, BG_PLTT_ID(1), PLTT_SIZE_4BPP);

    CopyBgTilemapBufferToVram(0);
    CopyBgTilemapBufferToVram(3);
}

void InitDeckBattleWindows(void)
{
    u32 windowId;
    InitWindows(sDeckBattleWinTemplates);
    for (u32 i = 0; i < WINDOW_COUNT; ++i)
    {
        windowId = AddWindow(&sDeckBattleWinTemplates[i]);
        PutWindowTilemap(windowId);
        CopyWindowToVram(i, COPYWIN_FULL);
    }
}

static void LoadDummySpriteTiles(void)
{
    LoadSpriteSheet(&sDummyPortraitSpriteSheet);
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        LoadSpriteSheet(&sDummyObjectSpriteSheets[i]);
    }
}

#define tTimer  data[0]
#define tActive data[1]
#define tPause  data[2]

void Task_DoBattlerBobEffect(u8 taskId)
{
    u32 battler;
    if ((gTasks[taskId].tPause
        || gSaveBlock2Ptr->optionsBattleSceneOff
        || gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_AUTO)
        && gTasks[taskId].tTimer == 0)
    {
        gTasks[taskId].tActive = FALSE;
    }
    else
    {
        gTasks[taskId].tActive = TRUE;
        switch (++gTasks[taskId].tTimer)
        {
        // Move left.
        case 16:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 += 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 -= 1;
            break;
        case 32:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 += 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 -= 1;
            break;
        case 48:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 += 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 -= 1;
            break;
        // Pause.
        case 64:
            break;
        // Move right.
        case 80:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 -= 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 += 1;
            break;
        case 96:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 -= 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 += 1;
            break;
        case 112:
            for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 -= 1;
            for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                gSprites[gDeckGraphics.battlerSpriteIds[battler]].x2 += 1;
            break;
        case 128:
            gTasks[taskId].tTimer = 0;
            break;
        }
    }
}

void SetBattlerBobPause(bool32 pause)
{
    gTasks[gDeckGraphics.bobTaskId].tPause = pause;
}

bool32 IsBattlerBobActive(void)
{
    return gTasks[gDeckGraphics.bobTaskId].tActive;
}

#undef tTimer
#undef tActive
#undef tPause

#define sBattlerId  data[0]
#define sCursorId   data[1]
#define sAnimState  data[2]
#define sTimer      data[3]

static void SpriteCB_Shadow(struct Sprite *sprite)
{
    sprite->x = gSprites[gDeckGraphics.battlerSpriteIds[sprite->sBattlerId]].x;
    sprite->x2 = gSprites[gDeckGraphics.battlerSpriteIds[sprite->sBattlerId]].x2;
    sprite->y2 = gSprites[gDeckGraphics.battlerSpriteIds[sprite->sBattlerId]].y2;
    sprite->invisible = gSprites[gDeckGraphics.battlerSpriteIds[sprite->sBattlerId]].invisible;
}

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    sprite->x2 = gSprites[gDeckGraphics.battlerSpriteIds[sprite->sBattlerId]].x2;
    if (++sprite->animDelayCounter > 12)
    {
        sprite->y2 ^= 1;
        sprite->animDelayCounter = 0;
    }
}

void LoadBattlerPortrait(enum BattleId battler)
{
    u32 *dst, *src, index;

    FreeSpritePaletteByTag(TAG_PORTRAIT); // just in case?
    index = LoadSpritePaletteWithTag(gDeckSpeciesInfo[gDeckMons[battler].species].portraitPalette, TAG_PORTRAIT);

    dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_PORTRAIT)));
    src = (u32 *)gDeckSpeciesInfo[gDeckMons[battler].species].portrait;
    for (u32 i = 0; i < PORTRAIT_SIZE / 4; ++i)
        dst[i] = src[i];

    gSprites[gDeckGraphics.portraitSpriteId].oam.paletteNum = index;
    gSprites[gDeckGraphics.portraitSpriteId].invisible = FALSE;
}

void SetBattlerPortraitVisibility(bool32 visible)
{
    gSprites[gDeckGraphics.portraitSpriteId].invisible = (visible ? FALSE : TRUE);
}

void LoadBattlerObjectSprite(enum BattleId battler)
{
    u32 *dst, *src, index;

    if (IsDeckBattlerAlive(battler))
    {
        FreeSpritePaletteByTag(TAG_BATTLER_OBJ + battler); // just in case?
        index = LoadSpritePaletteWithTag(gDeckSpeciesInfo[gDeckMons[battler].species].objectPalette, TAG_BATTLER_OBJ + battler);

        dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + battler)));
        if (GetDeckBattlerSide(battler) == B_SIDE_PLAYER)
            src = (u32 *)gDeckSpeciesInfo[gDeckMons[battler].species].playerIdle;
        else
            src = (u32 *)gDeckSpeciesInfo[gDeckMons[battler].species].opponentIdle;
        for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
            dst[i] = src[i];

        gSprites[gDeckGraphics.battlerSpriteIds[battler]].oam.paletteNum = index;
        gSprites[gDeckGraphics.battlerSpriteIds[battler]].x = GetBattlerXCoord(battler);
        gSprites[gDeckGraphics.battlerSpriteIds[battler]].y = GetBattlerYCoord(battler);
    }
    else
    {
        gSprites[gDeckGraphics.battlerSpriteIds[battler]].invisible = TRUE;
    }
}

void InitDeckBattleGfx(void)
{
    LoadSpritePalette(&gMiscGfxSpritePalette);
    LoadSpriteSheet(&gShadowSpriteSheet);
    LoadSpriteSheet(&gCursorSpriteSheet);
    LoadSpriteSheet(&sNumberSpriteSheet);

    LoadDummySpriteTiles();
    gDeckGraphics.portraitSpriteId = CreateSprite(&sPortraitSpriteTemplate, PORTRAIT_X, PORTRAIT_Y, 0);
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        if (GetDeckBattlerSide(i) == B_SIDE_PLAYER)
        {
            gDeckGraphics.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], PLAYER_OBJ_X + OBJ_OFFSET*i, PLAYER_OBJ_Y, 0);
            gDeckGraphics.shadowSpriteIds[i] = CreateSprite(&gShadowSpriteTemplate, PLAYER_OBJ_X + OBJ_OFFSET*i, PLAYER_OBJ_Y, 64);
        }
        else
        {
            gDeckGraphics.battlerSpriteIds[i] = CreateSprite(&sBattlerSpriteTemplates[i], OPPONENT_OBJ_X + OBJ_OFFSET*(i-B_OPPONENT_0), OPPONENT_OBJ_Y, 0);
            gDeckGraphics.shadowSpriteIds[i] = CreateSprite(&gShadowSpriteTemplate, OPPONENT_OBJ_X + OBJ_OFFSET*(i-B_OPPONENT_0), OPPONENT_OBJ_Y, 64);
        }
        gSprites[gDeckGraphics.battlerSpriteIds[i]].sBattlerId = i;
        gSprites[gDeckGraphics.shadowSpriteIds[i]].sBattlerId = i;
    }

    // Blend shadow sprites; may not be the best use of our blend.
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 6));

    u32 battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
    UpdatePlayerHPBar(battler);
    LoadBattlerPortrait(battler);

    gDeckGraphics.bobTaskId = CreateTask(Task_DoBattlerBobEffect, 1);
}

void CreateSelectionCursorOverBattler(enum BattleId battler)
{
    struct Sprite *sprite = &gSprites[gDeckGraphics.battlerSpriteIds[battler]];
    sprite->sCursorId = CreateSprite(&gCursorSpriteTemplate, sprite->x, sprite->y - 16, 0);
    gSprites[sprite->sCursorId].sBattlerId = battler;
}

void RemoveSelectionCursorOverBattler(enum BattleId battler)
{
    DestroySprite(&gSprites[gSprites[gDeckGraphics.battlerSpriteIds[battler]].sCursorId]);
    gSprites[gDeckGraphics.battlerSpriteIds[battler]].sCursorId = SPRITE_NONE;
}

void CreateSelectionCursorOverPosition(enum BattlePosition position)
{
    gDeckGraphics.swapCursorSpriteId = CreateSprite(&gCursorSpriteTemplate, PLAYER_OBJ_X + OBJ_OFFSET * position, PLAYER_OBJ_Y - 16, 0);
}

void RemoveSwapSelectionCursor(void)
{
    DestroySprite(&gSprites[gDeckGraphics.swapCursorSpriteId]);
    gDeckGraphics.swapCursorSpriteId = SPRITE_NONE;
}

void SetBattlerGrayscale(enum BattleId battler, bool32 grayscale)
{
    SetGrayscaleOrOriginalPalette(16 + gSprites[gDeckGraphics.battlerSpriteIds[battler]].oam.paletteNum, (grayscale ? FALSE : TRUE));
}

static void SpriteCB_BattlerAttack(struct Sprite *sprite)
{
    u32 *dst, *src;
    if (gSaveBlock2Ptr->optionsBattleSceneOff)
    {
        switch (sprite->sAnimState)
        {
        case 0:
            dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
            if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].playerAttack;
            else
                src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].opponentAttack;
            for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
                dst[i] = src[i];
            StartSpriteAnim(sprite, ANIM_ATTACK);
            ++sprite->sAnimState;
            break;
        case 1:
            if (sprite->animEnded)
            {
                dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
                if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                    src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].playerIdle;
                else
                    src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].opponentIdle;
                for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
                    dst[i] = src[i];
                
                PlayCry_Normal(gDeckMons[sprite->sBattlerId].species, 0);
                StartSpriteAnim(sprite, ANIM_PAUSED);
                sprite->callback = SpriteCallbackDummy;
            }
            break;
        }
    }
    else
    {
        switch (sprite->sAnimState)
        {
        case 0: // Wait for bob to finish.
            if (!IsBattlerBobActive())
            {
                StartSpriteAnim(sprite, ANIM_PAUSED);
                ++sprite->sAnimState;
            }
            break;
        case 1: // Move forward and copy attack sprite tiles.
            switch (++sprite->sTimer)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                {
                    sprite->x2 += 1;
                    sprite->y2 -= 1;
                }
                else
                {
                    sprite->x2 -= 1;
                    sprite->y2 += 1;
                }
                break;
            case 6:
                dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
                if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                    src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].playerAttack;
                else
                    src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].opponentAttack;
                for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
                    dst[i] = src[i];

                StartSpriteAnim(sprite, ANIM_ATTACK);
                sprite->sTimer = 0;
                ++sprite->sAnimState;
                break;
            }
            break;
        case 2: // Play cry on frame 2.
            if (sprite->animCmdIndex == 1)
            {
                PlayCry_Normal(gDeckMons[sprite->sBattlerId].species, 0);
                ++sprite->sAnimState;
            }
            break;
        case 3: // Do shake.
            if (sprite->animCmdIndex == 1)
            {
                sprite->x2 ^= 1;
                sprite->y2 ^= 1;
            }
            else
            {
                sprite->x2 ^= 1; // shake happens an odd number of times, needs to be evened out
                sprite->y2 ^= 1;
                ++sprite->sAnimState;
            }
            break;
        case 4: // Wait for attack anim to finish.
            if (sprite->animEnded)
                ++sprite->sAnimState;
            break;
        case 5: // Move back and copy idle sprite tiles.
            switch (++sprite->sTimer)
            {
            case 4:
                dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
                if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                    src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].playerIdle;
                else
                    src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].opponentIdle;
                for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
                    dst[i] = src[i];
                // fall through
            case 8:
            case 12:
            case 16:
            case 20:
                if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
                {
                    sprite->x2 -= 1;
                    sprite->y2 += 1;
                }
                else
                {
                    sprite->x2 += 1;
                    sprite->y2 -= 1;
                }
                break;
            case 24:            
                sprite->sTimer = 0;
                sprite->sAnimState = 0;
                StartSpriteAnim(sprite, ANIM_PAUSED);
                sprite->callback = SpriteCallbackDummy;
                break;
            }
        }
    }
}

static void SpriteCB_BattlerHurt(struct Sprite *sprite)
{
    u32 *dst, *src;
    switch (sprite->sAnimState)
    {
    case 0: // Copy hurt sprite tiles.
        dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
        if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
            src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].playerHurt;
        else
            src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].opponentHurt;
        for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
            dst[i] = src[i];
        StartSpriteAnim(sprite, ANIM_HURT);
        ++sprite->sAnimState;
        break;
    case 1: // Wait for hurt anim to finish.
        if (sprite->animEnded)
        {
            sprite->invisible = FALSE;
            ++sprite->sAnimState;
        }
        else
        {
            if (++sprite->sTimer % 4 < 2) // *TODO - move to hurt callback
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
        }
        break;
    case 2:
        dst = (u32 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId)));
        if (GetDeckBattlerSide(sprite->sBattlerId) == B_SIDE_PLAYER)
            src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].playerIdle;
        else
            src = (u32 *)gDeckSpeciesInfo[gDeckMons[sprite->sBattlerId].species].opponentIdle;
        for (u32 i = 0; i < OBJECT_SIZE / 4; ++i)
            dst[i] = src[i];

        sprite->sAnimState = 0;
        sprite->sTimer = 0;
        StartSpriteAnim(sprite, ANIM_PAUSED);
        sprite->callback = SpriteCallbackDummy;
        break;
    }
}

static void SpriteCB_BattlerFaint(struct Sprite *sprite)
{
    if (--sprite->sTimer < 0)
    {
        sprite->sTimer = 2;
        sprite->y += 4; // Move the sprite down.
        if (--sprite->sAnimState < 0)
        {
            sprite->invisible = TRUE;
            sprite->callback = SpriteCallbackDummy;
        }
        else // Erase bottom part of the sprite to create a smooth illusion of mon falling down.
        {
            u8 *dst = (u8 *)(OBJ_VRAM0 + TILE_OFFSET_4BPP(GetSpriteTileStartByTag(TAG_BATTLER_OBJ + sprite->sBattlerId) + 4*sprite->sAnimState));
            *dst = 0;
            for (u32 i = 0; i < 128; i++)
                *(dst++) = 0;
        }
    }
}

static void SpriteCB_BattlerStatChange(struct Sprite *sprite)
{
    if (++sprite->sTimer >= 60)
    {
        BlendPalettes(1 << (16 + sprite->oam.paletteNum), 0, RGB_WHITE);
        sprite->sTimer = 0;
        sprite->callback = SpriteCallbackDummy;
    }
    else if (sprite->sTimer < 60)
    {
        if (sprite->sTimer % 4 < 2)
            BlendPalettes(1 << (16 + sprite->oam.paletteNum), 8, RGB_WHITE);
        else
            BlendPalettes(1 << (16 + sprite->oam.paletteNum), 0, RGB_WHITE);
    }
}

void StartBattlerAnim(enum BattleId battler, u32 animId)
{
    switch (animId)
    {
        default:
            StartSpriteAnim(&gSprites[gDeckGraphics.battlerSpriteIds[battler]], animId);
            break;
        case ANIM_ATTACK:
            SetBattlerBobPause(TRUE);
            gSprites[gDeckGraphics.battlerSpriteIds[battler]].callback = SpriteCB_BattlerAttack;
            break;
        case ANIM_HURT:
            gSprites[gDeckGraphics.battlerSpriteIds[battler]].callback = SpriteCB_BattlerHurt;
            break;
        case ANIM_FAINT:
            gSprites[gDeckGraphics.battlerSpriteIds[battler]].sAnimState = 4;
            gSprites[gDeckGraphics.battlerSpriteIds[battler]].callback = SpriteCB_BattlerFaint;
            break;
        case ANIM_STAT_CHANGE:
            gSprites[gDeckGraphics.battlerSpriteIds[battler]].callback = SpriteCB_BattlerStatChange;
            break;
    }
}

struct Sprite * GetBattlerSprite(enum BattleId battler)
{
    return &gSprites[gDeckGraphics.battlerSpriteIds[battler]];
}

u32 GetBattlerXCoord(enum BattleId battler)
{
    if (GetDeckBattlerSide(battler) == B_SIDE_PLAYER)
        return PLAYER_OBJ_X + OBJ_OFFSET * gDeckMons[battler].pos;
    else
        return OPPONENT_OBJ_X + OBJ_OFFSET * gDeckMons[battler].pos;
}

u32 GetBattlerYCoord(enum BattleId battler)
{
    if (GetDeckBattlerSide(battler) == B_SIDE_PLAYER)
        return PLAYER_OBJ_Y + gDeckSpeciesInfo[gDeckMons[battler].species].playerYOffset;
    else
        return OPPONENT_OBJ_Y + gDeckSpeciesInfo[gDeckMons[battler].species].opponentYOffset;
}

bool32 HasBattlerAnimTriggeredCry(enum BattleId battler)
{
    return gSprites[gDeckGraphics.battlerSpriteIds[battler]].sAnimState >= 3;
}

#undef sBattlerId
#undef sCursorId
#undef sAnimState
#undef sTimer

// Damage numbers by TheSylphIsIn
#define sDelay data[0] // wait time until sprite appears
#define sStayTimer data[1] // number of frames sprite stays onscreen
#define sState data[2] // state tracker for the little bounce the number does when it appears 
#define sTracker data[3] // timer for the state-based little bounce

static void SpriteCB_DamageNumber(struct Sprite *sprite)
{
	sprite->sTracker++;

	switch (sprite->sState)
	{
		case 0: // Sprites appear in sequence if there are multiple
			if (sprite->sDelay == 0)
			{
				sprite->invisible = FALSE;
				sprite->sState++;
				sprite->sTracker = 0;
			}
			else
				sprite->sDelay--;
			break;
		case 1: // Sprite bounces up a teeny bit 
			sprite->y--;
			if (sprite->sTracker == 4)
			{
				sprite->sTracker = 0;
				sprite->sState++;
			}
			break;
		case 2:
			sprite->y++; // Sprite slides back down to a resting position
			if (sprite->sTracker == 2)
			{
				sprite->sTracker = 0;
				sprite->sState++;
			}
			break;
		case 3: // Sprite slides down again when it's about to disappear
			if (sprite->sStayTimer < 4)
				sprite->y++;
			break;
		default:
			sprite->sTracker = 0;
	}

	if (sprite->invisible == FALSE)
	{
		sprite->sStayTimer--;
		if (sprite->sStayTimer == 0)
			DestroySprite(sprite);
	}
}

static u32 GetDamageDigit(u32 damage, u32 digit) 
{
	u32 result = 0;
	while (damage >= digit)
	{
		result++;
		damage -= digit;
	}

	return result;
}

static void CreateDamageNumberSprite(u32 number, u32 numPrinted, enum BattleId battler, bool32 heal)
{
	u32 spriteId = CreateSprite(&sNumberSpriteTemplate, GetBattlerXCoord(battler) + (7 * numPrinted),
                                GetBattlerYCoord(battler) - 16, 0);
	gSprites[spriteId].oam.tileNum += number + heal*10;
	gSprites[spriteId].sDelay = 2 * numPrinted;
	gSprites[spriteId].sStayTimer = 45;
	gSprites[spriteId].invisible = TRUE;
}

void PrintDamageNumbers(enum BattleId battler, s32 damage)
{
	u32 spriteNumber;
	u32 numPrinted = 0; // used to avoid printing leading 0s
    bool32 heal = FALSE;

	if (damage < 0)
    {
		damage *= -1; // healing is shown as a positive value with a green palette
        heal = TRUE;
    }

	// hundreds place
	spriteNumber = GetDamageDigit(damage, 100);
	damage -= spriteNumber * 100;
	if (spriteNumber > 0 || numPrinted != 0)
	{
		CreateDamageNumberSprite(spriteNumber, numPrinted, battler, heal);
		numPrinted++;
	}

	// tens place
	spriteNumber = GetDamageDigit(damage, 10);
	damage -= spriteNumber * 10;
	if (spriteNumber > 0 || numPrinted != 0)
	{
		CreateDamageNumberSprite(spriteNumber, numPrinted, battler, heal);
		numPrinted++;
	}

	// ones place
	spriteNumber = damage;
	CreateDamageNumberSprite(spriteNumber, numPrinted, battler, heal);
}

#undef sDelay
#undef sStayTimer
#undef sState
#undef sTracker

static const u8 sTextColorNormal[] = { 0, 1, 2 };

void PrintBattlerMoveInfo(enum BattleId battler)
{
    StringCopy(gStringVar1, gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[battler].species].move].name);
    StringAppend(gStringVar1, COMPOUND_STRING(": "));
    StringAppend(gStringVar1, gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[battler].species].move].description);
    BreakStringAutomatic(gStringVar1, 180, 2, FONT_NORMAL, SHOW_SCROLL_PROMPT);

    FillWindowPixelBuffer(WINDOW_BATTLER_INFO, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_BATTLER_INFO, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_BATTLER_INFO, COPYWIN_FULL);
}

void PrintTargetBattlerPrompt(enum BattleId battler)
{
    StringCopy(gStringVar2, GetSpeciesName(gDeckMons[battler].species));
    if (gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move].effect == DECK_EFFECT_POWER_UP)
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Boost {STR_VAR_2}?"));
    else
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Attack {STR_VAR_2}?"));

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintFixedTargetsPrompt(bool32 viableTarget)
{
    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    if (viableTarget)
        AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, COMPOUND_STRING("Select targets?"));
    else
        AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, COMPOUND_STRING("Select no targets?"));
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintMoveUseString(void)
{
    StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gBattlerAttacker].species));
    StringCopy(gStringVar3, GetMoveName(gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move));
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} used {STR_VAR_3}!"));

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintMoveOutcomeString(void) // *TODO: refactor
{
    // Prepare string buffers.
    StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gBattlerTarget].species)); // unsafe
    ConvertIntToDecimalStringN(gStringVar3, abs(gDeckStruct.lastHitDamage), STR_CONV_MODE_LEFT_ALIGN, 2);

    if (gDeckMovesInfo[gCurrentMove].effect == DECK_EFFECT_HIT)
    {
        if (gDeckMovesInfo[gCurrentMove].target & TARGET_SINGLE_OPPONENT)
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} took {STR_VAR_3} damage!"));
        if (gDeckMovesInfo[gCurrentMove].target & TARGET_ALL_OPPONENTS)
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Opponents took damage!"));
    }
    if (gDeckMovesInfo[gCurrentMove].effect == DECK_EFFECT_HEAL)
    {
        if (gDeckMovesInfo[gCurrentMove].target & (TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY | TARGET_SINGLE_ALLY))
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} healed {STR_VAR_3} HP!"));
        if (gDeckMovesInfo[gCurrentMove].target & TARGET_ALL_ALLIES)
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Allies had their HP healed!"));
    }
    else if (gDeckMovesInfo[gCurrentMove].effect == DECK_EFFECT_POWER_UP)
    {
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2}'s power was boosted!"));
    }

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    BreakStringAutomatic(gStringVar1, 128, 2, FONT_NORMAL, SHOW_SCROLL_PROMPT);
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintSwapTargetPrompt(enum BattlePosition position)
{
    enum BattleId battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, position);
    if (battler != MAX_DECK_BATTLERS_COUNT && IsDeckBattlerAlive(battler))
    {
        StringCopy(gStringVar2, GetSpeciesName(gDeckMons[battler].species));
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Swap with {STR_VAR_2}?"));
    }
    else
    {
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Swap to empty space?"));
    }

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintSwapString(enum BattleId battler1, enum BattleId battler2)
{
    StringCopy(gStringVar2, GetSpeciesName(gDeckMons[battler1].species));
    StringCopy(gStringVar3, GetSpeciesName(gDeckMons[battler2].species));
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} and {STR_VAR_3} swapped\nplaces!"));

    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, TEXT_SKIP_DRAW, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

void PrintStringToMessageBox(const u8 *str)
{
    StringCopy(gStringVar1, str);
    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    BreakStringAutomatic(gStringVar1, 196, 2, FONT_NORMAL, SHOW_SCROLL_PROMPT);
    AddTextPrinterParameterized3(WINDOW_MESSAGE, FONT_NORMAL, 4, 1, sTextColorNormal, 1, gStringVar1);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_FULL);
}

#define POS_TO_SCR_ADDR(x,y) (32*(y) + (x))
#define SCR_MAP_ENTRY(tile, pal, hflip, vflip) ((tile) | (hflip ? (1<<10) : 0) | (vflip ? (1 << 11) : 0) | (pal << 12))

static const u16 sHpBarBottomTiles[] = {0x24, 0x1C, 0x1B, 0x22};
static const u16 sHpBarMiddleTiles[] = {0x1A, 0x19, 0x14, 0xD, 0xA, 0x9, 0x8, 0x7, 0x18};
static const u16 sHpBarTopTiles[] = {0x4, 0x23, 0x13};

static void SetHPBarColor(enum BattleId battler)
{
    u16 palette;
    u32 pct = ((gDeckMons[battler].hp * 100) / gDeckMons[battler].maxHP);
    if (pct > 50)
        palette = RGB(0, 27, 0);
    else if (pct > 20)
        palette = RGB(27, 27, 0);
    else
        palette = RGB(27, 0, 0);
    LoadPalette(&palette, BG_PLTT_ID(0) + 3, PLTT_SIZEOF(1));
}

void UpdatePlayerHPBar(enum BattleId battler)
{
    u32 pixels = ((gDeckMons[battler].hp * 100) / gDeckMons[battler].maxHP) * 36 / 100;
    SetHPBarColor(battler);

    if (pixels >= 3) // bottom
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 19)) = sHpBarBottomTiles[3];
    else
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 19)) = sHpBarBottomTiles[pixels];

    if (pixels >= 11) // 1st from bottom
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 18)) = sHpBarMiddleTiles[8];
    else if (pixels >= 3)
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 18)) = sHpBarMiddleTiles[pixels-3];
    else
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 18)) = sHpBarMiddleTiles[0];

    if (pixels >= 19) // 2nd from bottom
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 17)) = sHpBarMiddleTiles[8];
    else if (pixels >= 11)
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 17)) = sHpBarMiddleTiles[pixels-11];
    else
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 17)) = sHpBarMiddleTiles[0];

    if (pixels >= 27) // 3rd from bottom
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 16)) = sHpBarMiddleTiles[8];
    else if (pixels >= 19)
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 16)) = sHpBarMiddleTiles[pixels-19];
    else
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 16)) = sHpBarMiddleTiles[0];

    if (pixels >= 35) // 4th from bottom
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 15)) = sHpBarMiddleTiles[8];
    else if (pixels >= 27)
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 15)) = sHpBarMiddleTiles[pixels-27];
    else
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 15)) = sHpBarMiddleTiles[0];

    if (pixels == 36) // in case of overflow
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 14)) = sHpBarTopTiles[2];
    else if (pixels == 35)
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 14)) = sHpBarTopTiles[1];
    else
        *((u16 *)(BG_SCREEN_ADDR(14)) + POS_TO_SCR_ADDR(28, 14)) = sHpBarTopTiles[0];

    CopyBgTilemapBufferToVram(0);
}

// Update graphics for selecting and deselecting a battler.
void UpdateBattlerSelection(enum BattleId battler, bool32 selected)
{
    if (selected)
    {
        CreateSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_IDLE);
    }
    else
    {
        RemoveSelectionCursorOverBattler(battler);
        StartBattlerAnim(battler, ANIM_PAUSED);
    }
}

// Update graphics for action selection.
void DisplayActionSelectionInfo(enum BattleId battler)
{
    LoadBattlerPortrait(battler);
    PrintBattlerMoveInfo(battler);
    UpdatePlayerHPBar(battler);
}

// Update graphics for swap selection.
void DisplaySwapSelectionInfo(enum BattlePosition position)
{
    u32 battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, position);
    PrintSwapTargetPrompt(position);
    if (battler != MAX_DECK_BATTLERS_COUNT)
        UpdatePlayerHPBar(battler);
}
