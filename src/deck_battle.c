#include "global.h"
#include "battle.h"
#include "bg.h"
#include "deck_battle.h"
#include "deck_battle_effects.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "deck_battle_controller.h"
#include "deck_battle_ai.h"
#include "event_object_movement.h"
#include "event_data.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "list_menu.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "money.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/vars.h"

/* deck_battle.c
 *
 * This file handles the main flow for the battle engine,
 * i.e. data initalization, opening and ending sequences,
 * and action execution.
 * 
*/

// forward declarations
static void MainCB2_DeckBattle(void);
static void VBlankCB2_DeckBattle(void);
static void Task_OpenDeckBattle(u8 taskId);
static void InitBattleStructData(void);
static void InitBattleMonData(void);
static void ResetTurnValues(void);

static void Task_HandleBattleVictory(u8 taskId);
static void Task_HandleBattleLoss(u8 taskId);
static void Task_HandleCaughtBattler(u8 taskId);
static void Task_HandleTurnEndEffects(u8 taskId);
static void Task_SelectPartyMemberToSwap(u8 taskId);

static u32 GetBattleSpeedScale(void);

// ewram data
EWRAM_DATA struct DeckBattleStruct gDeckStruct = {0};
EWRAM_DATA struct DeckBattlePokemon gDeckMons[MAX_DECK_BATTLERS_COUNT] = {0};

// code
#include "data/graphics/deck_pokemon.h"
#include "data/pokemon/deck_species_info.h"
#include "data/deck_moves.h"

static void MainCB2_DeckBattle(void) // battle speed up ported from Pokeabbie by Alex/Rain
{
    u32 speedScale = GetBattleSpeedScale();
    if (PrevPaletteFadeResult() == PALETTE_FADE_STATUS_LOADING)
        speedScale = 1;

    if (speedScale <= 1)
    {
        RunTasks();
        AnimateSprites();
        BuildOamBuffer();
        DoScheduledBgTilemapCopiesToVram();
        UpdatePaletteFade();
        RunTextPrinters();
    }
    else
    {
        u32 s;
        u32 fadeResult;

        // Update select entries at higher speed
        // disable speed up during palette fades otherwise we run into issues with blending
        // (e.g. moves that change background like Psychic can get stuck or have their colours overflow)
        for(s = 1; s < speedScale; ++s)
        {
            AnimateSprites();
            RunTextPrinters();
            fadeResult = UpdatePaletteFade();

            if (fadeResult == PALETTE_FADE_STATUS_LOADING)
            {
                // minimal final update as we've just started a fade
                BuildOamBuffer();
                RunTasks();
                break;
            }
            else
            {
                RunTasks();
                VBlankCB2_DeckBattle();

                // Call it again to make sure everything is behaving as it should (this is crazy town now)
                if (gMain.callback1)
                    gMain.callback1();
            }
        }

        if (fadeResult != PALETTE_FADE_STATUS_LOADING)
        {
            // final update
            RunTasks();
            AnimateSprites();
            BuildOamBuffer();
            DoScheduledBgTilemapCopiesToVram();
            UpdatePaletteFade();
            RunTextPrinters();
        }
    }
}

static void VBlankCB2_DeckBattle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void PlayDeckBattleMusic(void)
{
    if (gDeckStruct.musicOverride != MUS_NONE)
        PlayBGM(gDeckStruct.musicOverride);
    else if (gDeckStruct.isBossBattle)
        PlayBGM(MUS_VS_RIVAL);
    else
        PlayBGM(MUS_VS_WILD);
}

void OpenDeckBattle(void)
{
    PlayDeckBattleMusic();
    FadeScreen(FADE_TO_BLACK, 0);
    SetMainCallback2(CB2_OpenDeckBattleCustom);
}

void CB2_OpenDeckBattleCustom(void)
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
            ClearTasksAndGraphicalStructs();
            ClearDeckBattleGraphicsStruct();
            gMain.state++;
            break;
        case 2:
            LoadBattleBoxesAndBackground();
            ShowBg(0);
            ShowBg(1);
            HideBg(2);
            ShowBg(3);
            gMain.state++;
            break;
        case 3:
            InitDeckBattleWindows();
            DeactivateAllTextPrinters();
            gMain.state++;
            break;
        case 4:
            InitBattleMonData();
            InitBattleStructData();
            CalculatePlayerPartyCount();
            CalculateEnemyPartyCount();
            gMain.state++;
            break;
        case 5:
            InitDeckBattleGfx();
            gMain.state++;
            break;
        case 6:
            for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
                LoadBattlerObjectSprite(battler);
            gMain.state++;
            break;
        case 7:
            BeginNormalPaletteFade(PALETTES_ALL, 4, 16, 0, RGB_BLACK);
            SetVBlankCallback(VBlankCB2_DeckBattle);
            gDeckStruct.isSelectionPhase = TRUE;
            CreateTask(Task_OpenDeckBattle, 0);
            SetMainCallback2(MainCB2_DeckBattle);
            break;
    }
}

