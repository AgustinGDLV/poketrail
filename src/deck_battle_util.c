#include "global.h"
#include "battle.h"
#include "deck_battle.h"
#include "deck_battle_effects.h"
#include "deck_battle_interface.h"
#include "deck_battle_util.h"
#include "deck_battle_controller.h"
#include "deck_battle_ai.h"
#include "constants/species.h"

/* deck_battle_util.c
 *
 * This file holds several utility functions used across
 * all the deck battle engine files. Splashable functions
 * should be kept here.
 * 
*/

u32 GetDeckBattlerSide(enum BattleId battler)
{
    if (battler < B_OPPONENT_0)
        return B_SIDE_PLAYER;
    else
        return B_SIDE_OPPONENT;
}

u32 GetDeckBattlerAtPos(u32 side, enum BattlePosition position)
{
    if (side == B_SIDE_PLAYER)
    {
        for (enum BattleId battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
        {
            if (IsDeckBattlerAlive(battler) && gDeckMons[battler].pos == position)
                return battler;
        }
    }
    else
    {
        for (enum BattleId battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
            if (IsDeckBattlerAlive(battler) && gDeckMons[battler].pos == position)
                return battler;
    }
    return MAX_DECK_BATTLERS_COUNT; // no luck
}

enum BattlePosition GetLeftmostOccupiedPosition(u32 side)
{
    enum BattleId battler;
    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (IsDeckBattlerAlive(battler))
            return pos;
    }
    return POSITION_0; // no luck
}

enum BattlePosition GetLeftmostPositionToMove(u32 side)
{
    enum BattleId battler;
    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && IsDeckBattlerAlive(battler))
            return pos;
    }
    return POSITIONS_COUNT; // no luck
}

enum BattlePosition GetOccupiedOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (IsDeckBattlerAlive(battler))
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetNonAttackerOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (IsDeckBattlerAlive(battler) && battler != gBattlerAttacker)
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetAnyNonAttackerOnLeft(u32 side, enum BattlePosition position)
{
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        if (GetDeckBattlerAtPos(side, pos) != gBattlerAttacker)
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetToMoveOnLeft(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    if (position == POSITION_0)
        return POSITIONS_COUNT;

    for (enum BattlePosition pos = position - 1; pos >= POSITION_0 && pos != (-1); --pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && IsDeckBattlerAlive(battler))
            return pos;
    }

    return POSITIONS_COUNT;
}

