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
    
    [DECK_SUPER_FANG] =
    {
        .name = COMPOUND_STRING("QUICK ATTACK"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 80,
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
        .description = COMPOUND_STRING("Boosts right ally PWR."),
        .power = 40,
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
        .power = 30,
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
        .power = 30,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_COTTON_GUARD] =
    {
        .name = COMPOUND_STRING("COTTON GUARD"),
        .description = COMPOUND_STRING("Boosts adjacent ally stats."),
        .power = 30,
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
        .name = COMPOUND_STRING("CRUNCH"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
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

    [DECK_TANGLE] =
    {
        .name = COMPOUND_STRING("TANGLE"),
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
        .description = COMPOUND_STRING("Damages all opponents, allies."),
        .power = 100,
        .target = TARGET_ALL_OPPONENTS | TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_BULLDOZE] =
    {
        .name = COMPOUND_STRING("BULLDOZE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 60,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_PSYCHO_SHIFT] =
    {
        .name = COMPOUND_STRING("PSYCHO SHIFT"),
        .description = COMPOUND_STRING("Swaps allies and boosts PWR."),
        .power = 50,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_ATK,
    },

    [DECK_HURRICANE] =
    {
        .name = COMPOUND_STRING("HURRICANE"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 70,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_SANDSTORM] =
    {
        .name = COMPOUND_STRING("SANDSTORM"),
        .description = COMPOUND_STRING("Lowers opponent stats."),
        .power = 15,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF, // PWR + DEF
    },

    [DECK_RAIN_DANCE] =
    {
        .name = COMPOUND_STRING("RAIN DANCE"),
        .description = COMPOUND_STRING("Boosts ally stats."),
        .power = 15,
        .target = TARGET_ALL_ALLIES,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF, // PWR + DEF
    },

    [DECK_HYPER_VOICE] =
    {
        .name = COMPOUND_STRING("RAIN DANCE"),
        .description = COMPOUND_STRING("Boosts ally stats."),
        .power = 70,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_BOOMBURST] =
    {
        .name = COMPOUND_STRING("RAIN DANCE"),
        .description = COMPOUND_STRING("Damages all opponents, recharges"),
        .power = 100,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
        .param = 1,
    },

    [DECK_SAND_ATTACK] =
    {
        .name = COMPOUND_STRING("SAND ATTACK"),
        .description = COMPOUND_STRING("Lowers opponent DEF."),
        .power = 40,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_SCARY_FACE] =
    {
        .name = COMPOUND_STRING("SCARY FACE"),
        .description = COMPOUND_STRING("Lowers opponent DEF."),
        .power = 20,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_IRON_DEFENSE] =
    {
        .name = COMPOUND_STRING("IRON DEFENSE"),
        .description = COMPOUND_STRING("Boosts ally DEF."),
        .power = 70,
        .target = TARGET_LEFT_ALLY | TARGET_USER | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_SMOKESCREEN] =
    {
        .name = COMPOUND_STRING("SMOKESCREEN"),
        .description = COMPOUND_STRING("Lowers opponent PWR."),
        .power = 20,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_ATK,
    },

    [DECK_LAVA_PLUME] =
    {
        .name = COMPOUND_STRING("LAVA PLUME"),
        .description = COMPOUND_STRING("Attacks 3 opposite opponents."),
        .power = 70,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_OVERHEAT] =
    {
        .name = COMPOUND_STRING("OVERHEAT"),
        .description = COMPOUND_STRING("Attacks opponents with recoil."),
        .power = 90,
        .target = TARGET_ALL_OPPONENTS | TARGET_USER,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_RAPID_SPIN] =
    {
        .name = COMPOUND_STRING("RAPID SPIN"),
        .description = COMPOUND_STRING("Swaps with an ally, boosts PWR."),
        .power = 40,
        .target = TARGET_SINGLE_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_ATK
    },

    [DECK_SLUDGE_BOMB] =
    {
        .name = COMPOUND_STRING("SLUDGE BOMB"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .power = 70,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_ACID_SPRAY] =
    {
        .name = COMPOUND_STRING("ACID SPRAY"),
        .description = COMPOUND_STRING("Lowers opposite's DEF."),
        .power = 50,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
        .param = STAT_DEF,
    },

    [DECK_MORNING_SUN] =
    {
        .name = COMPOUND_STRING("MORNING SUN"),
        .description = COMPOUND_STRING("Heals one ally."),
        .power = 60,
        .target = TARGET_SINGLE_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_MAGICAL_LEAF] =
    {
        .name = COMPOUND_STRING("MAGICAL LEAF"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .power = 70,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_SOFTBOILED] =
    {
        .name = COMPOUND_STRING("SOFTBOILED"),
        .description = COMPOUND_STRING("Heals right ally."),
        .power = 40,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_HELPING_HANDS] =
    {
        .name = COMPOUND_STRING("HELPING HANDS"),
        .description = COMPOUND_STRING("Boosts adjacent ally PWR."),
        .power = 40,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_TORMENT] =
    {
        .name = COMPOUND_STRING("TORMENT"),
        .description = COMPOUND_STRING("Lowers opponent PWR."),
        .power = 30,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF
    },

    [DECK_CALM_MIND] =
    {
        .name = COMPOUND_STRING("CALM MIND"),
        .description = COMPOUND_STRING("Boosts right ally stats."),
        .power = 30,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF,
    },

    [DECK_PLAY_ROUGH] =
    {
        .name = COMPOUND_STRING("PLAY ROUGH"),
        .description = COMPOUND_STRING("Damages opposite 3 opponents."),
        .power = 80,
        .target = TARGET_OPPOSITE | TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_ACID_ARMOR] =
    {
        .name = COMPOUND_STRING("ACID ARMOR"),
        .description = COMPOUND_STRING("Boosts right ally DEF."),
        .power = 40,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },
};
