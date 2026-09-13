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
#include "field_weather.h"
#include "gpu_regs.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/rgb.h"

/* deck_battle_controller.c
 *
 * This file handles all the player input processing for
 * action selection. Move target types can each have their
 * own selection function to allow for greater flexibility.
 * 
*/

static void Task_PlayerSelectAllyToSwap(u8 taskId);
static void Task_PlayerSelectSingleOpponent(u8 taskId);
static void Task_PlayerSelectSingleAlly(u8 taskId);
static void Task_PlayerDisplayTargets(u8 taskId);
static void Task_PlayerTryToRun(u8 taskId);
static void Task_RunAwayFailed(u8 taskId);
static void Task_RunAwaySuccessful(u8 taskId);

#define tState  data[0]
#define tTimer  data[1]

void Task_PlayerSelectAction(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT) // Check for a battler to move to the left.
        && (pos = GetToMoveOnLeft(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayActionSelectionInfo(battler);
    }
    if ((gMain.newKeys & DPAD_RIGHT) // Check for a battler to move to the right.
        && (pos = GetToMoveOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        DisplayActionSelectionInfo(battler);
    }
    if (gMain.newKeys & A_BUTTON) // Choose target to attack.
    {
        // Remove cursor but keep idle anim while selecting target.
        PlaySE(SE_SELECT);
        gBattlerAttacker = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        RemoveSelectionCursorOverBattler(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);

        // Set up UI for targeting.
        SetBattlerPortraitVisibility(FALSE);
        SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
        SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
        // gTasks[taskId].func = sPlayerMoveTargetTasks[gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move].target];
        if (gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move].target & TARGET_SINGLE_OPPONENT)
            gTasks[taskId].func = Task_PlayerSelectSingleOpponent;
        else if (gDeckMovesInfo[gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move].target & TARGET_SINGLE_ALLY)
            gTasks[taskId].func = Task_PlayerSelectSingleAlly;
        else
            gTasks[taskId].func = Task_PlayerDisplayTargets;
    }
    if (gMain.newKeys & START_BUTTON) // Choose target to swap.
    {
        gBattlerAttacker = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (!gDeckMons[gBattlerAttacker].hasSwapped)
        {
            // Select leftmost position.
            if (gDeckStruct.selectedPos == POSITION_0)
                gDeckStruct.selectedPos = POSITION_1;
            else
                gDeckStruct.selectedPos = POSITION_0;

            // Remove cursor but keep idle anim while selecting target.
            RemoveSelectionCursorOverBattler(gBattlerAttacker);
            GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_BLEND;

            // Mark target as selected.
            PlaySE(SE_SELECT);
            CreateSelectionCursorOverPosition(gDeckStruct.selectedPos);
            DisplaySwapSelectionInfo(gDeckStruct.selectedPos);
            
            // Set up UI for targeting.
            SetBattlerPortraitVisibility(FALSE);
            SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
            SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
            gTasks[taskId].func = Task_PlayerSelectAllyToSwap;
            return;
        }
        // Swap not possible.
        PlaySE(SE_FAILURE);
    }
    if ((gMain.newKeys & B_BUTTON) && gDeckStruct.actionsCount != 0)
    {
        PlaySE(SE_SELECT);
        struct BattleAction *action = &gDeckStruct.queuedActions[gDeckStruct.actionsCount - 1];
        if (action->type == ACTION_SWAP)
        {
            SwapBattlerPositions(action->attacker, action->target);
            GetBattlerSprite(action->attacker)->oam.objMode = ST_OAM_OBJ_NORMAL;
            GetBattlerSprite(action->target)->oam.objMode = ST_OAM_OBJ_NORMAL;
            gDeckMons[action->attacker].hasSwapped = FALSE;
            gDeckMons[action->target].hasSwapped = FALSE;

            if (gDeckMons[action->attacker].pos == gDeckStruct.selectedPos || gDeckMons[action->target].pos == gDeckStruct.selectedPos)
            {
                UpdateBattlerSelection(action->attacker, FALSE);
                UpdateBattlerSelection(action->target, FALSE);
                UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), TRUE);
                DisplayActionSelectionInfo(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos));
            }
            --gDeckStruct.actionsCount;
        }
        else
        {
            SetBattlerGrayscale(action->attacker, FALSE);
            gDeckMons[action->attacker].hasMoved = FALSE;
            --gDeckStruct.actionsCount;
        }
    }
    if (JOY_NEW(L_BUTTON))
    {
        if (gDeckStruct.isBossBattle || gDeckStruct.actionsCount > 0)
        {
            PlaySE(SE_FAILURE);
        }
        else
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].func = Task_PlayerTryToRun;
        }
    }
}