#define tState          data[0]
#define tTimer          data[1]
#define tTurnEndState   data[3]

// Fades in battle UI and sets up remaining graphics.
static void Task_OpenDeckBattle(u8 taskId)
{
    if (gTasks[taskId].tState == 0)
    {
        gDeckStruct.actingSide = B_SIDE_PLAYER;
        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        PrintStringToMessageBox(COMPOUND_STRING("Wild Pokémon appeared!"));
        gTasks[taskId].tState += 1;
    }
    else if (!gPaletteFade.active && (gMain.newKeys & A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_AUTO)
        {
            SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
            SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
            gTasks[taskId].func = Task_AutoSelectAction;
            gTasks[taskId].tState = 0;
            gTasks[taskId].tTimer = 0;
        }
        else
        {
            // Start selection phase and update display.
            enum BattleId battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            PrintBattlerMoveInfo(battler);
            SetBattlerPortraitVisibility(TRUE);
            // HP bar updated before fade begins
            CreateSelectionCursorOverBattler(battler);
            StartBattlerAnim(battler, ANIM_IDLE);

            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
            gTasks[taskId].tState = 0;
            gTasks[taskId].tTimer = 0;
        }
    }
}

// Set up data and reset positions to begin action phase.
void Task_PrepareForActionPhase(u8 taskId)
{
    struct Sprite *sprite;
    switch (gTasks[taskId].tState)
    {
    case 0:
        // Reset battler positions and remove grayscale and transparency.
        for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            gDeckMons[battler].pos = gDeckMons[battler].initialPos;
            SetBattlerGrayscale(battler, FALSE);
            sprite = GetBattlerSprite(battler);
            sprite->oam.objMode = ST_OAM_OBJ_NORMAL;
            sprite->x = GetBattlerXCoord(battler);
        }
        gDeckStruct.isSelectionPhase = FALSE;
        ++gTasks[taskId].tState;
        break;
    case 1:
        // Execute queued actions
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
    }
}

void Task_CheckFaintAndContinue(u8 taskId)
{
    bool32 fainted = FALSE;

    // Look for fainted battlers.
    for (enum BattleId battler = B_PLAYER_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
    {
        if (!GetBattlerSprite(battler)->invisible && !IsDeckBattlerAlive(battler))
        {
            StartBattlerAnim(battler, ANIM_FAINT);
            fainted = TRUE;

            // If opposing battler, add to EXP count and try to catch.
            if (GetDeckBattlerSide(battler) == B_SIDE_OPPONENT)
            {
                gDeckStruct.exp += gSpeciesInfo[gDeckMons[battler].species].expYield * gDeckMons[battler].lvl;
                if (gDeckStruct.battlerCaught == MAX_DECK_BATTLERS_COUNT)
                    gDeckStruct.battlerCaught = battler;
            }
        }
    }

    // Do faint animations if fainted, otherwise continue to end turn.
    if (fainted)
    {
        PlaySE(SE_FAINT);
        gTasks[taskId].func = Task_WaitForFaintAnim;
    }
    else if (gTasks[taskId].tTurnEndState > 0) // TODO: There's probably better ways to route here.
    {
        gTasks[taskId].func = Task_HandleTurnEndEffects;
    }
    else
    {
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
    }
}

void Task_WaitForFaintAnim(u8 taskId)
{
    if (++gTasks[taskId].tTimer > 48)
    {
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].func = Task_CheckForBattleEnd;
    }
}

void Task_CheckForBattleEnd(u8 taskId)
{
    if (!IsBattlerAliveOnSide(B_SIDE_PLAYER))
    {
        gBattleOutcome = B_OUTCOME_LOST;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_HandleBattleLoss;
    }
    else if (!IsBattlerAliveOnSide(B_SIDE_OPPONENT))
    {
        gBattleOutcome = B_OUTCOME_WON;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_HandleBattleVictory;
    }
    else if (gTasks[taskId].tTurnEndState > 0) // TODO: There's probably better ways to route here.
    {
        gTasks[taskId].func = Task_HandleTurnEndEffects;
    }
    else
    {
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
    }
}

