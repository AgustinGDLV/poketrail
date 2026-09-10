#ifndef GUARD_DECK_BATTLE_H
#define GUARD_DECK_BATTLE_H

enum BattleId
{
    B_PLAYER_0,
    B_PLAYER_1,
    B_PLAYER_2,
    B_PLAYER_3,
    B_PLAYER_4,
    B_PLAYER_5,
    B_OPPONENT_0,
    B_OPPONENT_1,
    B_OPPONENT_2,
    B_OPPONENT_3,
    B_OPPONENT_4,
    B_OPPONENT_5,
    MAX_DECK_BATTLERS_COUNT
};

enum BattlePosition // left to right for each side
{
    POSITION_0,
    POSITION_1,
    POSITION_2,
    POSITION_3,
    POSITION_4,
    POSITION_5,
    POSITIONS_COUNT,
};

struct DeckSpeciesInfo
{
    u16 baseHP;
    u16 basePower;
    u16 baseDef;
    u16 move;
    u16 ability;

    const u8 *playerIdle;
    const u8 *opponentIdle;
    const u8 *playerAttack;
    const u8 *opponentAttack;
    const u8 *playerHurt;
    const u8 *opponentHurt;
    const u16 *objectPalette;
    const s8 playerYOffset;
    const s8 opponentYOffset;

    const u8 *portrait;
    const u16 *portraitPalette;
};

struct DeckBattlePokemon
{
    u16 species;
    u16 lvl;
    u16 hp;
    u16 maxHP;
    u16 power;
    u16 def;
    u8 partyIndex;
    enum BattlePosition pos;
    enum BattlePosition initialPos;
    bool8 hasMoved;
    bool8 hasSwapped;
    u8 swapCount;
    u8 rechargeTurns;

    s16 powerBoost; // flat power boost
    s16 defBoost;   // flat defense boost
};

// action constants
#define ACTION_ATTACK   0
#define ACTION_SWAP     1

#define MAX_ACTIONS     12

struct BattleAction
{
    u8 type;
    u8 attacker;
    u8 target;
    u16 move;
};

// target constants
#define TARGET_SINGLE_OPPONENT      (1 << 0)
#define TARGET_SINGLE_ALLY          (1 << 1)
#define TARGET_ALL_OPPONENTS        (1 << 2)
#define TARGET_LEFT_ALLY            (1 << 3)
#define TARGET_RIGHT_ALLY           (1 << 4)
#define TARGET_ALL_ALLIES           (1 << 5)
#define TARGET_USER                 (1 << 6)
#define TARGET_OPPOSITE_LEFT        (1 << 7)
#define TARGET_OPPOSITE             (1 << 8)
#define TARGET_OPPOSITE_RIGHT       (1 << 9)

// turn end constants
#define TURN_END_CHECK_FATIGUE  0
#define TURN_END_FATIGUE        1
#define TURN_END_POISON         2
#define TURN_END_SLEEP          3
#define TURN_END_RESET_STATUS   4
#define TURN_END_REGENERATIVE   5
#define TURN_END_RECHARGE       6
#define TURN_END_COMPLETED      7
#define TURN_END_PAUSE          8

// move data

enum DeckMove
{
    DECK_TACKLE,
    DECK_QUICK_ATTACK,
    DECK_SUPER_FANG,
    DECK_VINE_WHIP,
    DECK_HELPING_HAND,
    DECK_SURF,
    DECK_CRABHAMMER,
    DECK_NOURISH,
    DECK_HARDEN,
    DECK_FLAME_WHEEL,
    DECK_FLARE_BLITZ,
    DECK_CHARGE,
    DECK_COTTON_GUARD,
    DECK_BITE,
    DECK_CRUNCH,
    DECK_HEAL_BELL,
    DECK_FRENZY_PLANT,
    DECK_POLLEN_PUFF,
    DECK_AROMATHERAPY,
    DECK_WHIRLWIND,
    DECK_BUG_BITE,
    DECK_SILVER_WIND,
    DECK_LICK,
    DECK_HEADBUTT,
    DECK_FOCUS_PUNCH,
    DECK_TANGLE,
    DECK_GIGA_IMPACT,
    DECK_BULLDOZE,
    DECK_EARTHQUAKE,
    DECK_PSYCHO_SHIFT,
    DECK_HURRICANE,
    DECK_SANDSTORM,
    DECK_RAIN_DANCE,
    DECK_HYPER_VOICE,
    DECK_BOOMBURST,
    DECK_SAND_ATTACK,
    DECK_SCARY_FACE,
    DECK_IRON_DEFENSE,
    DECK_SMOKESCREEN,
    DECK_LAVA_PLUME,
    DECK_OVERHEAT,
    DECK_RAPID_SPIN,
    DECK_SLUDGE_BOMB,
    DECK_ACID_SPRAY,
    DECK_MORNING_SUN,
    DECK_MAGICAL_LEAF,
    DECK_SOFTBOILED,
    DECK_HELPING_HANDS,
    DECK_TORMENT,
    DECK_CALM_MIND,
    DECK_MOVES_COUNT,
};

