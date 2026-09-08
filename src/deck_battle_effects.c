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
static void ExecuteSwapEffect(void);

void (*const gMoveEffectFuncs[DECK_EFFECT_COUNT])(void) =
{
    [DECK_EFFECT_HIT]       = ExecuteHitEffect,
    [DECK_EFFECT_POWER_UP]  = ExecuteStatChangeEffect,
    [DECK_EFFECT_HEAL]      = ExecuteHealEffect,
    [DECK_EFFECT_SWAP]      = ExecuteSwapEffect,
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
            if (targets[i] == gBattlerAttacker) // recoil
                damage /= 3;
            UpdateBattlerHP(targets[i], damage);
            aliveCount += 1;
        }
    }

    // Apply relevant abilities.
    if (GetDeckBattlerAbility(gBattlerAttacker) == DECK_VAMPIRIC)
    {
        s32 heal = -1 * (s32) (gDeckStruct.lastHitDamage / 3);
        UpdateBattlerHP(gBattlerAttacker, heal);
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

static void ApplyStatChange(u32 attacker, u32 target, u32 stat)
{
    if (stat == STAT_ATK || stat == 0xFF)
    {
        if (GetDeckBattlerSide(attacker) != GetDeckBattlerSide(target))
            gDeckMons[target].defBoost -= ((gDeckMons[gBattlerAttacker].power + gDeckMons[gBattlerAttacker].powerBoost) * gDeckMovesInfo[gCurrentMove].power) / 100;
        else
            gDeckMons[target].defBoost += ((gDeckMons[gBattlerAttacker].power + gDeckMons[gBattlerAttacker].powerBoost) * gDeckMovesInfo[gCurrentMove].power) / 100;
    }
    if (stat == STAT_DEF || stat == 0xFF)
    {
        if (GetDeckBattlerSide(attacker) != GetDeckBattlerSide(target))
            gDeckMons[target].powerBoost -= ((gDeckMons[gBattlerAttacker].power + gDeckMons[gBattlerAttacker].powerBoost) * gDeckMovesInfo[gCurrentMove].power) / 100;
        else
            gDeckMons[target].powerBoost += ((gDeckMons[gBattlerAttacker].power + gDeckMons[gBattlerAttacker].powerBoost) * gDeckMovesInfo[gCurrentMove].power) / 100;
    }
}

static void ExecuteStatChangeEffect(void)
{
    u32 targetsCount = 0;
    u32 aliveCount = 0;
    u32 negative = FALSE;
    enum BattleId targets[MAX_DECK_BATTLERS_COUNT] = {0};
    PopulateTargetsList(targets, &targetsCount);

    // Execute effect.
    for (u32 i = 0; i < targetsCount; ++i)
    {
        if (IsDeckBattlerAlive(targets[i]))
        {
            gBattlerTarget = targets[i];
            StartBattlerAnim(targets[i], ANIM_STAT_CHANGE);
            ApplyStatChange(gBattlerAttacker, gBattlerTarget, gDeckMovesInfo[gCurrentMove].param);
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
            gDeckStruct.lastHitDamage = damage = -(gDeckMovesInfo[gCurrentMove].power * gDeckMons[gBattlerAttacker].power) / 100;
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

static void ExecuteSwapEffect(void)
{
    u32 targetsCount = 0;
    u32 aliveCount = 0;
    enum BattleId targets[MAX_DECK_BATTLERS_COUNT] = {0};
    PopulateTargetsList(targets, &targetsCount);

    // Count living targets.
    for (u32 i = 0; i < targetsCount; ++i)
    {
        if (IsDeckBattlerAlive(targets[i]))
            aliveCount += 1;
    }

    // Execute effect.
    if (aliveCount == 1 && targets[0] != gBattlerAttacker)
    {
        SwapBattlerPositions(gBattlerAttacker, targets[0]);
        PlaySE(SE_M_DOUBLE_TEAM);
        PrintMoveOutcomeString(aliveCount);
        
        if (gDeckMovesInfo[gCurrentMove].param == STAT_ATK || gDeckMovesInfo[gCurrentMove].param == 0xFF)
            gDeckMons[targets[0]].powerBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
        if (gDeckMovesInfo[gCurrentMove].param == STAT_DEF || gDeckMovesInfo[gCurrentMove].param == 0xFF)
            gDeckMons[targets[0]].defBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
    }
    else if (aliveCount == 2)
    {
        SwapBattlerPositions(targets[0], targets[1]);
        PlaySE(SE_M_DOUBLE_TEAM);
        PrintMoveOutcomeString(aliveCount);
        
        if (gDeckMovesInfo[gCurrentMove].param == STAT_ATK || gDeckMovesInfo[gCurrentMove].param == 0xFF)
        {
            gDeckMons[targets[0]].powerBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
            gDeckMons[targets[1]].powerBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
        }
        if (gDeckMovesInfo[gCurrentMove].param == STAT_DEF || gDeckMovesInfo[gCurrentMove].param == 0xFF)
        {
            gDeckMons[targets[0]].defBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
            gDeckMons[targets[1]].defBoost += (gDeckMons[gBattlerAttacker].power * gDeckMovesInfo[gCurrentMove].power) / 100;
        }
    }
    else
    {
        PrintStringToMessageBox(COMPOUND_STRING("But it failed…"));
        return;
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
    case 4: // Handle secondary effects.
        switch (gDeckMovesInfo[gCurrentMove].secondary) // TODO: Move to separate func
        {
        case DECK_SECONDARY_RECHARGE:
            if (gDeckMovesInfo[gCurrentMove].param != 0)
                gDeckMons[gBattlerAttacker].rechargeTurns += 1 + gDeckMovesInfo[gCurrentMove].param;
            else
                gDeckMons[gBattlerAttacker].rechargeTurns += 2; // includes turn end
            break;
        }
        ++gTasks[taskId].tState;
        break;
    case 5: // Check for fainted battlers.
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