// Runs main action phase loop until no queued actions are left.
void Task_ExecuteQueuedActionOrEnd(u8 taskId)
{
    // Set up data and execute queued action if any remain.
    if (gDeckStruct.executedCount < gDeckStruct.actionsCount)
    {
        gBattlerAttacker = gDeckStruct.queuedActions[gDeckStruct.executedCount].attacker;
        gBattlerTarget = gDeckStruct.queuedActions[gDeckStruct.executedCount].target;
        gCurrentMove = gDeckStruct.queuedActions[gDeckStruct.executedCount].move;

        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        if (gDeckStruct.queuedActions[gDeckStruct.executedCount].type == ACTION_ATTACK)
            gTasks[taskId].func = Task_ExecuteMove;
        else
            gTasks[taskId].func = Task_ExecuteSwap;
        ++gDeckStruct.executedCount;
    }
    // Otherwise, run through turn end effects.
    else
    {
        gTasks[taskId].tState = 0;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tTurnEndState = 0;
        gTasks[taskId].func = Task_HandleTurnEndEffects;
    }
}

static void Task_HandleBattleVictory(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    default:
    case 0: // Print EXP message.
    {
        gDeckStruct.isSelectionPhase = TRUE;
        u32 exp = gDeckStruct.exp / gPlayerPartyCount; // *TODO - variable exp gain per battler
        ConvertIntToDecimalStringN(gStringVar2, exp, STR_CONV_MODE_LEFT_ALIGN, 5);
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Your party gained an average of {STR_VAR_2} Exp. Points!"));
        PrintStringToMessageBox(gStringVar1);

        PlayBGM(MUS_VICTORY_WILD);
        gDeckStruct.battlerExp = B_PLAYER_0;
        ++gTasks[taskId].tState;
        break;
    }
    case 1: // Wait for message box.
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2: // Give experience to the current battler and print message if they level.
        if (gDeckMons[gDeckStruct.battlerExp].species != SPECIES_NONE)
        {
            u32 level = gDeckMons[gDeckStruct.battlerExp].lvl;
            u32 species = gDeckMons[gDeckStruct.battlerExp].species;
            u32 currentExp = GetMonData(&gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex], MON_DATA_EXP);
            u32 nextLevelExp = gExperienceTables[gSpeciesInfo[species].growthRate][level + 1];
            u32 gainedExp = gDeckStruct.exp / gPlayerPartyCount;
            u32 expAfterGain = currentExp + gainedExp;

            SetMonData(&gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex], MON_DATA_EXP, &expAfterGain);
            CalculateMonStats(&gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex]);
            if (expAfterGain >= nextLevelExp)
                gTasks[taskId].tState = 3;
            else
                gTasks[taskId].tState = 6;
        }
        else
        {
            gTasks[taskId].tState = 6;
        }
        break;
    case 3: // Print level up message.
        if (gTasks[taskId].tTimer == 0)
        {
            StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gDeckStruct.battlerExp].species));
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} leveled up!"));
            PrintStringToMessageBox(gStringVar1);
            PlaySE(MUS_LEVEL_UP);
            ++gTasks[taskId].tTimer;
        }
        else if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 4: // Check evolution.
        if (gDeckMons[gDeckStruct.battlerExp].species != SPECIES_NONE)
        {
            bool32 canStopEvo = TRUE;
            struct Pokemon *mon = &gPlayerParty[gDeckMons[gDeckStruct.battlerExp].partyIndex];
            u32 evoSpecies = GetEvolutionTargetSpecies(mon, EVO_MODE_BATTLE_ONLY, ITEM_NONE, NULL, &canStopEvo, CHECK_EVO);
            if (evoSpecies != gDeckMons[gDeckStruct.battlerExp].species && evoSpecies != SPECIES_NONE)
            {
                StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gDeckStruct.battlerExp].species));
                SetMonData(mon, MON_DATA_SPECIES, &evoSpecies);
                EvolutionRenameMon(mon, gDeckMons[gDeckStruct.battlerExp].species, evoSpecies);
                gDeckMons[gDeckStruct.battlerExp].species = evoSpecies;
                CalculateMonStats(mon);
                LoadBattlerObjectSprite(gDeckStruct.battlerExp);
                StartBattlerAnim(gDeckStruct.battlerExp, ANIM_ATTACK);
                gTasks[taskId].tState = 5;
                return;
            }
        }
        gTasks[taskId].tState = 6;
        break;
    case 5: // Print evolution message.
        if (gTasks[taskId].tTimer == 0)
        {
            StringCopy(gStringVar3, GetSpeciesName(gDeckMons[gDeckStruct.battlerExp].species));
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} evolved into\n{STR_VAR_3}!"));
            PrintStringToMessageBox(gStringVar1);
            PlaySE(MUS_LEVEL_UP);
            ++gTasks[taskId].tTimer;
        }
        else if (++gTasks[taskId].tTimer > 60 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 6: // Loop until all player positions have been given exp.
        if (++gDeckStruct.battlerExp > B_PLAYER_5)
            gTasks[taskId].tState = 7;
        else
            gTasks[taskId].tState = 2;
        break;
    case 7: // End battle if no caught mon.
        gTasks[taskId].tState = 0;
        if (gDeckStruct.battlerCaught != MAX_DECK_BATTLERS_COUNT)
        {
            gTasks[taskId].func = Task_HandleCaughtBattler;
        }
        else
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_CloseDeckBattle;
        }
        break;
    }
}

