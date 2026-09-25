const struct DeckMoveInfo gDeckMovesInfo[DECK_MOVES_COUNT] =
{
    [DECK_TACKLE] =
    {
        .name = COMPOUND_STRING("TACKLE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("TACKLE: Damages one\nopponent."),
        .power = 60,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },
    
    [DECK_QUICK_ATTACK] =
    {
        .name = COMPOUND_STRING("QUICK ATTACK"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("QUICK ATTACK: Damages one\nopponent."),
        .power = 80,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },
    
    [DECK_SUPER_FANG] =
    {
        .name = COMPOUND_STRING("SUPER FANG"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("SUPER FANG: Damages one\nopponent."),
        .power = 120,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_VINE_WHIP] =
    {
        .name = COMPOUND_STRING("VINE WHIP"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .infoMenuDesc = COMPOUND_STRING("VINE WHIP: Damages\nopposite opponent."),
        .power = 90,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_FRENZY_PLANT] =
    {
        .name = COMPOUND_STRING("FRENZY PLANT"),
        .description = COMPOUND_STRING("Damages opponents, recharges."),
        .infoMenuDesc = COMPOUND_STRING("FRENZY PLANT: Damages\nopponents, recharges."),
        .power = 50,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
    },

    [DECK_HELPING_HAND] =
    {
        .name = COMPOUND_STRING("HELPING HAND"),
        .description = COMPOUND_STRING("Boosts right ally PWR."),
        .infoMenuDesc = COMPOUND_STRING("HELPING HAND: Boosts right\nally PWR."),
        .power = 40,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_SURF] =
    {
        .name = COMPOUND_STRING("SURF"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .infoMenuDesc = COMPOUND_STRING("SURF: Damages all\nopponents."),
        .power = 30,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CRABHAMMER] =
    {
        .name = COMPOUND_STRING("CRABHAMMER"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("CRABHAMMER: Damages 3\nopposite opponents."),
        .power = 100,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_NOURISH] =
    {
        .name = COMPOUND_STRING("NOURISH"),
        .description = COMPOUND_STRING("Heals right ally."),
        .infoMenuDesc = COMPOUND_STRING("NOURISH: Heals right\nally."),
        .power = 25,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_HARDEN] =
    {
        .name = COMPOUND_STRING("HARDEN"),
        .description = COMPOUND_STRING("Boosts DEF of adjacent allies."),
        .infoMenuDesc = COMPOUND_STRING("HARDEN: Boosts DEF of\nadjacent allies."),
        .power = 30,
        .target = TARGET_LEFT_ALLY | TARGET_USER | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_FLAME_WHEEL] =
    {
        .name = COMPOUND_STRING("FLAME WHEEL"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .infoMenuDesc = COMPOUND_STRING("FLAME WHEEL: Damages\nopposite opponent."),
        .power = 90,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_FLARE_BLITZ] =
    {
        .name = COMPOUND_STRING("FLARE BLITZ"),
        .description = COMPOUND_STRING("Damages opposite opponent, recoils."),
        .infoMenuDesc = COMPOUND_STRING("FLARE BLITZ: Damages opposite\nopponent, recoils."),
        .power = 180,
        .target = TARGET_OPPOSITE | TARGET_USER,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CHARGE] =
    {
        .name = COMPOUND_STRING("CHARGE"),
        .description = COMPOUND_STRING("Boosts power of adjacent allies."),
        .infoMenuDesc = COMPOUND_STRING("CHARGE: Boosts PWR of\nadjacent allies."),
        .power = 30,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_COTTON_GUARD] =
    {
        .name = COMPOUND_STRING("COTTON GUARD"),
        .description = COMPOUND_STRING("Boosts adjacent ally stats."),
        .infoMenuDesc = COMPOUND_STRING("COTTON GUARD: Boosts stats of\nadjacent allies."),
        .power = 30,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF,
    },

    [DECK_BITE] =
    {
        .name = COMPOUND_STRING("BITE"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .infoMenuDesc = COMPOUND_STRING("BITE: Damages\nopposite opponent."),
        .power = 60,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_CRUNCH] =
    {
        .name = COMPOUND_STRING("CRUNCH"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("CRUNCH: Damages 3\nopposite opponents."),
        .power = 100,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_HEAL_BELL] =
    {
        .name = COMPOUND_STRING("HEAL BELL"),
        .description = COMPOUND_STRING("Heals all allies, recharges."),
        .infoMenuDesc = COMPOUND_STRING("HEAL BELL: Heals\nall allies, recharges."),
        .power = 45,
        .target = TARGET_ALL_ALLIES,
        .effect = DECK_EFFECT_HEAL,
        .secondary = DECK_SECONDARY_RECHARGE,
    },

    [DECK_POLLEN_PUFF] =
    {
        .name = COMPOUND_STRING("POLLEN PUFF"),
        .description = COMPOUND_STRING("Heals right ally."),
        .infoMenuDesc = COMPOUND_STRING("POLLEN PUFF: Heals right\nally."),
        .power = 40,
        .target = TARGET_SINGLE_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_AROMATHERAPY] =
    {
        .name = COMPOUND_STRING("AROMATHERAPY"),
        .description = COMPOUND_STRING("Heals allies."),
        .infoMenuDesc = COMPOUND_STRING("HEAL BELL: Heals\nall allies."),
        .power = 25,
        .target = TARGET_ALL_ALLIES,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_WHIRLWIND] =
    {
        .name = COMPOUND_STRING("WHIRLWIND"),
        .description = COMPOUND_STRING("Swaps allies, boosts DEF."),
        .infoMenuDesc = COMPOUND_STRING("WHIRLWIND: Swaps\nallies, boosts DEF."),
        .power = 30,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_DEF,
    },

    [DECK_BUG_BITE] =
    {
        .name = COMPOUND_STRING("BUG BITE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("BUG BITE: Damages one\nopponent."),
        .power = 80,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_SILVER_WIND] =
    {
        .name = COMPOUND_STRING("SILVER WIND"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .infoMenuDesc = COMPOUND_STRING("SILVER WIND: Damages all\nopponents."),
        .power = 30,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_LICK] =
    {
        .name = COMPOUND_STRING("LICK"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("LICK: Damages one\nopponent."),
        .power = 70,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_HEADBUTT] =
    {
        .name = COMPOUND_STRING("HEADBUTT"),
        .description = COMPOUND_STRING("Damages opposite opponent."),
        .infoMenuDesc = COMPOUND_STRING("HEADBUTT: Damages\nopposite opponent."),
        .power = 100,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_FOCUS_PUNCH] =
    {
        .name = COMPOUND_STRING("FOCUS PUNCH"),
        .description = COMPOUND_STRING("Damages one opponent, recharges."),
        .infoMenuDesc = COMPOUND_STRING("FOCUS PUNCH: Damages one\nopponent, recharges."),
        .power = 200,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
    },

    [DECK_TANGLE] =
    {
        .name = COMPOUND_STRING("TANGLE"),
        .description = COMPOUND_STRING("Swaps allies, boosts PWR."),
        .infoMenuDesc = COMPOUND_STRING("WHIRLWIND: Swaps\n2 allies, boosts PWR."),
        .power = 40,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_ATK,
    },

    [DECK_GIGA_IMPACT] =
    {
        .name = COMPOUND_STRING("GIGA IMPACT"),
        .description = COMPOUND_STRING("Damages 3 opposite, recharges 2."),
        .infoMenuDesc = COMPOUND_STRING("GIGA IMPACT: Damages 3\nopposite, recharges 2."),
        .power = 220,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
        .param = 2,
    },

    [DECK_EARTHQUAKE] =
    {
        .name = COMPOUND_STRING("Earthquakes"),
        .description = COMPOUND_STRING("Damages all opponents, 2 allies."),
        .infoMenuDesc = COMPOUND_STRING("EARTHQUAKE: Damages all\nopponents, 2 allies."),
        .power = 50,
        .target = TARGET_ALL_OPPONENTS | TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_BULLDOZE] =
    {
        .name = COMPOUND_STRING("BULLDOZE"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("BULLDOZE: Damages one\nopponent."),
        .power = 70,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_PSYCHO_SHIFT] =
    {
        .name = COMPOUND_STRING("PSYCHO SHIFT"),
        .description = COMPOUND_STRING("Swaps allies and boosts PWR."),
        .infoMenuDesc = COMPOUND_STRING("WHIRLWIND: Swaps\n2 allies, boosts DEF."),
        .power = 50,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_ATK,
    },

    [DECK_HURRICANE] =
    {
        .name = COMPOUND_STRING("HURRICANE"),
        .description = COMPOUND_STRING("Damages all opponents."),
        .infoMenuDesc = COMPOUND_STRING("HURRICANE: Damages all\nopponents."),
        .power = 30,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_SANDSTORM] =
    {
        .name = COMPOUND_STRING("SANDSTORM"),
        .description = COMPOUND_STRING("Lowers opponent stats."),
        .infoMenuDesc = COMPOUND_STRING("SANDSTORM: Lowers opponent\nstats."),
        .power = 20,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF, // PWR + DEF
    },

    [DECK_RAIN_DANCE] =
    {
        .name = COMPOUND_STRING("RAIN DANCE"),
        .description = COMPOUND_STRING("Boosts ally stats."),
        .infoMenuDesc = COMPOUND_STRING("RAIN DANCE: Boosts ally\nstats."),
        .power = 20,
        .target = TARGET_ALL_ALLIES,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF, // PWR + DEF
    },

    [DECK_HYPER_VOICE] =
    {
        .name = COMPOUND_STRING("HYPER VOICE"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("HYPER VOICE: Damages 3\nopposite opponents."),
        .power = 70,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_BOOMBURST] =
    {
        .name = COMPOUND_STRING("BOOMBURST"),
        .description = COMPOUND_STRING("Damages all opponents, recharges"),
        .infoMenuDesc = COMPOUND_STRING("BOOMBURST: Damages all\nopponents, recharges"),
        .power = 50,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_HIT,
        .secondary = DECK_SECONDARY_RECHARGE,
        .param = 1,
    },

    [DECK_SAND_ATTACK] =
    {
        .name = COMPOUND_STRING("SAND ATTACK"),
        .description = COMPOUND_STRING("Lowers 3 opposite DEF."),
        .infoMenuDesc = COMPOUND_STRING("SAND ATTACK: Lowers 3\nopposite DEF."),
        .power = 40,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_SCARY_FACE] =
    {
        .name = COMPOUND_STRING("SCARY FACE"),
        .description = COMPOUND_STRING("Lowers opponent DEF."),
        .infoMenuDesc = COMPOUND_STRING("SCARY FACE: Lowers\nopponent DEF."),
        .power = 30,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_IRON_DEFENSE] =
    {
        .name = COMPOUND_STRING("IRON DEFENSE"),
        .description = COMPOUND_STRING("Boosts adjacent ally DEF."),
        .infoMenuDesc = COMPOUND_STRING("RAIN DANCE: Boosts adjacent\nally DEF."),
        .power = 70,
        .target = TARGET_LEFT_ALLY | TARGET_USER | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_SMOKESCREEN] =
    {
        .name = COMPOUND_STRING("SMOKESCREEN"),
        .description = COMPOUND_STRING("Lowers opponent PWR."),
        .infoMenuDesc = COMPOUND_STRING("SMOKESCREEN: Lowers\nopponent PWR."),
        .power = 20,
        .target = TARGET_ALL_OPPONENTS,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_ATK,
    },

    [DECK_LAVA_PLUME] =
    {
        .name = COMPOUND_STRING("LAVA PLUME"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("LAVA PLUME: Damages 3\nopposite opponents."),
        .power = 70,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_OVERHEAT] =
    {
        .name = COMPOUND_STRING("OVERHEAT"),
        .description = COMPOUND_STRING("Damages opponents, recoils."),
        .power = 50,
        .target = TARGET_ALL_OPPONENTS | TARGET_USER,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_RAPID_SPIN] =
    {
        .name = COMPOUND_STRING("RAPID SPIN"),
        .description = COMPOUND_STRING("Swaps with ally, boosts PWR."),
        .infoMenuDesc = COMPOUND_STRING("RAPID SPIN: Swaps with\nally, boosts PWR."),
        .power = 40,
        .target = TARGET_SINGLE_ALLY,
        .effect = DECK_EFFECT_SWAP,
        .param = STAT_ATK
    },

    [DECK_SLUDGE_BOMB] =
    {
        .name = COMPOUND_STRING("SLUDGE BOMB"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("SLUDGE BOMB: Damages one\nopponent."),
        .power = 70,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_ACID_SPRAY] =
    {
        .name = COMPOUND_STRING("ACID SPRAY"),
        .description = COMPOUND_STRING("Lowers opposite DEF."),
        .infoMenuDesc = COMPOUND_STRING("ACID SPRAY: Lowers\noppposite DEF."),
        .power = 50,
        .target = TARGET_OPPOSITE,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_MORNING_SUN] =
    {
        .name = COMPOUND_STRING("MORNING SUN"),
        .description = COMPOUND_STRING("Heals one ally."),
        .infoMenuDesc = COMPOUND_STRING("MORNING SUN: Heals\none ally."),
        .power = 60,
        .target = TARGET_SINGLE_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_MAGICAL_LEAF] =
    {
        .name = COMPOUND_STRING("MAGICAL LEAF"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("MAGICAL LEAF: Damages 3\nopposite opponents."),
        .power = 80,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_SOFTBOILED] =
    {
        .name = COMPOUND_STRING("SOFTBOILED"),
        .description = COMPOUND_STRING("Heals right ally."),
        .infoMenuDesc = COMPOUND_STRING("SOFTBOILED: Heals\nright ally."),
        .power = 40,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_HEAL,
    },

    [DECK_HELPING_HANDS] =
    {
        .name = COMPOUND_STRING("HELPING HANDS"),
        .description = COMPOUND_STRING("Boosts adjacent ally PWR."),
        .infoMenuDesc = COMPOUND_STRING("HELPING HANDS: Boosts\nadjacent ally PWR."),
        .power = 40,
        .target = TARGET_LEFT_ALLY | TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
    },

    [DECK_TORMENT] =
    {
        .name = COMPOUND_STRING("TORMENT"),
        .description = COMPOUND_STRING("Lowers one opponent PWR."),
        .infoMenuDesc = COMPOUND_STRING("TORMENT: Lowers one\nopponent PWR."),
        .power = 35,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_ATK
    },

    [DECK_CALM_MIND] =
    {
        .name = COMPOUND_STRING("CALM MIND"),
        .description = COMPOUND_STRING("Boosts right ally stats."),
        .infoMenuDesc = COMPOUND_STRING("CALM MIND: Boosts right\nally stats."),
        .power = 50,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = 0xFF,
    },

    [DECK_PLAY_ROUGH] =
    {
        .name = COMPOUND_STRING("PLAY ROUGH"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("PLAY ROUGH: Damages 3\nopposite opponents."),
        .power = 80,
        .target = TARGET_OPPOSITE | TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_ACID_ARMOR] =
    {
        .name = COMPOUND_STRING("ACID ARMOR"),
        .description = COMPOUND_STRING("Boosts right ally DEF."),
        .infoMenuDesc = COMPOUND_STRING("ACID ARMOR: Boosts\nright ally DEF."),
        .power = 50,
        .target = TARGET_RIGHT_ALLY,
        .effect = DECK_EFFECT_POWER_UP,
        .param = STAT_DEF,
    },

    [DECK_ICICLE_CRASH] =
    {
        .name = COMPOUND_STRING("ICICLE CRASH"),
        .description = COMPOUND_STRING("Damages 3 opposite opponents."),
        .infoMenuDesc = COMPOUND_STRING("ICICLE CRASH: Damages 3\nopposite opponents."),
        .power = 80,
        .target = TARGET_OPPOSITE_LEFT | TARGET_OPPOSITE | TARGET_OPPOSITE_RIGHT,
        .effect = DECK_EFFECT_HIT,
    },

    [DECK_TOXIC_FANG] =
    {
        .name = COMPOUND_STRING("TOXIC FANG"),
        .description = COMPOUND_STRING("Damages one opponent."),
        .infoMenuDesc = COMPOUND_STRING("TOXIC FANG: Damages one\nopponent."),
        .power = 130,
        .target = TARGET_SINGLE_OPPONENT,
        .effect = DECK_EFFECT_HIT,
    },
};
