#ifndef GUARD_DECK_BATTLE_UTIL_H
#define GUARD_DECK_BATTLE_UTIL_H

u32 GetDeckBattlerSide(enum BattleId battler);
u32 GetDeckBattlerAtPos(u32 side, enum BattlePosition position);

enum BattlePosition GetLeftmostOccupiedPosition(u32 side);
enum BattlePosition GetLeftmostPositionToMove(u32 side);

enum BattlePosition GetOccupiedOnLeft(u32 side, enum BattlePosition position);
enum BattlePosition GetNonAttackerOnLeft(u32 side, enum BattlePosition position);
enum BattlePosition GetAnyNonAttackerOnLeft(u32 side, enum BattlePosition position);
enum BattlePosition GetToMoveOnLeft(u32 side, enum BattlePosition position);
enum BattlePosition GetOccupiedOnRight(u32 side, enum BattlePosition position);
enum BattlePosition GetNonAttackerOnRight(u32 side, enum BattlePosition position);
enum BattlePosition GetAnyNonAttackerOnRight(u32 side, enum BattlePosition position);
enum BattlePosition GetToMoveOnRight(u32 side, enum BattlePosition position);

enum BattleId GetRandomBattlerOnSide(u32 side);
enum BattleId GetRandomTargetOnSide(u32 side);

bool32 IsDeckBattlerAlive(enum BattleId battler);
bool32 IsBattlerAliveOnSide(u32 side);

void PopulateTargetsList(enum BattleId *targets, u32 *targetsCount);

#endif