static void Task_HandleBattleLoss(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    default:
    case 0:
        gDeckStruct.isSelectionPhase = TRUE;
        PrintStringToMessageBox(COMPOUND_STRING("You have no more Pokémon that can fight!"));
        ++gTasks[taskId].tState;
        break;
    case 1:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2: // *TODO: check for scripted loss
    {
        u32 partyLevel = 0;
        for (enum BattleId battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
            partyLevel += gDeckMons[battler].lvl;
        RemoveMoney(&gSaveBlock1Ptr->money, 8 * partyLevel);
        ConvertIntToDecimalStringN(gStringVar2, 8 * partyLevel, STR_CONV_MODE_LEFT_ALIGN, 5);
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("You panicked and dropped ¥{STR_VAR_2}!"));
        PrintStringToMessageBox(gStringVar1);
        ++gTasks[taskId].tState;
        break;
    }
    case 3:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 4:
        PrintStringToMessageBox(COMPOUND_STRING("You were overwhelmed by your defeat!"));
        ++gTasks[taskId].tState;
    case 5:
        if (++gTasks[taskId].tTimer > 10 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 6:
        gTasks[taskId].tState = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        break;
    }
}

// Exits battle UI and sends to overworld.
void Task_CloseDeckBattle(u8 taskId)
{
    // Change transparency color to black so fade out looks better.
    u16 palette = RGB_BLACK;
    LoadPalette(&palette, BG_PLTT_ID(0), PLTT_SIZEOF(1));

    // Reset data and destroy task.
    FreeAllWindowBuffers();
    ResetSpriteData();
    UnfreezeObjectEvents();
    DestroyTask(taskId);

    // Return to overworld.
    FadeOutMapMusic(5);
    if (gBattleOutcome == B_OUTCOME_LOST)
        SetMainCallback2(CB2_WhiteOut); // *TODO: fade out looks off
    else
        SetMainCallback2(CB2_ReturnToField);
}

static const struct ListMenuItem sCaughtListMenuItems[] = 
{
    { COMPOUND_STRING("RECRUIT"),   0 },
    { COMPOUND_STRING("RELEASE"),   1 },
};

static const struct ListMenuItem sYesNoMenuItems[] = 
{
    { COMPOUND_STRING("YES"),   0 },
    { COMPOUND_STRING("NO"),   1 },
};

static const struct WindowTemplate sCaughtWindowTemplate =
{
    .bg = 1,
    .tilemapLeft = 18,
    .tilemapTop = 29,
    .width = 9,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 1 + 21*4 + 24*4,
};

static const struct WindowTemplate sYesNoWindowTemplate =
{
    .bg = 1,
    .tilemapLeft = 22,
    .tilemapTop = 29,
    .width = 5,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 1 + 21*4 + 24*4,
};

