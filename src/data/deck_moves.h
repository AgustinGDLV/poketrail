const struct DeckMoveInfo gDeckMovesInfo[DECK_MOVES_COUNT] =
{
    [DECK_TACKLE] =
    {
        .name = COMPOUND_STRING("TACKLE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 50,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },
    
    [DECK_QUICK_ATTACK] =
    {
        .name = COMPOUND_STRING("QUICK ATTACK"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 40,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_VINE_WHIP] =
    {
        .name = COMPOUND_STRING("VINE WHIP"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .power = 60,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_FRENZY_PLANT] =
    {
        .name = COMPOUND_STRING("FRENZY PLANT"),
        .description = COMPOUND_STRING("Damages opponents, recharges."),
        .power = 100,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
    },

    [DECK_HELPING_HAND] =
    {
        .name = COMPOUND_STRING("HELPING HAND"),
        .description = COMPOUND_STRING("Boosts right ally power."),
        .power = 50,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_SURF] =
    {
        .name = COMPOUND_STRING("SURF"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 60,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CRABHAMMER] =
    {
        .name = COMPOUND_STRING("CRABHAMMER"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .power = 80,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_NOURISH] =
    {
        .name = COMPOUND_STRING("NOURISH"),
        .description = COMPOUND_STRING("Heals right ally."),
        .power = 25,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_HARDEN] =
    {
        .name = COMPOUND_STRING("HARDEN"),
        .description = COMPOUND_STRING("Boosts defense of adjacent allies."),
        .power = 50,
        .target = TARGET_LEFT_ALLY | TARGET_USER | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_FLAME_WHEEL] =
    {
        .name = COMPOUND_STRING("FLAME WHEEL"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .power = 60,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_FLARE_BLITZ] =
    {
        .name = COMPOUND_STRING("FLARE_BLITZ"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .power = 120,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CHARGE] =
    {
        .name = COMPOUND_STRING("CHARGE"),
        .description = COMPOUND_STRING("Boosts power of adjacent allies."),
        .power = 50,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_COTTON_GUARD] =
    {
        .name = COMPOUND_STRING("COTTON GUARD"),
        .description = COMPOUND_STRING("Boosts adjacent ally stats."),
        .power = 50,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF,
    },

    [DECK_BITE] =
    {
        .name = COMPOUND_STRING("BITE"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .power = 60,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CRUNCH] =
    {
        .name = COMPOUND_STRING("BITE"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .power = 80,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_HEAL_BELL] =
    {
        .name = COMPOUND_STRING("HEAL BELL"),
        .description = COMPOUND_STRING("Heals all allies."),
        .power = 70,
        .target = TARGET_ALL_ALLIES,
        .effect = DECK_EFFECT_HEAL,
        .secondary = DECK_SECONDARY_RECHARGE,
    },

    [DECK_POLLEN_PUFF] =
    {
        .name = COMPOUND_STRING("POLLEN PUFF"),
        .description = COMPOUND_STRING("Heals right ally."),
        .power = 40,
        .target = TARGET_SINGLE_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_AROMATHERAPY] =
    {
        .name = COMPOUND_STRING("AROMATHERAPY"),
        .description = COMPOUND_STRING("Heals allies."),
        .power = 25,
        .target = TARGET_ALL_ALLIES,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_WHIRLWIND] =
    {
        .name = COMPOUND_STRING("WHIRLWIND"),
        .description = COMPOUND_STRING("Swaps allies, boosts DEF."),
        .power = 50,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_DEF,
    },

    [DECK_BUG_BITE] =
    {
        .name = COMPOUND_STRING("BUG BITE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 50,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_SILVER_WIND] =
    {
        .name = COMPOUND_STRING("SILVER WIND"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 40,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_LICK] =
    {
        .name = COMPOUND_STRING("LICK"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 40,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_HEADBUTT] =
    {
        .name = COMPOUND_STRING("HEADBUTT"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .power = 60,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_FOCUS_PUNCH] =
    {
        .name = COMPOUND_STRING("FOCUS PUNCH"),
        .description = COMPOUND_STRING("Damages one opponent, recharges."),
        .power = 150,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
    },

    [DECK_TWIST] =
    {
        .name = COMPOUND_STRING("TWIST"),
        .description = COMPOUND_STRING("Swaps allies, boosts PWR."),
        .power = 40,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_ATK,
    },

    [DECK_GIGA_IMPACT] =
    {
        .name = COMPOUND_STRING("GIGA IMPACT"),
        .description = COMPOUND_STRING("Damages 3 opposite, recharges 2."),
        .power = 150,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
        .param = 2,
    },

    [DECK_EARTHQUAKE] =
    {
        .name = COMPOUND_STRING("Earthquakes"),
        .description = COMPOUND_STRING("Damages all opponents, recharges."),
        .power = 100,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
        .param = 1,
    },
};