struct DeckMoveInfo
{
    const u8* name;
    const u8* description;
    u8 power;
    u16 target;
    u8 effect;
    u8 secondary;
    u8 param;
};

// ability data

enum DeckAbility
{
    DECK_ABILITY_NONE,
    DECK_AGGRESSIVE,
    DECK_PATIENT,
    DECK_ENERGETIC,
    DECK_HEAVY,
    DECK_SOCIAL,
    DECK_ALPHA,
    DECK_INSPIRED,
    DECK_RESILIENT,
    DECK_TRICKY,
    DECK_VAMPIRIC,
    DECK_REGENERATIVE,
    DECK_SHORT_TEMPERED,
    DECK_ADAPTIVE,
    DECK_ABILITIES_COUNT,
};

struct DeckAbilityInfo
{
    const u8* name;
    const u8* description;
};

struct DeckBattleStruct
{
    enum BattlePosition selectedPos; // for player selection
    struct BattleAction queuedActions[MAX_ACTIONS];
    u8 actionsCount;
    u8 executedCount;
    u8 actingSide;
    u8 turns;
    u32 exp;
    enum BattleId battlerExp;
    enum BattleId battlerCaught;
    bool8 isSelectionPhase;

    s32 lastHitDamage;

    bool8 isBossBattle;
    u32 musicOverride;
    uq4_12_t bossHPMult;
    u8 caughtWindowId; // maybe belongs in interface struct
};

enum DeckBattleBackgroundId
{
    BG_GRASS,
    BG_GRASS_EVE,
    BG_GRASS_NIGHT,

    BG_UNDERWATER,
    BG_UNDERWATER_EVE,
    BG_UNDERWATER_NIGHT,

    BG_CITY,
    BG_CITY_EVE,
    BG_CITY_NIGHT,

    BG_ICE_PATH,
    BG_ICE_PATH_EVE,
    BG_ICE_PATH_NIGHT,

    BG_VOLCANO,
    BG_VOLCANO_EVE,
    BG_VOLCANO_NIGHT,

    BG_CAVE,
    BG_CAVE_EVE,
    BG_CAVE_NIGHT,

    BG_DESERT,
    BG_DESERT_EVE,
    BG_DESERT_NIGHT,

    BG_FOREST,
    BG_FOREST_EVE,
    BG_FOREST_NIGHT,

    BG_COUNT,
};

struct DeckBattleBackground
{
    const u32 *tiles;
    const u16 *palette;
    const u32 *map;
};

void CB2_OpenDeckBattleCustom(void);
void Task_PrepareForActionPhase(u8 taskId);
void Task_CheckFaintAndContinue(u8 taskId);
void Task_WaitForFaintAnim(u8 taskId);
void Task_CheckForBattleEnd(u8 taskId);
void Task_CloseDeckBattle(u8 taskId);
void Task_ExecuteQueuedActionOrEnd(u8 taskId);
void QueueAction(u32 type, u32 battlerAtk, u32 battlerDef, u32 move);
void SwapBattlerPositions(u32 battler1, u32 battler2);
s32 CalculateDamage(u32 battlerAtk, u32 battlerDef, u32 move);
void UpdateBattlerHP(u32 battler, s32 damage);

extern struct DeckBattleStruct gDeckStruct;
extern struct DeckBattlePokemon gDeckMons[MAX_DECK_BATTLERS_COUNT];
extern const struct DeckSpeciesInfo gDeckSpeciesInfo[NUM_SPECIES];
extern const struct DeckMoveInfo gDeckMovesInfo[DECK_MOVES_COUNT];
extern const struct DeckAbilityInfo gDeckAbilitiesInfo[DECK_ABILITIES_COUNT];
extern const struct DeckBattleBackground gDeckBackgrounds[BG_COUNT];

#endif