static void Task_HandleCaughtBattler(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    default:
    case 0: // Print message.
        StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gDeckStruct.battlerCaught].species));
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} wants to join\nyour team!"));
        PrintStringToMessageBox(gStringVar1);
        ++gTasks[taskId].tState;
        break;
    case 1: // Wait for message box.
        if (++gTasks[taskId].tTimer > 15 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2: // Create list menu.
    {
        struct ListMenuTemplate menuTemplate = {0};
        gDeckStruct.caughtWindowId = AddWindow(&sCaughtWindowTemplate);
        LoadMessageBoxAndBorderGfx();
        DrawStdWindowFrame(gDeckStruct.caughtWindowId, FALSE);

        menuTemplate.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
        menuTemplate.items = sCaughtListMenuItems;
        menuTemplate.totalItems = 2;
        menuTemplate.maxShowed = 2;
        menuTemplate.windowId = gDeckStruct.caughtWindowId;
        menuTemplate.item_X = 8;
        menuTemplate.upText_Y = 1;
        menuTemplate.cursorPal = 1;
        menuTemplate.fillValue = 15;
        menuTemplate.cursorShadowPal = 15;
        menuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
        menuTemplate.fontId = FONT_NORMAL;
        gTasks[taskId].data[2] = ListMenuInit(&menuTemplate, 0, 0);
        CopyWindowToVram(gDeckStruct.caughtWindowId, COPYWIN_FULL);
        CopyBgTilemapBufferToVram(1);
        ++gTasks[taskId].tState;
        break;
    }
    case 3: // Wait for input.
    {
        u32 input = ListMenu_ProcessInput(gTasks[taskId].data[2]);
        if (++gTasks[taskId].tTimer > 15 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            DestroyTask(gTasks[taskId].data[2]);
            gTasks[taskId].tTimer = 0;
            gTasks[taskId].tState += input + 1; // maybe better to use constants
        }
        else if (gTasks[taskId].tTimer > 15 && (gMain.newKeys & B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DestroyTask(gTasks[taskId].data[2]);
            gTasks[taskId].tTimer = 0;
            gTasks[taskId].tState = 6;
        }
        break;
    }
    case 4: // Party
    {
        FillWindowPixelBuffer(gDeckStruct.caughtWindowId, PIXEL_FILL(0));
        ClearStdWindowAndFrame(gDeckStruct.caughtWindowId, FALSE);
        CopyWindowToVram(gDeckStruct.caughtWindowId, COPYWIN_FULL);
        CopyBgTilemapBufferToVram(1);

        if (CalculatePlayerPartyCount() == PARTY_SIZE)
        {
            gTasks[taskId].tState = 0;
            gTasks[taskId].func = Task_SelectPartyMemberToSwap;
            return;
        }
        else
        {
            StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gDeckStruct.battlerCaught].species));
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2} joined\nyour party!"));
            PrintStringToMessageBox(gStringVar1);
        }

        struct Pokemon *mon = &gEnemyParty[gDeckMons[gDeckStruct.battlerCaught].partyIndex];
        u32 hp = gDeckMons[gDeckStruct.battlerCaught].maxHP;
        SetMonData(mon, MON_DATA_HP, &hp);
        GiveMonToPlayer(mon);
        gTasks[taskId].tState = 6; // *TODO: remove magic numbers
        break;
    }
    case 5: // Release
        FillWindowPixelBuffer(gDeckStruct.caughtWindowId, PIXEL_FILL(0));
        ClearStdWindowAndFrame(gDeckStruct.caughtWindowId, FALSE);
        CopyWindowToVram(gDeckStruct.caughtWindowId, COPYWIN_FULL);
        CopyBgTilemapBufferToVram(1);

        StringCopy(gStringVar2, GetSpeciesName(gDeckMons[gDeckStruct.battlerCaught].species));
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("You let {STR_VAR_2} go…"));
        PrintStringToMessageBox(gStringVar1);
        gTasks[taskId].tState = 6;
        break;
    case 6: // Wait for message box.
        if (++gTasks[taskId].tTimer > 15 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 7: // End battle.
        gTasks[taskId].tState = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        break;
    }
}

