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
        .description = COMPOUND_STRING("Acting first increases PWR"),
    },

    [DECK_PATIENT] =
    {
        .name = COMPOUND_STRING("PATIENT"),
        .description = COMPOUND_STRING("Acting last increases PWR"),
    },

    [DECK_RESTLESS] =
    {
        .name = COMPOUND_STRING("RESTLESS"),
        .description = COMPOUND_STRING("Swapping increases PWR"),
    },

    [DECK_HEAVY] =
    {
        .name = COMPOUND_STRING("HEAVY"),
        .description = COMPOUND_STRING("Swapping reduces PWR"),
    },

    [DECK_SOCIAL] =
    {
        .name = COMPOUND_STRING("SOCIAL"),
        .description = COMPOUND_STRING("More power for each SOCIAL"),
    },

    [DECK_ALPHA] =
    {
        .name = COMPOUND_STRING("ALPHA"),
        .description = COMPOUND_STRING("Less power for each ALPHA"),
    },

    [DECK_INSPIRED] =
    {
        .name = COMPOUND_STRING("INSPIRED"),
        .description = COMPOUND_STRING("Acting grants allies PWR"),
    },

    [DECK_ENDURING] =
    {
        .name = COMPOUND_STRING("ENDURING"),
        .description = COMPOUND_STRING("Takes reduced fatigue"),
    },
};