static void Task_PlayerTryToRun(u8 taskId)
{
    u32 playerPartyLevel = 0;
    u32 enemyPartyLevel = 0;

    for (u32 i = 0; i < PARTY_SIZE; ++i)
    {
        if (gDeckMons[i].hp != 0)
            playerPartyLevel += gDeckMons[i].lvl;
        if (gDeckMons[i+PARTY_SIZE].hp != 0)
            enemyPartyLevel += gDeckMons[i+PARTY_SIZE].lvl;
    }

    if (playerPartyLevel > enemyPartyLevel + 5)
        gTasks[taskId].func = Task_RunAwaySuccessful;
    else if (playerPartyLevel >= enemyPartyLevel && (Random() % 100) > 80)
        gTasks[taskId].func = Task_RunAwaySuccessful;
    else if ((Random() % 100) > 40)
        gTasks[taskId].func = Task_RunAwaySuccessful;
    else
        gTasks[taskId].func = Task_RunAwayFailed;        
}

static void Task_RunAwaySuccessful(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
        default:
        case 0:
            PrintStringToMessageBox(COMPOUND_STRING(""));
            ++gTasks[taskId].tState;
            break;
        case 1:
            PrintStringToMessageBox(COMPOUND_STRING("You ran away!"));
            RemoveSelectionCursorOverBattler(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos));
            SetBattlerPortraitVisibility(FALSE);
            SetBattlerBobPause(TRUE);
            ++gTasks[taskId].tState;
            break;
        case 2:
            SetBattlerPortraitVisibility(FALSE);
            SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
            SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
            ++gTasks[taskId].tState;
            break;
        case 3:
            if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
            {
                PlaySE(SE_FLEE);
                gTasks[taskId].func = Task_CloseDeckBattle;
                FadeScreen(FADE_TO_BLACK, 0);
            }
            break;
    }
}

static void Task_RunAwayFailed(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
        default:
        case 0:
            PrintStringToMessageBox(COMPOUND_STRING(""));
            ++gTasks[taskId].tState;
            break;
        case 1:
            PrintStringToMessageBox(COMPOUND_STRING("You couldn't run away!"));
            RemoveSelectionCursorOverBattler(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos));
            StartBattlerAnim(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), ANIM_PAUSED);
            SetBattlerBobPause(TRUE);
            ++gTasks[taskId].tState;
            break;
        case 2:
            SetBattlerPortraitVisibility(FALSE);
            SetGpuReg(REG_OFFSET_BG0VOFS, DISPLAY_HEIGHT);
            SetGpuReg(REG_OFFSET_BG1VOFS, DISPLAY_HEIGHT);
            ++gTasks[taskId].tState;
            break;
        case 3:
            if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
            {
                PlaySE(SE_SELECT);
                gTasks[taskId].tState = 0;
                gTasks[taskId].func = Task_PrepareForActionPhase;
            }
            break;
    }
}