static void Task_SelectPartyMemberToSwap(u8 taskId)
{
    enum BattleId battler;
    switch (gTasks[taskId].tState)
    {
    default:
    case 0: // Print message.
        PrintStringToMessageBox(COMPOUND_STRING("Your party is full! Select a battler to send home."));
        ++gTasks[taskId].tState;
        break;
    case 1: // Wait for message box.
        if (++gTasks[taskId].tTimer > 15 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
            UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), TRUE);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2:
        if ((JOY_NEW(DPAD_LEFT)) // Check for a battler to move to the left.
            && gDeckStruct.selectedPos != POSITION_0)
        {
            // Deselect battler.
            PlaySE(SE_SELECT);
            UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

            // Select new battler.
            gDeckStruct.selectedPos -= 1;
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
        }
        if ((JOY_NEW(DPAD_RIGHT)) // Check for a battler to move to the right.
            && gDeckStruct.selectedPos != POSITION_5)
        {
            // Deselect battler.
            PlaySE(SE_SELECT);
            UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

            // Select new battler.
            gDeckStruct.selectedPos += 1;
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
        }
        if (JOY_NEW(A_BUTTON))
        {
            StringCopy(gStringVar2, GetSpeciesName(gDeckMons[GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos)].species));
            StringCopy(gStringVar3, GetSpeciesName(gDeckMons[gDeckStruct.battlerCaught].species));
            StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("Swap {STR_VAR_2} with {STR_VAR_3}?"));
            PrintStringToMessageBox(gStringVar1);
            ++gTasks[taskId].tState;
        }
        if (JOY_NEW(B_BUTTON))
        {
            UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);
            gTasks[taskId].tState = 5; // pick up at the end of the caught mon task
            gTasks[taskId].func = Task_HandleCaughtBattler;
            return;
        }
        break;
    case 3: // Create list menu.
    {
        struct ListMenuTemplate menuTemplate = {0};
        gDeckStruct.caughtWindowId = AddWindow(&sYesNoWindowTemplate);
        LoadMessageBoxAndBorderGfx();
        DrawStdWindowFrame(gDeckStruct.caughtWindowId, FALSE);

        menuTemplate.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
        menuTemplate.items = sYesNoMenuItems;
        menuTemplate.totalItems = 2;
        menuTemplate.maxShowed = 2;
        menuTemplate.windowId = gDeckStruct.caughtWindowId;
        menuTemplate.item_X = 8;
        menuTemplate.upText_Y = 1;
        menuTemplate.cursorPal = 1;
        menuTemplate.fillValue = 15;
        menuTemplate.cursorShadowPal = 15;
        menuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
        menuTemplate.fontId = FONT_NORMAL;
        gTasks[taskId].data[2] = ListMenuInit(&menuTemplate, 0, 0);
        CopyWindowToVram(gDeckStruct.caughtWindowId, COPYWIN_FULL);
        CopyBgTilemapBufferToVram(1);
        ++gTasks[taskId].tState;
        break;
    }
    case 4: // Wait for input.
    {
        u32 input = ListMenu_ProcessInput(gTasks[taskId].data[2]);
        if (++gTasks[taskId].tTimer > 15 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            DestroyTask(gTasks[taskId].data[2]);
            FillWindowPixelBuffer(gDeckStruct.caughtWindowId, PIXEL_FILL(0));
            ClearStdWindowAndFrame(gDeckStruct.caughtWindowId, FALSE);
            CopyWindowToVram(gDeckStruct.caughtWindowId, COPYWIN_FULL);
            CopyBgTilemapBufferToVram(1);

            gTasks[taskId].tTimer = 0;
            if (input == 0)
            {
                gTasks[taskId].tState = 5;
            }
            else
            {
                PrintStringToMessageBox(COMPOUND_STRING("Your party is full! Select a battler to send home."));
                gTasks[taskId].tState = 2;
            }
        }
        else if (gTasks[taskId].tTimer > 15 && (gMain.newKeys & B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DestroyTask(gTasks[taskId].data[2]);
            FillWindowPixelBuffer(gDeckStruct.caughtWindowId, PIXEL_FILL(0));
            ClearStdWindowAndFrame(gDeckStruct.caughtWindowId, FALSE);
            CopyWindowToVram(gDeckStruct.caughtWindowId, COPYWIN_FULL);
            CopyBgTilemapBufferToVram(1);

            gTasks[taskId].tTimer = 0;
            gTasks[taskId].tState = 2;
            PrintStringToMessageBox(COMPOUND_STRING("Your party is full! Select a battler to send home."));
        }
        break;
    }
    case 5: // Update party data.
    {
        u32 battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        StringCopy(gStringVar2, GetSpeciesName(gDeckMons[battler].species));
        StringCopy(gStringVar3, GetSpeciesName(gDeckMons[gDeckStruct.battlerCaught].species));
        StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("You recruited {STR_VAR_2} and sent {STR_VAR_3} home."));
        PrintStringToMessageBox(gStringVar1);

        CpuCopy32(&gEnemyParty[gDeckMons[gDeckStruct.battlerCaught].partyIndex], &gPlayerParty[gDeckMons[battler].partyIndex], sizeof(struct Pokemon));
        gDeckMons[battler].species = gDeckMons[gDeckStruct.battlerCaught].species;
        SetMonData(&gPlayerParty[gDeckMons[battler].partyIndex], MON_DATA_POSITION, &gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, FALSE);
        LoadBattlerObjectSprite(battler);
        StartBattlerAnim(battler, ANIM_ATTACK);
        gTasks[taskId].tState = 6;
        break;
    }
    case 6: // Wait for message box.
        if (++gTasks[taskId].tTimer > 60 && (gMain.newKeys & A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tTimer = 0;
            gTasks[taskId].tState = 7;
        }
        break;
    case 7: // End battle.
        gTasks[taskId].tState = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_CloseDeckBattle;
        break;
    }
}

