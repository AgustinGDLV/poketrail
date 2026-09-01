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

/* deck_battle_interface.c
 *
 * This file holds the handling for all move execution and
 * animation tasks. The two are currently interwoven, but
 * should be fairly easy to copy for new move effects.
 * 
*/

static void ExecuteHitEffect(void);
static void ExecuteStatChangeEffect(void);
static void ExecuteHealEffect(void);

void (*const gMoveEffectFuncs[DECK_EFFECT_COUNT])(void) =
{
    [DECK_EFFECT_HIT]       = ExecuteHitEffect,
    [DECK_EFFECT_POWER_UP]  = ExecuteStatChangeEffect,
    [DECK_EFFECT_HEAL]      = ExecuteHealEffect,
};

#define tState  data[0]
#define tTimer  data[1]

static void ExecuteHitEffect(void)
{
    s32 damage = 0;
    u32 targetsCount = 0;
    u32 aliveCount = 0;
    enum BattleId targets[MAX_DECK_BATTLERS_COUNT] = {0};
    PopulateTargetsList(targets, &targetsCount);

    // Execute effect.
    for (u32 i = 0; i < targetsCount; ++i)
    {
        if (IsDeckBattlerAlive(targets[i]))
        {
            gBattlerTarget = targets[i];
            StartBattlerAnim(targets[i], ANIM_HURT);
            gDeckStruct.lastHitDamage = damage = CalculateDamage(gBattlerAttacker, targets[i], gCurrentMove);
            UpdateBattlerHP(targets[i], damage);
            aliveCount += 1;
        }
    }

    // Print string.
    if (aliveCount > 0)
    {
        PrintMoveOutcomeString(aliveCount);
        PlaySE(SE_EFFECTIVE);
    }
    else
    {
        PrintStringToMessageBox(COMPOUND_STRING("But it failed…"));
    }
}

static void ExecuteStatChangeEffect(void)
{
    u32 targetsCount = 0;
    u32 aliveCount = 0;
    enum BattleId targets[MAX_DECK_BATTLERS_COUNT] = {0};
    PopulateTargetsList(targets, &targetsCount);

    // Execute effect.
    for (u32 i = 0; i < targetsCount; ++i)
    {
        if (IsDeckBattlerAlive(targets[i]))
        {
            gBattlerTarget = targets[i];
            StartBattlerAnim(targets[i], ANIM_STAT_CHANGE);
            if (gDeckMovesInfo[gCurrentMove].param == STAT_DEF)
                gDeckMons[targets[i]].defBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
            else
                gDeckMons[targets[i]].powerBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
            aliveCount += 1;
        }
    }

    // Print string.
    if (aliveCount > 0)
    {
        PrintMoveOutcomeString(aliveCount);
        PlaySE(SE_M_STAT_INCREASE);
    }
    else
    {
        PrintStringToMessageBox(COMPOUND_STRING("But it failed…"));
    }
}

static void ExecuteHealEffect(void)
{
    s32 damage = 0;
    u32 targetsCount = 0;
    u32 aliveCount = 0;
    enum BattleId targets[MAX_DECK_BATTLERS_COUNT] = {0};
    PopulateTargetsList(targets, &targetsCount);

    // Execute effect.
    for (u32 i = 0; i < targetsCount; ++i)
    {
        if (IsDeckBattlerAlive(targets[i]))
        {
            gBattlerTarget = targets[i];
            StartBattlerAnim(targets[i], ANIM_STAT_CHANGE);
            gDeckStruct.lastHitDamage = damage = -(gDeckMons[targets[i]].maxHP * gDeckMovesInfo[gCurrentMove].power) / 100;
            UpdateBattlerHP(targets[i], damage);
            aliveCount += 1;
        }
    }

    // Print string.
    if (aliveCount > 0)
    {
        PrintMoveOutcomeString(aliveCount);
        PlaySE(SE_M_ABSORB_2);
    }
    else
    {
        PrintStringToMessageBox(COMPOUND_STRING("But it failed…"));
    }
}

void Task_ExecuteMove(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    case 0: // Do attack animation.
        SetBattlerGrayscale(gBattlerAttacker, FALSE);
        StartBattlerAnim(gBattlerAttacker, ANIM_ATTACK);
        PrintMoveUseString();
        ++gTasks[taskId].tState;
        break;
    case 1: // Wait for attack animation to execute damage.
        if (HasBattlerAnimTriggeredCry(gBattlerAttacker) || gSaveBlock2Ptr->optionsBattleSceneOff)
            ++gTasks[taskId].tState;
        break;
    case 2: // Execute move effect.
        gMoveEffectFuncs[gDeckMovesInfo[gCurrentMove].effect]();
        ++gTasks[taskId].tState;
        break;
    case 3: // Wait for animations.
        if (++gTasks[taskId].tTimer >= 60)
            ++gTasks[taskId].tState;
        break;
    case 4: // Check for fainted battlers.
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_CheckFaintAndContinue;
        break;
    }
}

void Task_ExecuteSwap(u8 taskId)
{
    switch (gTasks[taskId].tState)
    {
    case 0: // Do attack animation.
        StartBattlerAnim(gBattlerAttacker, ANIM_ATTACK);
        StartBattlerAnim(gBattlerTarget, ANIM_ATTACK);
        PrintSwapString(gBattlerAttacker, gBattlerTarget);
        ++gTasks[taskId].tState;
        break;
    case 1: // Wait for attack animation to execute damage.
        if (HasBattlerAnimTriggeredCry(gBattlerAttacker) || gSaveBlock2Ptr->optionsBattleSceneOff)
        {
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 2: // Swap battler positions.
        if (++gTasks[taskId].tTimer > 8)
        {
            PlaySE(SE_M_DOUBLE_TEAM);
            SwapBattlerPositions(gBattlerAttacker, gBattlerTarget);
            gTasks[taskId].tTimer = 0;
            ++gTasks[taskId].tState;
        }
        break;
    case 3: // Wait for animation.
        if (++gTasks[taskId].tTimer >= 60)
            ++gTasks[taskId].tState;
        break;
    case 4:
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_ExecuteQueuedActionOrEnd;
        break;
    }
}

#undef tState
#undef tTimer
