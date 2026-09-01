const struct DeckMoveInfo gDeckMovesInfo[DECK_MOVES_COUNT] =
{
    [DECK_TACKLE] =
    {
        .name = COMPOUND_STRING("TACKLE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 40,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_VINE_WHIP] =
    {
        .name = COMPOUND_STRING("VINE WHIP"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 60,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_HELPING_HAND] =
    {
        .name = COMPOUND_STRING("HELPING HAND"),
        .description = COMPOUND_STRING("Powers up right ally."),
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

    [DECK_EARTHQUAKE] =
    {
        .name = COMPOUND_STRING("EARTHQUAKE"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 60,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY | TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CRABHAMMER] =
    {
        .name = COMPOUND_STRING("CRABHAMMER"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 80,
        .target = TARGET_SINGLE_OPPONENT,
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
};