// Calculate end of turn fatigue damage.
static u32 GetTurnEndFatigueDamage(u32 turns)
{
    if (turns < 10)
        return 0;
    if (turns < 12)
        return 10;
    if (turns < 14)
        return 20;
    if (turns < 18)
        return 50;
    else
        return 100;
}

// Execute any turn end effects (e.g., poison, fatigue, sleep).
static void Task_HandleTurnEndEffects(u8 taskId)
{
    u32 damage = 0;
    switch (gTasks[taskId].tTurnEndState)
    {
    case TURN_END_CHECK_FATIGUE:
        if (gDeckStruct.turns >= 10)
        {
            PrintStringToMessageBox(COMPOUND_STRING("Battlers are weakened by\nfatigue…"));
            ++gTasks[taskId].tTurnEndState;
            return;
        }
        else
        {
            gTasks[taskId].tTurnEndState = TURN_END_POISON;
        }
        break;
    case TURN_END_FATIGUE:
        if (++gTasks[taskId].tTimer > 10 && JOY_NEW(A_BUTTON))
        {
            for (u32 battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
            {
                if (IsDeckBattlerAlive(battler))
                {
                    StartBattlerAnim(battler, ANIM_HURT);
                    damage = GetTurnEndFatigueDamage(gDeckStruct.turns);
                    UpdateBattlerHP(battler, damage);
                }
            }
            PlaySE(SE_EFFECTIVE);
            gTasks[taskId].tTimer = 0;
            gTasks[taskId].tTurnEndState = TURN_END_PAUSE;
            gTasks[taskId].tState = TURN_END_POISON; // this is a bug waiting to happen
            gTasks[taskId].func = Task_CheckFaintAndContinue;
        }
        break;
    case TURN_END_POISON: // TODO
        ++gTasks[taskId].tTurnEndState;
        break;
    case TURN_END_SLEEP: // TODO
        ++gTasks[taskId].tTurnEndState;
        break;
    case TURN_END_RESET_STATUS: // TODO
        ++gTasks[taskId].tTurnEndState;
        break;
    default:
    case TURN_END_COMPLETED: // TODO
        ResetTurnValues();
        gDeckStruct.turns++;
        gDeckStruct.actingSide ^= 1; // get opposite side
        gDeckStruct.isSelectionPhase = TRUE;
        gTasks[taskId].tState = 0;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tTurnEndState = 0;
        if (gDeckStruct.actingSide == B_SIDE_PLAYER)
        {
            // Return to auto battle if enabled.
            if (gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_AUTO)
            {
                SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
                SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
                gTasks[taskId].func = Task_AutoSelectAction;
            }
            // Or set up UI for action selection.
            else
            {
                gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
                u32 battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
                UpdateBattlerSelection(battler, TRUE);
                DisplayActionSelectionInfo(battler);

                SetBattlerBobPause(FALSE);
                SetBattlerPortraitVisibility(TRUE);
                SetGpuReg(REG_OFFSET_BG0VOFS, 0);
                SetGpuReg(REG_OFFSET_BG1VOFS, 0);
                gTasks[taskId].func = Task_PlayerSelectAction;
            }
        }
        else
        {
            gTasks[taskId].func = Task_OpponentSelectAction;
        }
        break;
    case TURN_END_PAUSE: // Allow enough time for animations to reset.
        if (++gTasks[taskId].tTimer >= 60)
            gTasks[taskId].tTurnEndState = gTasks[taskId].tState;
        break;
    }
}

#undef tState
#undef tTimer
#undef tTurnEndState

// Initialize gDeckStruct to start battle with clean data.
static void InitBattleStructData(void)
{
    ResetTurnValues();

    gDeckStruct.turns = 0;
    gDeckStruct.exp = 0;
    gDeckStruct.battlerCaught = MAX_DECK_BATTLERS_COUNT;
    gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
}

// Reset struct data associated with a single turn.
static void ResetTurnValues(void)
{
    for (enum BattleId battler = 0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
    {
        gDeckMons[battler].hasMoved = FALSE;
        gDeckMons[battler].hasSwapped = FALSE;
        gDeckMons[battler].initialPos = gDeckMons[battler].pos;
        gDeckMons[battler].powerBoost = 0;
    }
    gDeckStruct.actionsCount = 0;
    gDeckStruct.executedCount = 0;
}

// Populate enemy party with dummy data.
void LoadDummyEnemyParty(void)
{
    CreateMon(&gEnemyParty[0], SPECIES_BELLSPROUT, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[1], SPECIES_SLOWPOKE, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[2], SPECIES_SLOWBRO, 8, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[3], SPECIES_APPLIN, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[4], SPECIES_BELLSPROUT, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);
    CreateMon(&gEnemyParty[5], SPECIES_SLOWPOKE, 5, USE_RANDOM_IVS, 0, 0, OT_ID_PLAYER_ID, 0);

    u32 pos = 0;
    SetMonData(&gEnemyParty[0], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[1], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[2], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[3], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[4], MON_DATA_POSITION, &pos); pos++;
    SetMonData(&gEnemyParty[5], MON_DATA_POSITION, &pos);
}

// Load party data into gDeckMons struct.
static void InitBattleMonData(void)
{
    struct Pokemon *mon;
    for (u32 i = 0; i < MAX_DECK_BATTLERS_COUNT; ++i)
    {
        if (GetDeckBattlerSide(i) == B_SIDE_PLAYER)
        {
            mon = &gPlayerParty[i];
            gDeckMons[i].partyIndex = i;
        }
        else
        {
            mon = &gEnemyParty[i - POSITIONS_COUNT];
            gDeckMons[i].partyIndex = i - POSITIONS_COUNT;
        }
        gDeckMons[i].species = GetMonData(mon, MON_DATA_SPECIES);
        gDeckMons[i].lvl = GetMonData(mon, MON_DATA_LEVEL);
        gDeckMons[i].hp = GetMonData(mon, MON_DATA_HP);
        gDeckMons[i].maxHP = GetMonData(mon, MON_DATA_MAX_HP);
        gDeckMons[i].power = GetMonData(mon, MON_DATA_ATK);
        gDeckMons[i].def = GetMonData(mon, MON_DATA_DEF);
        gDeckMons[i].pos = GetMonData(mon, MON_DATA_POSITION);
        gDeckMons[i].initialPos = gDeckMons[i].pos;
    }
}

// Set up data for a queued action.
void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move)
{
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].type = type;
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].attacker = battlerAtk;
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].target = battlerDef;
    gDeckStruct.queuedActions[gDeckStruct.actionsCount].move = move;

    if (type == ACTION_SWAP)
    {
        gDeckMons[battlerAtk].initialPos = gDeckMons[battlerAtk].pos;
        gDeckMons[battlerDef].initialPos = gDeckMons[battlerDef].pos;
    }

    ++gDeckStruct.actionsCount;
}