enum BattlePosition GetOccupiedOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (IsDeckBattlerAlive(battler))
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetNonAttackerOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (IsDeckBattlerAlive(battler) && battler != gBattlerAttacker)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetAnyNonAttackerOnRight(u32 side, enum BattlePosition position)
{
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        if (GetDeckBattlerAtPos(side, pos) != gBattlerAttacker)
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattlePosition GetToMoveOnRight(u32 side, enum BattlePosition position)
{
    enum BattleId battler;
    for (enum BattlePosition pos = position + 1; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (!gDeckMons[battler].hasMoved && IsDeckBattlerAlive(battler))
            return pos;
    }
    return POSITIONS_COUNT;
}

enum BattleId GetRandomBattlerOnSide(u32 side)
{
    enum BattleId battler;
    enum BattleId ids[PARTY_SIZE];
    u32 occupiedCount = 0;

    for (enum BattlePosition pos = POSITION_0; pos < POSITIONS_COUNT; ++pos)
    {
        battler = GetDeckBattlerAtPos(side, pos);
        if (IsDeckBattlerAlive(battler))
        {
            ids[occupiedCount] = battler;
            ++occupiedCount;
        }
    }

    return ids[Random() % occupiedCount];
}

bool32 IsDeckBattlerAlive(enum BattleId battler)
{
    return (battler != MAX_DECK_BATTLERS_COUNT && gDeckMons[battler].hp != 0 && gDeckMons[battler].species != SPECIES_NONE);
}

bool32 IsBattlerAliveOnSide(u32 side)
{
    bool32 result = FALSE;
    if (side == B_SIDE_PLAYER)
    {
        for (enum BattleId battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
            if (IsDeckBattlerAlive(battler))
                result = TRUE;
    }
    else
    {
        for (enum BattleId battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
            if (IsDeckBattlerAlive(battler))
                result = TRUE;
    }
    return result;
}

void PopulateTargetsList(enum BattleId *targets, u32 *targetsCount)
{
    u32 side = GetDeckBattlerSide(gBattlerAttacker);
    enum BattleId battler;
    enum BattlePosition pos = gDeckMons[gBattlerAttacker].pos;

    // ALL_OPPONENTS / ALL_ALLIES
    if ((side == B_SIDE_PLAYER && (gDeckMovesInfo[gCurrentMove].target & TARGET_ALL_OPPONENTS))
        || (side == B_SIDE_OPPONENT && (gDeckMovesInfo[gCurrentMove].target & TARGET_ALL_ALLIES)))
    {
        for (battler = B_OPPONENT_0; battler < MAX_DECK_BATTLERS_COUNT; ++battler)
        {
            targets[*targetsCount] = battler;
            *targetsCount += 1;
        }
    }

    // ALL_OPPONENTS / ALL_ALLIES
    if ((side == B_SIDE_OPPONENT && (gDeckMovesInfo[gCurrentMove].target & TARGET_ALL_OPPONENTS))
        || (side == B_SIDE_PLAYER && (gDeckMovesInfo[gCurrentMove].target & TARGET_ALL_ALLIES)))
    {
        for (battler = B_PLAYER_0; battler < B_OPPONENT_0; ++battler)
        {
            targets[*targetsCount] = battler;
            *targetsCount += 1;
        }
    }

    // USER
    if (gDeckMovesInfo[gCurrentMove].target & TARGET_USER)
    {
        targets[*targetsCount] = gBattlerAttacker;
        *targetsCount += 1;
    }

    // LEFT_ALLY
    if ((gDeckMovesInfo[gCurrentMove].target & TARGET_LEFT_ALLY)
        && pos != POSITION_0)
    {
        targets[*targetsCount] = GetDeckBattlerAtPos(side, pos - 1);
        *targetsCount += 1;
    }

    // RIGHT_ALLY
    if ((gDeckMovesInfo[gCurrentMove].target & TARGET_RIGHT_ALLY)
        && pos != POSITION_5)
    {
        targets[*targetsCount] = GetDeckBattlerAtPos(side, pos + 1);
        *targetsCount += 1;
    }

    // SINGLE_OPPONENT / SINGLE_ALLY
    if ((gDeckMovesInfo[gCurrentMove].target & TARGET_SINGLE_OPPONENT)
        || (gDeckMovesInfo[gCurrentMove].target & TARGET_SINGLE_ALLY))
    {
        if (!IsDeckBattlerAlive(gBattlerTarget))
        {
            if (gDeckMovesInfo[gCurrentMove].target & TARGET_SINGLE_OPPONENT)
                targets[*targetsCount] = GetRandomBattlerOnSide(side ^ 1);
            else
                targets[*targetsCount] = GetRandomBattlerOnSide(side);
        }
        else
        {
            targets[*targetsCount] = gBattlerTarget;
        }
        *targetsCount += 1;
    }

    // OPPOSITE_LEFT
    if ((gDeckMovesInfo[gCurrentMove].target & TARGET_OPPOSITE_LEFT)
        && pos != POSITION_0)
    {
        targets[*targetsCount] = GetDeckBattlerAtPos(side ^ 1, pos - 1);
        *targetsCount += 1;
    }

    // OPPOSITE
    if (gDeckMovesInfo[gCurrentMove].target & TARGET_OPPOSITE)
    {
        targets[*targetsCount] = GetDeckBattlerAtPos(side ^ 1, pos);
        *targetsCount += 1;
    }

    // OPPOSITE_RIGHT
    if ((gDeckMovesInfo[gCurrentMove].target & TARGET_OPPOSITE_RIGHT)
        && pos != POSITION_5)
    {
        targets[*targetsCount] = GetDeckBattlerAtPos(side ^ 1, pos + 1);
        *targetsCount += 1;
    }
}