static void Task_PlayerSelectAllyToSwap(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if ((gMain.newKeys & DPAD_LEFT)
        && (pos = GetAnyNonAttackerOnLeft(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (IsDeckBattlerAlive(battler))
            StartBattlerAnim(battler, ANIM_PAUSED);
        RemoveSwapSelectionCursor();

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (IsDeckBattlerAlive(battler))
            StartBattlerAnim(battler, ANIM_IDLE);
        CreateSelectionCursorOverPosition(gDeckStruct.selectedPos);
        DisplaySwapSelectionInfo(gDeckStruct.selectedPos);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetAnyNonAttackerOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (IsDeckBattlerAlive(battler))
            StartBattlerAnim(battler, ANIM_PAUSED);
        RemoveSwapSelectionCursor();

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (IsDeckBattlerAlive(battler))
            StartBattlerAnim(battler, ANIM_IDLE);
        CreateSelectionCursorOverPosition(gDeckStruct.selectedPos);
        DisplaySwapSelectionInfo(gDeckStruct.selectedPos);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target battler.
        PlaySE(SE_SELECT);
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (IsDeckBattlerAlive(battler))
            StartBattlerAnim(battler, ANIM_PAUSED);
        RemoveSwapSelectionCursor();

        // Reselect acting battler.
        UpdateBattlerSelection(gBattlerAttacker, TRUE);
        DisplayActionSelectionInfo(gBattlerAttacker);
        GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_NORMAL;
        gDeckStruct.selectedPos = gDeckMons[gBattlerAttacker].pos;     

        // Set up UI for action selection.
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
    }
    if ((gMain.newKeys & A_BUTTON) || (gMain.newKeys & START_BUTTON))
    {
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        if (!IsDeckBattlerAlive(gBattlerTarget))
        {
            // Deselect battler.
            PlaySE(SE_SELECT);
            RemoveSwapSelectionCursor();
            
            // Swap any fainted battlers.
            gBattlerTarget = GetDeckBattlerAtPosUnsafe(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            gDeckMons[gBattlerTarget].pos = gDeckMons[gBattlerAttacker].pos;
            gDeckMons[gBattlerTarget].initialPos = gDeckMons[gBattlerAttacker].pos;

            // Immediately execute swap without cost.
            gDeckMons[gBattlerAttacker].pos = gDeckStruct.selectedPos;
            gDeckMons[gBattlerAttacker].initialPos = gDeckStruct.selectedPos;
            GetBattlerSprite(gBattlerAttacker)->x = GetBattlerXCoord(gBattlerAttacker);
            StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);
            GetBattlerSprite(gBattlerAttacker)->oam.objMode = ST_OAM_OBJ_NORMAL;

            // Prepare to select next battler for action.
            gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
            gBattlerAttacker = battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

            // Set up UI for action selection.
            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else if (!gDeckMons[gBattlerTarget].hasSwapped)
        {
            // Deselect battler and mark as swapped with transparency.
            PlaySE(SE_SELECT);
            StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);
            RemoveSwapSelectionCursor();
            GetBattlerSprite(gBattlerTarget)->oam.objMode = ST_OAM_OBJ_BLEND;

            // Queue swap action.
            QueueAction(ACTION_SWAP, gBattlerAttacker, gBattlerTarget, MOVE_NONE);
            SwapBattlerPositions(gBattlerAttacker, gBattlerTarget);
            StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);
            StartBattlerAnim(gBattlerTarget, ANIM_PAUSED);

            // Prepare to select next battler for action.
            gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

            // Set up UI for action selection.
            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else
        {
            PlaySE(SE_FAILURE);
        }
    }
}

static void Task_PlayerSelectSingleOpponent(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if (gTasks[taskId].tState == 0)
    {
        // Display first possible target.
        gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_OPPONENT);
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        PrintTargetBattlerPrompt(battler);
        ++gTasks[taskId].tState;
    }
    if ((gMain.newKeys & DPAD_LEFT)
        && (pos = GetOccupiedOnLeft(B_SIDE_OPPONENT, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        PrintTargetBattlerPrompt(battler);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetOccupiedOnRight(B_SIDE_OPPONENT, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        PrintTargetBattlerPrompt(battler);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos), FALSE);

        // Reselect acting battler.
        UpdateBattlerSelection(gBattlerAttacker, TRUE);
        DisplayActionSelectionInfo(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, FALSE);
        gDeckStruct.selectedPos = gDeckMons[gBattlerAttacker].pos;

        // Set up UI for action selection.
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
        gTasks[taskId].tState = 0;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_OPPONENT, gDeckStruct.selectedPos);
        UpdateBattlerSelection(gBattlerTarget, FALSE);

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, gBattlerTarget, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        gTasks[taskId].tState = 0;
        if (gDeckStruct.selectedPos != POSITIONS_COUNT)
        {
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else
        {
            gTasks[taskId].func = Task_PrepareForActionPhase; 
        }
    }
}

static void Task_PlayerSelectSingleAlly(u8 taskId)
{
    enum BattleId battler;
    enum BattlePosition pos;
    if (gTasks[taskId].tState == 0)
    {
        // Display first possible target.
        gDeckStruct.selectedPos = GetLeftmostOccupiedPosition(B_SIDE_PLAYER);
        if (gDeckStruct.selectedPos == gDeckMons[gBattlerAttacker].pos)
            gDeckStruct.selectedPos = GetNonAttackerOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        PrintTargetBattlerPrompt(battler);
        ++gTasks[taskId].tState;
    }
    if ((gMain.newKeys & DPAD_LEFT)
        && (pos = GetNonAttackerOnLeft(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        PrintTargetBattlerPrompt(battler);
    }
    if ((gMain.newKeys & DPAD_RIGHT)
        && (pos = GetNonAttackerOnRight(B_SIDE_PLAYER, gDeckStruct.selectedPos)) != POSITIONS_COUNT)
    {
        // Deselect battler.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Select new battler.
        gDeckStruct.selectedPos = pos;
        battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(battler, TRUE);
        PrintTargetBattlerPrompt(battler);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        UpdateBattlerSelection(GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos), FALSE);

        // Reselect acting battler.
        UpdateBattlerSelection(gBattlerAttacker, TRUE);
        DisplayActionSelectionInfo(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, FALSE);
        gDeckStruct.selectedPos = gDeckMons[gBattlerAttacker].pos;

        // Set up UI for action selection.
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
        gTasks[taskId].tState = 0;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect target.
        PlaySE(SE_SELECT);
        gBattlerTarget = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        UpdateBattlerSelection(gBattlerTarget, FALSE);

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, gBattlerTarget, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        gTasks[taskId].tState = 0;
        if (gDeckStruct.selectedPos != POSITIONS_COUNT)
        {
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else
        {
            gTasks[taskId].func = Task_PrepareForActionPhase; 
        }
    }
}

static void UpdateDisplayedTargetsSelection(bool32 selected)
{
    // Load targets to display.
    gCurrentMove = gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move;
    u32 targetsCount = 0;
    u32 aliveCount = 0;
    enum BattleId targets[MAX_DECK_BATTLERS_COUNT] = {0};
    PopulateTargetsList(targets, &targetsCount);

    // Place cursors over targets.
    for (u32 i = 0; i < targetsCount; ++i)
    {
        if (IsDeckBattlerAlive(targets[i]))
        {
            UpdateBattlerSelection(targets[i], selected);
            aliveCount += 1;
        }
    }

    if (selected)
        PrintFixedTargetsPrompt(aliveCount > 0);
}

static void Task_PlayerDisplayTargets(u8 taskId)
{
    enum BattleId battler;

    if (gTasks[taskId].tState == 0)
    {
        UpdateDisplayedTargetsSelection(TRUE);
        ++gTasks[taskId].tState;
    }

    if (gMain.newKeys & B_BUTTON)
    {
        // Deselect targets.
        UpdateDisplayedTargetsSelection(FALSE);

        // Reselect acting battler.
        UpdateBattlerSelection(gBattlerAttacker, TRUE);
        DisplayActionSelectionInfo(gBattlerAttacker);
        SetBattlerGrayscale(gBattlerAttacker, FALSE);
        gDeckStruct.selectedPos = gDeckMons[gBattlerAttacker].pos;

        // Set up UI for action selection.
        SetBattlerPortraitVisibility(TRUE);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        gTasks[taskId].func = Task_PlayerSelectAction;
        gTasks[taskId].tState = 0;
    }
    if (gMain.newKeys & A_BUTTON)
    {
        // Deselect targets.
        UpdateDisplayedTargetsSelection(FALSE);

        // Queue attack action and update data.
        QueueAction(ACTION_ATTACK, gBattlerAttacker, MAX_DECK_BATTLERS_COUNT, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        SetBattlerGrayscale(gBattlerAttacker, TRUE);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
        StartBattlerAnim(gBattlerAttacker, ANIM_PAUSED);

        // Select next battler for action selection or begin action phase.
        gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
        gTasks[taskId].tState = 0;
        if (gDeckStruct.selectedPos != POSITIONS_COUNT)
        {
            battler = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
            UpdateBattlerSelection(battler, TRUE);
            DisplayActionSelectionInfo(battler);

            SetBattlerPortraitVisibility(TRUE);
            SetGpuReg(REG_OFFSET_BG0VOFS, 0);
            SetGpuReg(REG_OFFSET_BG1VOFS, 0);
            gTasks[taskId].func = Task_PlayerSelectAction;
        }
        else
        {
            gTasks[taskId].func = Task_PrepareForActionPhase; 
        }
    }
}

void Task_AutoSelectAction(u8 taskId)
{
    gDeckStruct.selectedPos = GetLeftmostPositionToMove(B_SIDE_PLAYER);
    if (gDeckStruct.selectedPos != POSITIONS_COUNT)
    {
        gBattlerAttacker = GetDeckBattlerAtPos(B_SIDE_PLAYER, gDeckStruct.selectedPos);
        QueueAction(ACTION_ATTACK, gBattlerAttacker, MAX_DECK_BATTLERS_COUNT, gDeckSpeciesInfo[gDeckMons[gBattlerAttacker].species].move);
        gDeckMons[gBattlerAttacker].hasMoved = TRUE;
    }
    else
    {
        gTasks[taskId].func = Task_PrepareForActionPhase;
    } 
}

#undef tState
#undef tTimer