// Update data required to swap two battler positions.
void SwapBattlerPositions(u32 battler1, u32 battler2)
{
    u32 temp;
    SWAP(gDeckMons[battler1].pos, gDeckMons[battler2].pos, temp);
    GetBattlerSprite(battler1)->x = GetBattlerXCoord(battler1);
    GetBattlerSprite(battler2)->x = GetBattlerXCoord(battler2);
    gDeckMons[battler1].hasSwapped = TRUE;
    gDeckMons[battler2].hasSwapped = TRUE;
}

// Performs basic damage calc formula using two battler IDs and a move.
s32 CalculateDamage(u32 battlerAtk, u32 battlerDef, u32 move)
{
    u32 movePower = gDeckMovesInfo[move].power;
    u32 level = 50;
    u32 power = gDeckMons[battlerAtk].power + gDeckMons[battlerAtk].powerBoost;
    u32 defense = gDeckMons[battlerDef].def;

    s32 dmg = movePower * power * (2 * level / 5 + 2) / defense / 50 + 2;
    dmg *= DMG_ROLL_PERCENT_HI - RandomUniform(RNG_DAMAGE_MODIFIER, 0, DMG_ROLL_PERCENT_HI - DMG_ROLL_PERCENT_LO);
    dmg /= 100;

    if (dmg != 0)
        return dmg;
    else
        return 1;
}

// Performs the HP change when a battler is hurt or healed.
void UpdateBattlerHP(u32 battler, s32 damage)
{
    if (damage < -999) // cap damage at 3 digits
        damage = -999;

    if (damage > gDeckMons[battler].hp) // correctly bound HP
        gDeckMons[battler].hp = 0;
    else if (-damage > gDeckMons[battler].maxHP - gDeckMons[battler].hp)
        gDeckMons[battler].hp = gDeckMons[battler].maxHP;
    else
        gDeckMons[battler].hp -= damage;

    PrintDamageNumbers(battler, damage);
}

u32 GetBattleSpeedScale(void)
{
    // Don't speed up during selection phase or if holding L.
    if (JOY_HELD(L_BUTTON) || gDeckStruct.isSelectionPhase)
        return 1;
    
    switch (VarGet(VAR_BATTLE_SPEED))
    {
    default:
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 4;
    }
}
