const struct DeckAbilityInfo gDeckAbilitiesInfo[DECK_ABILITIES_COUNT] =
{
    [DECK_ABILITY_NONE] =
    {
        .name = COMPOUND_STRING("NONE"),
        .description = COMPOUND_STRING("…"),
    },

    [DECK_AGGRESSIVE] =
    {
        .name = COMPOUND_STRING("AGGRESSIVE"),
        .description = COMPOUND_STRING("Acting first increases PWR for turn"),
        .infoMenuDesc = COMPOUND_STRING("AGGRESSIVE: Acting first\nincreases PWR for turn"),
    },

    [DECK_PATIENT] =
    {
        .name = COMPOUND_STRING("PATIENT"),
        .description = COMPOUND_STRING("Acting last increases PWR for turn"),
        .infoMenuDesc = COMPOUND_STRING("PATIENT: Acting last\nincreases PWR for turn"),
    },

    [DECK_ENERGETIC] =
    {
        .name = COMPOUND_STRING("ENERGETIC"),
        .description = COMPOUND_STRING("Swapping increases PWR for battle"),
        .infoMenuDesc = COMPOUND_STRING("ENERGETIC: Swapping increases\nPWR for battle"),
    },

    [DECK_HEAVY] =
    {
        .name = COMPOUND_STRING("HEAVY"),
        .description = COMPOUND_STRING("Swapping reduces PWR for battle"),
        .infoMenuDesc = COMPOUND_STRING("HEAVY: Swapping reduces\nPWR for battle"),
    },

    [DECK_SOCIAL] =
    {
        .name = COMPOUND_STRING("SOCIAL"),
        .description = COMPOUND_STRING("More power for each SOCIAL"),
        .infoMenuDesc = COMPOUND_STRING("SOCIAL: More power\nfor each SOCIAL"),
    },

    [DECK_ALPHA] =
    {
        .name = COMPOUND_STRING("ALPHA"),
        .description = COMPOUND_STRING("Less power for each ALPHA"),
        .infoMenuDesc = COMPOUND_STRING("ALPHA: Less power\nfor each ALPHA"),
    },

    [DECK_INSPIRED] =
    {
        .name = COMPOUND_STRING("INSPIRED"),
        .description = COMPOUND_STRING("Acting grants allies PWR for turn"),
        .infoMenuDesc = COMPOUND_STRING("INSPIRED: Acting grants\nallies PWR for turn"),
    },

    [DECK_RESILIENT] =
    {
        .name = COMPOUND_STRING("RESILIENT"),
        .description = COMPOUND_STRING("Takes reduced fatigue damage"),
        .infoMenuDesc = COMPOUND_STRING("RESILIENT: Takes reduced\nfatigue damage"),
    },

    [DECK_TRICKY] =
    {
        .name = COMPOUND_STRING("TRICKY"),
        .description = COMPOUND_STRING("Gains random PWR for turn"),
        .infoMenuDesc = COMPOUND_STRING("TRICKY: Gains random\nPWR for turn"),
    },

    [DECK_VAMPIRIC] =
    {
        .name = COMPOUND_STRING("VAMPIRIC"),
        .description = COMPOUND_STRING("Heals percent of damage"),
        .infoMenuDesc = COMPOUND_STRING("VAMPIRIC: Heals percent\nof damage"),
    },

    [DECK_REGENERATIVE] =
    {
        .name = COMPOUND_STRING("REGENERATIVE"),
        .description = COMPOUND_STRING("Heals each turn"),
        .infoMenuDesc = COMPOUND_STRING("REGENERATIVE: Heals\neach turn"),
    },

    [DECK_SHORT_TEMPERED] =
    {
        .name = COMPOUND_STRING("SHORT TEMPERED"),
        .description = COMPOUND_STRING("Less PWR with each turn"),
        .infoMenuDesc = COMPOUND_STRING("SHORT TEMPERED: Less PWR\nwith each turn"),
    },

    [DECK_ADAPTIVE] =
    {
        .name = COMPOUND_STRING("ADAPTIVE"),
        .description = COMPOUND_STRING("More PWR with each turn"),
        .infoMenuDesc = COMPOUND_STRING("ADAPTIVE: More PWR\nwith each turn"),
    },
};