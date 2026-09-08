const struct DeckSpeciesInfo gDeckSpeciesInfo[NUM_SPECIES] =
{
    [SPECIES_CATERPIE] = 
    { 
        .baseHP = 40, 
        .basePower = 50, 
        .baseDef = 35, 
        .move = DECK_BUG_BITE, 
        .ability = DECK_AGGRESSIVE, 
    
        .playerIdle = gCaterpiePlayerIdleGfx, 
        .opponentIdle = gCaterpieOpponentIdleGfx, 
        .playerAttack = gCaterpiePlayerAttackGfx, 
        .opponentAttack = gCaterpieOpponentAttackGfx, 
        .playerHurt = gCaterpiePlayerHurtGfx, 
        .opponentHurt = gCaterpieOpponentHurtGfx, 
        .objectPalette = gCaterpieObjectPal, 
        .playerYOffset = 2, 
        .opponentYOffset = 2, 
    
        .portrait = gCaterpiePortraitGfx, 
        .portraitPalette = gCaterpiePortraitPal, 
    },

    [SPECIES_METAPOD] = 
    { 
        .baseHP = 60, 
        .basePower = 60, 
        .baseDef = 60, 
        .move = DECK_BUG_BITE, 
        .ability = DECK_AGGRESSIVE, 
    
        .playerIdle = gMetapodPlayerIdleGfx, 
        .opponentIdle = gMetapodOpponentIdleGfx, 
        .playerAttack = gMetapodPlayerAttackGfx, 
        .opponentAttack = gMetapodOpponentAttackGfx, 
        .playerHurt = gMetapodPlayerHurtGfx, 
        .opponentHurt = gMetapodOpponentHurtGfx, 
        .objectPalette = gMetapodObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gMetapodPortraitGfx, 
        .portraitPalette = gMetapodPortraitPal, 
    },

    [SPECIES_BUTTERFREE] = 
    { 
        .baseHP = 60, 
        .basePower = 90,
        .baseDef = 60,
        .move = DECK_SILVER_WIND,
        .ability = DECK_ENERGETIC,
    
        .playerIdle = gButterfreePlayerIdleGfx, 
        .opponentIdle = gButterfreeOpponentIdleGfx, 
        .playerAttack = gButterfreePlayerAttackGfx, 
        .opponentAttack = gButterfreeOpponentAttackGfx, 
        .playerHurt = gButterfreePlayerHurtGfx, 
        .opponentHurt = gButterfreeOpponentHurtGfx, 
        .objectPalette = gButterfreeObjectPal, 
        .playerYOffset = -6, 
        .opponentYOffset = -6, 
    
        .portrait = gButterfreePortraitGfx, 
        .portraitPalette = gButterfreePortraitPal, 
    },

    [SPECIES_RATTATA] =
    {
        .baseHP = 50,
        .basePower = 60,
        .baseDef = 35,
        .move = DECK_QUICK_ATTACK,
        .ability = DECK_SOCIAL,

        .playerIdle = gRattataPlayerIdleGfx,
        .opponentIdle = gRattataOpponentIdleGfx,
        .playerAttack = gRattataPlayerAttackGfx,
        .opponentAttack = gRattataOpponentAttackGfx,
        .playerHurt = gRattataPlayerHurtGfx,
        .opponentHurt = gRattataOpponentHurtGfx,
        .objectPalette = gRattataObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = -2,

        .portrait = gRattataPortraitGfx,
        .portraitPalette = gRattataPortraitPal,
    },

    [SPECIES_RATICATE] = 
    { 
        .baseHP = 80, 
        .basePower = 100, 
        .baseDef = 50, 
        .move = DECK_QUICK_ATTACK, 
        .ability = DECK_SOCIAL, 
    
        .playerIdle = gRaticatePlayerIdleGfx, 
        .opponentIdle = gRaticateOpponentIdleGfx, 
        .playerAttack = gRaticatePlayerAttackGfx, 
        .opponentAttack = gRaticateOpponentAttackGfx, 
        .playerHurt = gRaticatePlayerHurtGfx, 
        .opponentHurt = gRaticateOpponentHurtGfx, 
        .objectPalette = gRaticateObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gRaticatePortraitGfx, 
        .portraitPalette = gRaticatePortraitPal, 
    },

    [SPECIES_EKANS] = 
    { 
        .baseHP = 80, 
        .basePower = 50, 
        .baseDef = 60, 
        .move = DECK_BITE, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gEkansPlayerIdleGfx, 
        .opponentIdle = gEkansOpponentIdleGfx, 
        .playerAttack = gEkansPlayerAttackGfx, 
        .opponentAttack = gEkansOpponentAttackGfx, 
        .playerHurt = gEkansPlayerHurtGfx, 
        .opponentHurt = gEkansOpponentHurtGfx, 
        .objectPalette = gEkansObjectPal, 
        .playerYOffset = -1,
        .opponentYOffset = -1,
    
        .portrait = gEkansPortraitGfx, 
        .portraitPalette = gEkansPortraitPal, 
    },

    [SPECIES_ARBOK] = 
    { 
        .baseHP = 100, 
        .basePower = 70, 
        .baseDef = 100, 
        .move = DECK_CRUNCH, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gArbokPlayerIdleGfx, 
        .opponentIdle = gArbokOpponentIdleGfx, 
        .playerAttack = gArbokPlayerAttackGfx, 
        .opponentAttack = gArbokOpponentAttackGfx, 
        .playerHurt = gArbokPlayerHurtGfx, 
        .opponentHurt = gArbokOpponentHurtGfx, 
        .objectPalette = gArbokObjectPal, 
        .playerYOffset = -2, 
        .opponentYOffset = -4, 
    
        .portrait = gArbokPortraitGfx, 
        .portraitPalette = gArbokPortraitPal, 
    },

    [SPECIES_SPEAROW] =
    {
        .baseHP = 50,
        .basePower = 60,
        .baseDef = 50,
        .move = DECK_SAND_ATTACK,
        .ability = DECK_AGGRESSIVE,

        .playerIdle = gSpearowPlayerIdleGfx,
        .opponentIdle = gSpearowOpponentIdleGfx,
        .playerAttack = gSpearowPlayerAttackGfx,
        .opponentAttack = gSpearowOpponentAttackGfx,
        .playerHurt = gSpearowPlayerHurtGfx,
        .opponentHurt = gSpearowOpponentHurtGfx,
        .objectPalette = gSpearowObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = -2,

        .portrait = gSpearowPortraitGfx,
        .portraitPalette = gSpearowPortraitPal,
    },

    [SPECIES_FEAROW] = 
    { 
        .baseHP = 90, 
        .basePower = 90, 
        .baseDef = 90, 
        .move = DECK_SCARY_FACE, 
        .ability = DECK_AGGRESSIVE,
    
        .playerIdle = gFearowPlayerIdleGfx, 
        .opponentIdle = gFearowOpponentIdleGfx, 
        .playerAttack = gFearowPlayerAttackGfx, 
        .opponentAttack = gFearowOpponentAttackGfx, 
        .playerHurt = gFearowPlayerHurtGfx, 
        .opponentHurt = gFearowOpponentHurtGfx, 
        .objectPalette = gFearowObjectPal, 
        .playerYOffset = -6, 
        .opponentYOffset = -6, 
    
        .portrait = gFearowPortraitGfx, 
        .portraitPalette = gFearowPortraitPal, 
    },

    [SPECIES_VULPIX] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gVulpixPlayerIdleGfx,
        .opponentIdle = gVulpixOpponentIdleGfx,
        .playerAttack = gVulpixPlayerAttackGfx,
        .opponentAttack = gVulpixOpponentAttackGfx,
        .playerHurt = gVulpixPlayerHurtGfx,
        .opponentHurt = gVulpixOpponentHurtGfx,
        .objectPalette = gVulpixObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = -2,

        .portrait = gVulpixPortraitGfx,
        .portraitPalette = gVulpixPortraitPal,
    },

    [SPECIES_BELLSPROUT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_VINE_WHIP,
        .ability = DECK_PATIENT,

        .playerIdle = gBellsproutPlayerIdleGfx,
        .opponentIdle = gBellsproutOpponentIdleGfx,
        .playerAttack = gBellsproutPlayerAttackGfx,
        .opponentAttack = gBellsproutOpponentAttackGfx,
        .playerHurt = gBellsproutPlayerHurtGfx,
        .opponentHurt = gBellsproutOpponentHurtGfx,
        .objectPalette = gBellsproutObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 1,

        .portrait = gBellsproutPortraitGfx,
        .portraitPalette = gBellsproutPortraitPal,
    },

    [SPECIES_WEEPINBELL] = // lol typo
    { 
        .baseHP = 75, 
        .basePower = 75, 
        .baseDef = 55, 
        .move = DECK_VINE_WHIP, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gWeepingbellPlayerIdleGfx, 
        .opponentIdle = gWeepingbellOpponentIdleGfx, 
        .playerAttack = gWeepingbellPlayerAttackGfx, 
        .opponentAttack = gWeepingbellOpponentAttackGfx, 
        .playerHurt = gWeepingbellPlayerHurtGfx, 
        .opponentHurt = gWeepingbellOpponentHurtGfx, 
        .objectPalette = gWeepingbellObjectPal, 
        .playerYOffset = -2, 
        .opponentYOffset = -2, 
    
        .portrait = gWeepingbellPortraitGfx, 
        .portraitPalette = gWeepingbellPortraitPal, 
    },

    [SPECIES_VICTREEBEL] = 
    { 
        .baseHP = 90, 
        .basePower = 100, 
        .baseDef = 90, 
        .move = DECK_FRENZY_PLANT, 
        .ability = DECK_PATIENT,
    
        .playerIdle = gVictreebelPlayerIdleGfx, 
        .opponentIdle = gVictreebelOpponentIdleGfx, 
        .playerAttack = gVictreebelPlayerAttackGfx, 
        .opponentAttack = gVictreebelOpponentAttackGfx, 
        .playerHurt = gVictreebelPlayerHurtGfx, 
        .opponentHurt = gVictreebelOpponentHurtGfx, 
        .objectPalette = gVictreebelObjectPal, 
        .playerYOffset = -4, 
        .opponentYOffset = -4, 
    
        .portrait = gVictreebelPortraitGfx, 
        .portraitPalette = gVictreebelPortraitPal, 
    },

    [SPECIES_ZUBAT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_BITE,
        .ability = DECK_VAMPIRIC,

        .playerIdle = gZubatPlayerIdleGfx,
        .opponentIdle = gZubatOpponentIdleGfx,
        .playerAttack = gZubatPlayerAttackGfx,
        .opponentAttack = gZubatOpponentAttackGfx,
        .playerHurt = gZubatPlayerHurtGfx,
        .opponentHurt = gZubatOpponentHurtGfx,
        .objectPalette = gZubatObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = -2,

        .portrait = gZubatPortraitGfx,
        .portraitPalette = gZubatPortraitPal,
    },

    [SPECIES_SLOWPOKE] =
    {
        .baseHP = 90,
        .basePower = 55,
        .baseDef = 55,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gSlowpokePlayerIdleGfx,
        .opponentIdle = gSlowpokeOpponentIdleGfx,
        .playerAttack = gSlowpokePlayerAttackGfx,
        .opponentAttack = gSlowpokeOpponentAttackGfx,
        .playerHurt = gSlowpokePlayerHurtGfx,
        .opponentHurt = gSlowpokeOpponentHurtGfx,
        .objectPalette = gSlowpokeObjectPal,
        .playerYOffset = -1,
        .opponentYOffset = -1,

        .portrait = gSlowpokePortraitGfx,
        .portraitPalette = gSlowpokePortraitPal,
    },

    [SPECIES_SLOWBRO] =
    {
        .baseHP = 95,
        .basePower = 90,
        .baseDef = 95,
        .move = DECK_SURF,
        .ability = ABILITY_NONE,

        .playerIdle = gSlowbroPlayerIdleGfx,
        .opponentIdle = gSlowbroOpponentIdleGfx,
        .playerAttack = gSlowbroPlayerAttackGfx,
        .opponentAttack = gSlowbroOpponentAttackGfx,
        .playerHurt = gSlowbroPlayerHurtGfx,
        .opponentHurt = gSlowbroOpponentHurtGfx,
        .objectPalette = gSlowbroObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = -4,

        .portrait = gSlowbroPortraitGfx,
        .portraitPalette = gSlowbroPortraitPal,
    },

    [SPECIES_KRABBY] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_CRABHAMMER,
        .ability = DECK_ALPHA,

        .playerIdle = gKrabbyPlayerIdleGfx,
        .opponentIdle = gKrabbyOpponentIdleGfx,
        .playerAttack = gKrabbyPlayerAttackGfx,
        .opponentAttack = gKrabbyOpponentAttackGfx,
        .playerHurt = gKrabbyPlayerHurtGfx,
        .opponentHurt = gKrabbyOpponentHurtGfx,
        .objectPalette = gKrabbyObjectPal,
        .playerYOffset = 1,
        .opponentYOffset = 1,

        .portrait = gKrabbyPortraitGfx,
        .portraitPalette = gKrabbyPortraitPal,
    },

    [SPECIES_PONYTA] = 
    { 
        .baseHP = 40, 
        .basePower = 70, 
        .baseDef = 40, 
        .move = DECK_FLAME_WHEEL, 
        .ability = DECK_AGGRESSIVE, 
    
        .playerIdle = gPonytaPlayerIdleGfx, 
        .opponentIdle = gPonytaOpponentIdleGfx, 
        .playerAttack = gPonytaPlayerAttackGfx, 
        .opponentAttack = gPonytaOpponentAttackGfx, 
        .playerHurt = gPonytaPlayerHurtGfx, 
        .opponentHurt = gPonytaOpponentHurtGfx, 
        .objectPalette = gPonytaObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gPonytaPortraitGfx, 
        .portraitPalette = gPonytaPortraitPal, 
    },

    [SPECIES_RAPIDASH] = 
    { 
        .baseHP = 70, 
        .basePower = 110, 
        .baseDef = 70, 
        .move = DECK_FLARE_BLITZ, 
        .ability = DECK_AGGRESSIVE, 
    
        .playerIdle = gRapidashPlayerIdleGfx, 
        .opponentIdle = gRapidashOpponentIdleGfx, 
        .playerAttack = gRapidashPlayerAttackGfx, 
        .opponentAttack = gRapidashOpponentAttackGfx, 
        .playerHurt = gRapidashPlayerHurtGfx, 
        .opponentHurt = gRapidashOpponentHurtGfx, 
        .objectPalette = gRapidashObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gRapidashPortraitGfx, 
        .portraitPalette = gRapidashPortraitPal, 
    },

    [SPECIES_MAREEP] =
    {
        .baseHP = 60,
        .basePower = 55,
        .baseDef = 45,
        .move = DECK_HELPING_HAND,
        .ability = DECK_ENERGETIC,

        .playerIdle = gMareepPlayerIdleGfx,
        .opponentIdle = gMareepOpponentIdleGfx,
        .playerAttack = gMareepPlayerAttackGfx,
        .opponentAttack = gMareepOpponentAttackGfx,
        .playerHurt = gMareepPlayerHurtGfx,
        .opponentHurt = gMareepOpponentHurtGfx,
        .objectPalette = gMareepObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gMareepPortraitGfx,
        .portraitPalette = gMareepPortraitPal,
    },

    [SPECIES_FLAAFFY] = 
    { 
        .baseHP = 80, 
        .basePower = 65, 
        .baseDef = 60, 
        .move = DECK_CHARGE, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gFlaaffyPlayerIdleGfx, 
        .opponentIdle = gFlaaffyOpponentIdleGfx, 
        .playerAttack = gFlaaffyPlayerAttackGfx, 
        .opponentAttack = gFlaaffyOpponentAttackGfx, 
        .playerHurt = gFlaaffyPlayerHurtGfx, 
        .opponentHurt = gFlaaffyOpponentHurtGfx, 
        .objectPalette = gFlaaffyObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gFlaaffyPortraitGfx, 
        .portraitPalette = gFlaaffyPortraitPal, 
    },

    [SPECIES_AMPHAROS] = 
    { 
        .baseHP = 90, 
        .basePower = 75, 
        .baseDef = 90, 
        .move = DECK_COTTON_GUARD, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gAmpharosPlayerIdleGfx, 
        .opponentIdle = gAmpharosOpponentIdleGfx, 
        .playerAttack = gAmpharosPlayerAttackGfx, 
        .opponentAttack = gAmpharosOpponentAttackGfx, 
        .playerHurt = gAmpharosPlayerHurtGfx, 
        .opponentHurt = gAmpharosOpponentHurtGfx, 
        .objectPalette = gAmpharosObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gAmpharosPortraitGfx, 
        .portraitPalette = gAmpharosPortraitPal, 
    },

    [SPECIES_HOPPIP] = 
    { 
        .baseHP = 50, 
        .basePower = 60, 
        .baseDef = 30, 
        .move = DECK_WHIRLWIND, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gHoppipPlayerIdleGfx, 
        .opponentIdle = gHoppipOpponentIdleGfx, 
        .playerAttack = gHoppipPlayerAttackGfx, 
        .opponentAttack = gHoppipOpponentAttackGfx, 
        .playerHurt = gHoppipPlayerHurtGfx, 
        .opponentHurt = gHoppipOpponentHurtGfx, 
        .objectPalette = gHoppipObjectPal, 
        .playerYOffset = -4, 
        .opponentYOffset = -4, 
    
        .portrait = gHoppipPortraitGfx, 
        .portraitPalette = gHoppipPortraitPal, 
    },

    [SPECIES_SKIPLOOM] = 
    { 
        .baseHP = 60, 
        .basePower = 90, 
        .baseDef = 40, 
        .move = DECK_WHIRLWIND, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gSkiploomPlayerIdleGfx, 
        .opponentIdle = gSkiploomOpponentIdleGfx, 
        .playerAttack = gSkiploomPlayerAttackGfx, 
        .opponentAttack = gSkiploomOpponentAttackGfx, 
        .playerHurt = gSkiploomPlayerHurtGfx, 
        .opponentHurt = gSkiploomOpponentHurtGfx, 
        .objectPalette = gSkiploomObjectPal, 
        .playerYOffset = -4, 
        .opponentYOffset = -4, 
    
        .portrait = gSkiploomPortraitGfx, 
        .portraitPalette = gSkiploomPortraitPal, 
    },

    [SPECIES_JUMPLUFF] = 
    { 
        .baseHP = 60, 
        .basePower = 100, 
        .baseDef = 60, 
        .move = DECK_WHIRLWIND, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gJumpluffPlayerIdleGfx, 
        .opponentIdle = gJumpluffOpponentIdleGfx, 
        .playerAttack = gJumpluffPlayerAttackGfx, 
        .opponentAttack = gJumpluffOpponentAttackGfx, 
        .playerHurt = gJumpluffPlayerHurtGfx, 
        .opponentHurt = gJumpluffOpponentHurtGfx, 
        .objectPalette = gJumpluffObjectPal, 
        .playerYOffset = -4, 
        .opponentYOffset = -4, 
    
        .portrait = gJumpluffPortraitGfx, 
        .portraitPalette = gJumpluffPortraitPal, 
    },

    [SPECIES_CLEFFA] =
    {
        .baseHP = 35,
        .basePower = 80,
        .baseDef = 25,
        .move = DECK_HELPING_HAND,
        .ability = ABILITY_NONE,

        .playerIdle = gCleffaPlayerIdleGfx,
        .opponentIdle = gCleffaOpponentIdleGfx,
        .playerAttack = gCleffaPlayerAttackGfx,
        .opponentAttack = gCleffaOpponentAttackGfx,
        .playerHurt = gCleffaPlayerHurtGfx,
        .opponentHurt = gCleffaOpponentHurtGfx,
        .objectPalette = gCleffaObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gCleffaPortraitGfx,
        .portraitPalette = gCleffaPortraitPal,
    },

    [SPECIES_SWINUB] =
    {
        .baseHP = 50,
        .basePower = 40,
        .baseDef = 40,
        .move = DECK_EARTHQUAKE,
        .ability = ABILITY_NONE,

        .playerIdle = gSwinubPlayerIdleGfx,
        .opponentIdle = gSwinubOpponentIdleGfx,
        .playerAttack = gSwinubPlayerAttackGfx,
        .opponentAttack = gSwinubOpponentAttackGfx,
        .playerHurt = gSwinubPlayerHurtGfx,
        .opponentHurt = gSwinubOpponentHurtGfx,
        .objectPalette = gSwinubObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = -1,

        .portrait = gSwinubPortraitGfx,
        .portraitPalette = gSwinubPortraitPal,
    },

    [SPECIES_POOCHYENA] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_BITE,
        .ability = DECK_ALPHA,

        .playerIdle = gPoochyenaPlayerIdleGfx,
        .opponentIdle = gPoochyenaOpponentIdleGfx,
        .playerAttack = gPoochyenaPlayerAttackGfx,
        .opponentAttack = gPoochyenaOpponentAttackGfx,
        .playerHurt = gPoochyenaPlayerHurtGfx,
        .opponentHurt = gPoochyenaOpponentHurtGfx,
        .objectPalette = gPoochyenaObjectPal,
        .playerYOffset = -4,
        .opponentYOffset = -2,

        .portrait = gPoochyenaPortraitGfx,
        .portraitPalette = gPoochyenaPortraitPal,
    },

    [SPECIES_MIGHTYENA] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = DECK_CRUNCH,
        .ability = DECK_ALPHA, 
    
        .playerIdle = gMightyenaPlayerIdleGfx, 
        .opponentIdle = gMightyenaOpponentIdleGfx, 
        .playerAttack = gMightyenaPlayerAttackGfx, 
        .opponentAttack = gMightyenaOpponentAttackGfx, 
        .playerHurt = gMightyenaPlayerHurtGfx, 
        .opponentHurt = gMightyenaOpponentHurtGfx, 
        .objectPalette = gMightyenaObjectPal, 
        .playerYOffset = -3, 
        .opponentYOffset = -3, 
    
        .portrait = gMightyenaPortraitGfx, 
        .portraitPalette = gMightyenaPortraitPal, 
    },

    [SPECIES_SWABLU] =
    {
        .baseHP = 45,
        .basePower = 40,
        .baseDef = 70,
        .move = DECK_HELPING_HAND,
        .ability = DECK_RESILIENT,

        .playerIdle = gSwabluPlayerIdleGfx,
        .opponentIdle = gSwabluOpponentIdleGfx,
        .playerAttack = gSwabluPlayerAttackGfx,
        .opponentAttack = gSwabluOpponentAttackGfx,
        .playerHurt = gSwabluPlayerHurtGfx,
        .opponentHurt = gSwabluOpponentHurtGfx,
        .objectPalette = gSwabluObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gSwabluPortraitGfx,
        .portraitPalette = gSwabluPortraitPal,
    },

    [SPECIES_ALTARIA] = 
    { 
        .baseHP = 70, 
        .basePower = 70, 
        .baseDef = 120, 
        .move = DECK_COTTON_GUARD, 
        .ability = DECK_RESILIENT, 
    
        .playerIdle = gAltariaPlayerIdleGfx, 
        .opponentIdle = gAltariaOpponentIdleGfx, 
        .playerAttack = gAltariaPlayerAttackGfx, 
        .opponentAttack = gAltariaOpponentAttackGfx, 
        .playerHurt = gAltariaPlayerHurtGfx, 
        .opponentHurt = gAltariaOpponentHurtGfx, 
        .objectPalette = gAltariaObjectPal, 
        .playerYOffset = -3, 
        .opponentYOffset = -3, 
    
        .portrait = gAltariaPortraitGfx, 
        .portraitPalette = gAltariaPortraitPal, 
    },

    [SPECIES_BALTOY] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gBaltoyPlayerIdleGfx,
        .opponentIdle = gBaltoyOpponentIdleGfx,
        .playerAttack = gBaltoyPlayerAttackGfx,
        .opponentAttack = gBaltoyOpponentAttackGfx,
        .playerHurt = gBaltoyPlayerHurtGfx,
        .opponentHurt = gBaltoyOpponentHurtGfx,
        .objectPalette = gBaltoyObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = -3,

        .portrait = gBaltoyPortraitGfx,
        .portraitPalette = gBaltoyPortraitPal,
    },

    [SPECIES_BUDEW] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = DECK_VINE_WHIP, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gBudewPlayerIdleGfx, 
        .opponentIdle = gBudewOpponentIdleGfx, 
        .playerAttack = gBudewPlayerAttackGfx, 
        .opponentAttack = gBudewOpponentAttackGfx, 
        .playerHurt = gBudewPlayerHurtGfx, 
        .opponentHurt = gBudewOpponentHurtGfx, 
        .objectPalette = gBudewObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gBudewPortraitGfx, 
        .portraitPalette = gBudewPortraitPal, 
    },

    [SPECIES_CHINGLING] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_HEAL_BELL,
        .ability = DECK_HEAVY,

        .playerIdle = gChinglingPlayerIdleGfx,
        .opponentIdle = gChinglingOpponentIdleGfx,
        .playerAttack = gChinglingPlayerAttackGfx,
        .opponentAttack = gChinglingOpponentAttackGfx,
        .playerHurt = gChinglingPlayerHurtGfx,
        .opponentHurt = gChinglingOpponentHurtGfx,
        .objectPalette = gChinglingObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = 0,

        .portrait = gChinglingPortraitGfx,
        .portraitPalette = gChinglingPortraitPal,
    },

    [SPECIES_CHIMECHO] = 
    { 
        .baseHP = 90, 
        .basePower = 100, 
        .baseDef = 70, 
        .move = DECK_HEAL_BELL, 
        .ability = DECK_HEAVY, 
    
        .playerIdle = gChimechoPlayerIdleGfx, 
        .opponentIdle = gChimechoOpponentIdleGfx, 
        .playerAttack = gChimechoPlayerAttackGfx, 
        .opponentAttack = gChimechoOpponentAttackGfx, 
        .playerHurt = gChimechoPlayerHurtGfx, 
        .opponentHurt = gChimechoOpponentHurtGfx, 
        .objectPalette = gChimechoObjectPal, 
        .playerYOffset = -4, 
        .opponentYOffset = -4, 
    
        .portrait = gChimechoPortraitGfx, 
        .portraitPalette = gChimechoPortraitPal, 
    },

    [SPECIES_DEINO] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gDeinoPlayerIdleGfx,
        .opponentIdle = gDeinoOpponentIdleGfx,
        .playerAttack = gDeinoPlayerAttackGfx,
        .opponentAttack = gDeinoOpponentAttackGfx,
        .playerHurt = gDeinoPlayerHurtGfx,
        .opponentHurt = gDeinoOpponentHurtGfx,
        .objectPalette = gDeinoObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = 0,

        .portrait = gDeinoPortraitGfx,
        .portraitPalette = gDeinoPortraitPal,
    },

    [SPECIES_GOOMY] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gGoomyPlayerIdleGfx,
        .opponentIdle = gGoomyOpponentIdleGfx,
        .playerAttack = gGoomyPlayerAttackGfx,
        .opponentAttack = gGoomyOpponentAttackGfx,
        .playerHurt = gGoomyPlayerHurtGfx,
        .opponentHurt = gGoomyOpponentHurtGfx,
        .objectPalette = gGoomyObjectPal,
        .playerYOffset = 1,
        .opponentYOffset = 2,

        .portrait = gGoomyPortraitGfx,
        .portraitPalette = gGoomyPortraitPal,
    },

    [SPECIES_APPLIN] =
    {
        .baseHP = 40,
        .basePower = 40,
        .baseDef = 60,
        .move = DECK_NOURISH,
        .ability = ABILITY_NONE,

        .playerIdle = gApplinPlayerIdleGfx,
        .opponentIdle = gApplinOpponentIdleGfx,
        .playerAttack = gApplinPlayerAttackGfx,
        .opponentAttack = gApplinOpponentAttackGfx,
        .playerHurt = gApplinPlayerHurtGfx,
        .opponentHurt = gApplinOpponentHurtGfx,
        .objectPalette = gApplinObjectPal,
        .playerYOffset = 0,
        .opponentYOffset = -1,

        .portrait = gApplinPortraitGfx,
        .portraitPalette = gApplinPortraitPal,
    },

    [SPECIES_JANGMO_O] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gJangmoOPlayerIdleGfx,
        .opponentIdle = gJangmoOOpponentIdleGfx,
        .playerAttack = gJangmoOPlayerAttackGfx,
        .opponentAttack = gJangmoOOpponentAttackGfx,
        .playerHurt = gJangmoOPlayerHurtGfx,
        .opponentHurt = gJangmoOOpponentHurtGfx,
        .objectPalette = gJangmoOObjectPal,
        .playerYOffset = -2,
        .opponentYOffset = 0,

        .portrait = gJangmoOPortraitGfx,
        .portraitPalette = gJangmoOPortraitPal,
    },

    [SPECIES_WIGLETT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = DECK_TACKLE,
        .ability = ABILITY_NONE,

        .playerIdle = gWiglettPlayerIdleGfx,
        .opponentIdle = gWiglettOpponentIdleGfx,
        .playerAttack = gWiglettPlayerAttackGfx,
        .opponentAttack = gWiglettOpponentAttackGfx,
        .playerHurt = gWiglettPlayerHurtGfx,
        .opponentHurt = gWiglettOpponentHurtGfx,
        .objectPalette = gWiglettObjectPal,
        .playerYOffset = 3,
        .opponentYOffset = 3,

        .portrait = gWiglettPortraitGfx,
        .portraitPalette = gWiglettPortraitPal,
    },

    [SPECIES_SHUCKLE] = 
    { 
        .baseHP = 50, 
        .basePower = 60, 
        .baseDef = 150, 
        .move = DECK_HARDEN, 
        .ability = DECK_HEAVY, 
    
        .playerIdle = gShucklePlayerIdleGfx, 
        .opponentIdle = gShuckleOpponentIdleGfx, 
        .playerAttack = gShucklePlayerAttackGfx, 
        .opponentAttack = gShuckleOpponentAttackGfx, 
        .playerHurt = gShucklePlayerHurtGfx, 
        .opponentHurt = gShuckleOpponentHurtGfx, 
        .objectPalette = gShuckleObjectPal, 
        .playerYOffset = 2,
        .opponentYOffset = 2,
    
        .portrait = gShucklePortraitGfx, 
        .portraitPalette = gShucklePortraitPal, 
    },

    [SPECIES_GASTLY] = 
    { 
        .baseHP = 40, 
        .basePower = 70, 
        .baseDef = 40, 
        .move = DECK_LICK, 
        .ability = DECK_TRICKY, 
    
        .playerIdle = gGastlyPlayerIdleGfx, 
        .opponentIdle = gGastlyOpponentIdleGfx, 
        .playerAttack = gGastlyPlayerAttackGfx, 
        .opponentAttack = gGastlyOpponentAttackGfx, 
        .playerHurt = gGastlyPlayerHurtGfx, 
        .opponentHurt = gGastlyOpponentHurtGfx, 
        .objectPalette = gGastlyObjectPal, 
        .playerYOffset = -5, 
        .opponentYOffset = -5, 
    
        .portrait = gGastlyPortraitGfx, 
        .portraitPalette = gGastlyPortraitPal, 
    },

    [SPECIES_HAUNTER] = 
    { 
        .baseHP = 60, 
        .basePower = 100, 
        .baseDef = 40, 
        .move = DECK_LICK, 
        .ability = DECK_TRICKY, 
    
        .playerIdle = gHaunterPlayerIdleGfx, 
        .opponentIdle = gHaunterOpponentIdleGfx, 
        .playerAttack = gHaunterPlayerAttackGfx, 
        .opponentAttack = gHaunterOpponentAttackGfx, 
        .playerHurt = gHaunterPlayerHurtGfx, 
        .opponentHurt = gHaunterOpponentHurtGfx, 
        .objectPalette = gHaunterObjectPal, 
        .playerYOffset = -3, 
        .opponentYOffset = -3, 
    
        .portrait = gHaunterPortraitGfx, 
        .portraitPalette = gHaunterPortraitPal, 
    },

    [SPECIES_GENGAR] = 
    { 
        .baseHP = 90, 
        .basePower = 140, 
        .baseDef = 50, 
        .move = DECK_LICK,
        .ability = DECK_TRICKY, 
    
        .playerIdle = gGengarPlayerIdleGfx, 
        .opponentIdle = gGengarOpponentIdleGfx, 
        .playerAttack = gGengarPlayerAttackGfx, 
        .opponentAttack = gGengarOpponentAttackGfx, 
        .playerHurt = gGengarPlayerHurtGfx, 
        .opponentHurt = gGengarOpponentHurtGfx, 
        .objectPalette = gGengarObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gGengarPortraitGfx, 
        .portraitPalette = gGengarPortraitPal, 
    },

    [SPECIES_PARAS] = 
    { 
        .baseHP = 60, 
        .basePower = 60, 
        .baseDef = 60, 
        .move = DECK_AROMATHERAPY, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gParasPlayerIdleGfx, 
        .opponentIdle = gParasOpponentIdleGfx, 
        .playerAttack = gParasPlayerAttackGfx, 
        .opponentAttack = gParasOpponentAttackGfx, 
        .playerHurt = gParasPlayerHurtGfx, 
        .opponentHurt = gParasOpponentHurtGfx, 
        .objectPalette = gParasObjectPal, 
        .playerYOffset = 2, 
        .opponentYOffset = 2, 
    
        .portrait = gParasPortraitGfx, 
        .portraitPalette = gParasPortraitPal, 
    },

    [SPECIES_PARASECT] = 
    { 
        .baseHP = 90, 
        .basePower = 70, 
        .baseDef = 90, 
        .move = DECK_AROMATHERAPY, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gParasectPlayerIdleGfx, 
        .opponentIdle = gParasectOpponentIdleGfx, 
        .playerAttack = gParasectPlayerAttackGfx, 
        .opponentAttack = gParasectOpponentAttackGfx, 
        .playerHurt = gParasectPlayerHurtGfx, 
        .opponentHurt = gParasectOpponentHurtGfx, 
        .objectPalette = gParasectObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gParasectPortraitGfx, 
        .portraitPalette = gParasectPortraitPal, 
    },

    [SPECIES_SHROOMISH] = 
    { 
        .baseHP = 60, 
        .basePower = 60, 
        .baseDef = 60,
        .move = DECK_HEADBUTT,
        .ability = DECK_REGENERATIVE, 
    
        .playerIdle = gShroomishPlayerIdleGfx, 
        .opponentIdle = gShroomishOpponentIdleGfx, 
        .playerAttack = gShroomishPlayerAttackGfx, 
        .opponentAttack = gShroomishOpponentAttackGfx, 
        .playerHurt = gShroomishPlayerHurtGfx, 
        .opponentHurt = gShroomishOpponentHurtGfx, 
        .objectPalette = gShroomishObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gShroomishPortraitGfx, 
        .portraitPalette = gShroomishPortraitPal, 
    },

    [SPECIES_BRELOOM] = 
    { 
        .baseHP = 80, 
        .basePower = 90, 
        .baseDef = 80, 
        .move = DECK_FOCUS_PUNCH, 
        .ability = DECK_REGENERATIVE, 
    
        .playerIdle = gBreloomPlayerIdleGfx, 
        .opponentIdle = gBreloomOpponentIdleGfx, 
        .playerAttack = gBreloomPlayerAttackGfx, 
        .opponentAttack = gBreloomOpponentAttackGfx, 
        .playerHurt = gBreloomPlayerHurtGfx, 
        .opponentHurt = gBreloomOpponentHurtGfx, 
        .objectPalette = gBreloomObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gBreloomPortraitGfx, 
        .portraitPalette = gBreloomPortraitPal, 
    },

    [SPECIES_TANGELA] = 
    { 
        .baseHP = 50, 
        .basePower = 60, 
        .baseDef = 100, 
        .move = DECK_TANGLE,
        .ability = DECK_REGENERATIVE, 
    
        .playerIdle = gTangelaPlayerIdleGfx, 
        .opponentIdle = gTangelaOpponentIdleGfx, 
        .playerAttack = gTangelaPlayerAttackGfx, 
        .opponentAttack = gTangelaOpponentAttackGfx, 
        .playerHurt = gTangelaPlayerHurtGfx, 
        .opponentHurt = gTangelaOpponentHurtGfx, 
        .objectPalette = gTangelaObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gTangelaPortraitGfx, 
        .portraitPalette = gTangelaPortraitPal, 
    },

    [SPECIES_TANGROWTH] = 
    { 
        .baseHP = 60, 
        .basePower = 80, 
        .baseDef = 120, 
        .move = DECK_TANGLE, 
        .ability = DECK_REGENERATIVE, 
    
        .playerIdle = gTangrowthPlayerIdleGfx, 
        .opponentIdle = gTangrowthOpponentIdleGfx, 
        .playerAttack = gTangrowthPlayerAttackGfx, 
        .opponentAttack = gTangrowthOpponentAttackGfx, 
        .playerHurt = gTangrowthPlayerHurtGfx, 
        .opponentHurt = gTangrowthOpponentHurtGfx, 
        .objectPalette = gTangrowthObjectPal, 
        .playerYOffset = -4, 
        .opponentYOffset = -4, 
    
        .portrait = gTangrowthPortraitGfx, 
        .portraitPalette = gTangrowthPortraitPal, 
    },

    [SPECIES_SLAKOTH] = 
    { 
        .baseHP = 70, 
        .basePower = 70, 
        .baseDef = 50, 
        .move = DECK_HEADBUTT, 
        .ability = DECK_HEAVY, 
    
        .playerIdle = gSlakothPlayerIdleGfx, 
        .opponentIdle = gSlakothOpponentIdleGfx, 
        .playerAttack = gSlakothPlayerAttackGfx, 
        .opponentAttack = gSlakothOpponentAttackGfx, 
        .playerHurt = gSlakothPlayerHurtGfx, 
        .opponentHurt = gSlakothOpponentHurtGfx, 
        .objectPalette = gSlakothObjectPal, 
        .playerYOffset = 4, 
        .opponentYOffset = 4, 
    
        .portrait = gSlakothPortraitGfx, 
        .portraitPalette = gSlakothPortraitPal, 
    },

    [SPECIES_VIGOROTH] = 
    { 
        .baseHP = 90, 
        .basePower = 80, 
        .baseDef = 70, 
        .move = DECK_HEADBUTT, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gVigorothPlayerIdleGfx, 
        .opponentIdle = gVigorothOpponentIdleGfx, 
        .playerAttack = gVigorothPlayerAttackGfx, 
        .opponentAttack = gVigorothOpponentAttackGfx, 
        .playerHurt = gVigorothPlayerHurtGfx, 
        .opponentHurt = gVigorothOpponentHurtGfx, 
        .objectPalette = gVigorothObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gVigorothPortraitGfx, 
        .portraitPalette = gVigorothPortraitPal, 
    },

    [SPECIES_SLAKING] = 
    { 
        .baseHP = 100, 
        .basePower = 120, 
        .baseDef = 100, 
        .move = DECK_GIGA_IMPACT, 
        .ability = DECK_HEAVY, 
    
        .playerIdle = gSlakingPlayerIdleGfx, 
        .opponentIdle = gSlakingOpponentIdleGfx, 
        .playerAttack = gSlakingPlayerAttackGfx, 
        .opponentAttack = gSlakingOpponentAttackGfx, 
        .playerHurt = gSlakingPlayerHurtGfx, 
        .opponentHurt = gSlakingOpponentHurtGfx, 
        .objectPalette = gSlakingObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gSlakingPortraitGfx, 
        .portraitPalette = gSlakingPortraitPal, 
    },

    [SPECIES_HIPPOPOTAS] = 
    { 
        .baseHP = 80, 
        .basePower = 60, 
        .baseDef = 60, 
        .move = DECK_EARTHQUAKE, 
        .ability = DECK_SHORT_TEMPERED, 
    
        .playerIdle = gHippopotasPlayerIdleGfx, 
        .opponentIdle = gHippopotasOpponentIdleGfx, 
        .playerAttack = gHippopotasPlayerAttackGfx, 
        .opponentAttack = gHippopotasOpponentAttackGfx, 
        .playerHurt = gHippopotasPlayerHurtGfx, 
        .opponentHurt = gHippopotasOpponentHurtGfx, 
        .objectPalette = gHippopotasObjectPal, 
        .playerYOffset = 1, 
        .opponentYOffset = 1, 
    
        .portrait = gHippopotasPortraitGfx, 
        .portraitPalette = gHippopotasPortraitPal, 
    },

    [SPECIES_HIPPOWDON] = 
    { 
        .baseHP = 100, 
        .basePower = 80, 
        .baseDef = 80, 
        .move = DECK_EARTHQUAKE, 
        .ability = DECK_SHORT_TEMPERED, 
    
        .playerIdle = gHippowdonPlayerIdleGfx, 
        .opponentIdle = gHippowdonOpponentIdleGfx, 
        .playerAttack = gHippowdonPlayerAttackGfx, 
        .opponentAttack = gHippowdonOpponentAttackGfx, 
        .playerHurt = gHippowdonPlayerHurtGfx, 
        .opponentHurt = gHippowdonOpponentHurtGfx, 
        .objectPalette = gHippowdonObjectPal, 
        .playerYOffset = -3, 
        .opponentYOffset = 0, 
    
        .portrait = gHippowdonPortraitGfx, 
        .portraitPalette = gHippowdonPortraitPal, 
    },

    [SPECIES_KINGLER] = 
    { 
        .baseHP = 70, 
        .basePower = 120, 
        .baseDef = 70, 
        .move = DECK_CRABHAMMER, 
        .ability = DECK_ALPHA, 
    
        .playerIdle = gKinglerPlayerIdleGfx, 
        .opponentIdle = gKinglerOpponentIdleGfx, 
        .playerAttack = gKinglerPlayerAttackGfx, 
        .opponentAttack = gKinglerOpponentAttackGfx, 
        .playerHurt = gKinglerPlayerHurtGfx, 
        .opponentHurt = gKinglerOpponentHurtGfx, 
        .objectPalette = gKinglerObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gKinglerPortraitGfx, 
        .portraitPalette = gKinglerPortraitPal, 
    },

    [SPECIES_CLAYDOL] = 
    { 
        .baseHP = 100, 
        .basePower = 70, 
        .baseDef = 80, 
        .move = DECK_PSYCHO_SHIFT, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gClaydolPlayerIdleGfx, 
        .opponentIdle = gClaydolOpponentIdleGfx, 
        .playerAttack = gClaydolPlayerAttackGfx, 
        .opponentAttack = gClaydolOpponentAttackGfx, 
        .playerHurt = gClaydolPlayerHurtGfx, 
        .opponentHurt = gClaydolOpponentHurtGfx, 
        .objectPalette = gClaydolObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gClaydolPortraitGfx, 
        .portraitPalette = gClaydolPortraitPal, 
    },

    [SPECIES_DIGLETT] = 
    { 
        .baseHP = 40, 
        .basePower = 60, 
        .baseDef = 40, 
        .move = DECK_BULLDOZE, 
        .ability = DECK_AGGRESSIVE, 
    
        .playerIdle = gDiglettPlayerIdleGfx, 
        .opponentIdle = gDiglettOpponentIdleGfx, 
        .playerAttack = gDiglettPlayerAttackGfx, 
        .opponentAttack = gDiglettOpponentAttackGfx, 
        .playerHurt = gDiglettPlayerHurtGfx, 
        .opponentHurt = gDiglettOpponentHurtGfx, 
        .objectPalette = gDiglettObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gDiglettPortraitGfx, 
        .portraitPalette = gDiglettPortraitPal, 
    },

    [SPECIES_DUGTRIO] = 
    { 
        .baseHP = 70, 
        .basePower = 100, 
        .baseDef = 60, 
        .move = DECK_EARTHQUAKE, 
        .ability = DECK_AGGRESSIVE, 
    
        .playerIdle = gDugtrioPlayerIdleGfx, 
        .opponentIdle = gDugtrioOpponentIdleGfx, 
        .playerAttack = gDugtrioPlayerAttackGfx, 
        .opponentAttack = gDugtrioOpponentAttackGfx, 
        .playerHurt = gDugtrioPlayerHurtGfx, 
        .opponentHurt = gDugtrioOpponentHurtGfx, 
        .objectPalette = gDugtrioObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gDugtrioPortraitGfx, 
        .portraitPalette = gDugtrioPortraitPal, 
    },

    [SPECIES_TRAPINCH] = 
    { 
        .baseHP = 60, 
        .basePower = 50, 
        .baseDef = 50, 
        .move = DECK_BITE, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gTrapinchPlayerIdleGfx, 
        .opponentIdle = gTrapinchOpponentIdleGfx, 
        .playerAttack = gTrapinchPlayerAttackGfx, 
        .opponentAttack = gTrapinchOpponentAttackGfx, 
        .playerHurt = gTrapinchPlayerHurtGfx, 
        .opponentHurt = gTrapinchOpponentHurtGfx, 
        .objectPalette = gTrapinchObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gTrapinchPortraitGfx, 
        .portraitPalette = gTrapinchPortraitPal, 
    },

    [SPECIES_VIBRAVA] = 
    { 
        .baseHP = 80, 
        .basePower = 70, 
        .baseDef = 70, 
        .move = DECK_BITE, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gVibravaPlayerIdleGfx, 
        .opponentIdle = gVibravaOpponentIdleGfx, 
        .playerAttack = gVibravaPlayerAttackGfx, 
        .opponentAttack = gVibravaOpponentAttackGfx, 
        .playerHurt = gVibravaPlayerHurtGfx, 
        .opponentHurt = gVibravaOpponentHurtGfx, 
        .objectPalette = gVibravaObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gVibravaPortraitGfx, 
        .portraitPalette = gVibravaPortraitPal, 
    },

    [SPECIES_FLYGON] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = DECK_HURRICANE, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gFlygonPlayerIdleGfx, 
        .opponentIdle = gFlygonOpponentIdleGfx, 
        .playerAttack = gFlygonPlayerAttackGfx, 
        .opponentAttack = gFlygonOpponentAttackGfx, 
        .playerHurt = gFlygonPlayerHurtGfx, 
        .opponentHurt = gFlygonOpponentHurtGfx, 
        .objectPalette = gFlygonObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gFlygonPortraitGfx, 
        .portraitPalette = gFlygonPortraitPal, 
    },

    [SPECIES_GLIGAR] = 
    { 
        .baseHP = 80, 
        .basePower = 50, 
        .baseDef = 60, 
        .move = DECK_SAND_ATTACK, 
        .ability = DECK_RESILIENT, 
    
        .playerIdle = gGligarPlayerIdleGfx, 
        .opponentIdle = gGligarOpponentIdleGfx, 
        .playerAttack = gGligarPlayerAttackGfx, 
        .opponentAttack = gGligarOpponentAttackGfx, 
        .playerHurt = gGligarPlayerHurtGfx, 
        .opponentHurt = gGligarOpponentHurtGfx, 
        .objectPalette = gGligarObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gGligarPortraitGfx, 
        .portraitPalette = gGligarPortraitPal, 
    },

    [SPECIES_GLISCOR] = 
    { 
        .baseHP = 100, 
        .basePower = 70, 
        .baseDef = 80, 
        .move = DECK_SANDSTORM, 
        .ability = DECK_RESILIENT, 
    
        .playerIdle = gGliscorPlayerIdleGfx, 
        .opponentIdle = gGliscorOpponentIdleGfx, 
        .playerAttack = gGliscorPlayerAttackGfx, 
        .opponentAttack = gGliscorOpponentAttackGfx, 
        .playerHurt = gGliscorPlayerHurtGfx, 
        .opponentHurt = gGliscorOpponentHurtGfx, 
        .objectPalette = gGliscorObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gGliscorPortraitGfx, 
        .portraitPalette = gGliscorPortraitPal, 
    },

    [SPECIES_WHISMUR] = 
    { 
        .baseHP = 50, 
        .basePower = 60, 
        .baseDef = 50, 
        .move = DECK_HYPER_VOICE, 
        .ability = DECK_ALPHA, 
    
        .playerIdle = gWhismurPlayerIdleGfx, 
        .opponentIdle = gWhismurOpponentIdleGfx, 
        .playerAttack = gWhismurPlayerAttackGfx, 
        .opponentAttack = gWhismurOpponentAttackGfx, 
        .playerHurt = gWhismurPlayerHurtGfx, 
        .opponentHurt = gWhismurOpponentHurtGfx, 
        .objectPalette = gWhismurObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gWhismurPortraitGfx, 
        .portraitPalette = gWhismurPortraitPal, 
    },

    [SPECIES_LOUDRED] = 
    { 
        .baseHP = 60, 
        .basePower = 80, 
        .baseDef = 60, 
        .move = DECK_HYPER_VOICE, 
        .ability = DECK_ALPHA, 
    
        .playerIdle = gLoudredPlayerIdleGfx, 
        .opponentIdle = gLoudredOpponentIdleGfx, 
        .playerAttack = gLoudredPlayerAttackGfx, 
        .opponentAttack = gLoudredOpponentAttackGfx, 
        .playerHurt = gLoudredPlayerHurtGfx, 
        .opponentHurt = gLoudredOpponentHurtGfx, 
        .objectPalette = gLoudredObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gLoudredPortraitGfx, 
        .portraitPalette = gLoudredPortraitPal, 
    },

    [SPECIES_EXPLOUD] = 
    { 
        .baseHP = 100, 
        .basePower = 90, 
        .baseDef = 70, 
        .move = DECK_BOOMBURST, 
        .ability = DECK_ALPHA, 
    
        .playerIdle = gExploudPlayerIdleGfx, 
        .opponentIdle = gExploudOpponentIdleGfx, 
        .playerAttack = gExploudPlayerAttackGfx, 
        .opponentAttack = gExploudOpponentAttackGfx, 
        .playerHurt = gExploudPlayerHurtGfx, 
        .opponentHurt = gExploudOpponentHurtGfx, 
        .objectPalette = gExploudObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gExploudPortraitGfx, 
        .portraitPalette = gExploudPortraitPal, 
    },

    [SPECIES_SANDSHREW] = 
    { 
        .baseHP = 50, 
        .basePower = 50, 
        .baseDef = 70, 
        .move = DECK_HARDEN, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gSandshrewPlayerIdleGfx, 
        .opponentIdle = gSandshrewOpponentIdleGfx, 
        .playerAttack = gSandshrewPlayerAttackGfx, 
        .opponentAttack = gSandshrewOpponentAttackGfx, 
        .playerHurt = gSandshrewPlayerHurtGfx, 
        .opponentHurt = gSandshrewOpponentHurtGfx, 
        .objectPalette = gSandshrewObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gSandshrewPortraitGfx, 
        .portraitPalette = gSandshrewPortraitPal, 
    },

    [SPECIES_SANDSLASH] = 
    { 
        .baseHP = 70, 
        .basePower = 80, 
        .baseDef = 80, 
        .move = DECK_IRON_DEFENSE, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gSandslashPlayerIdleGfx, 
        .opponentIdle = gSandslashOpponentIdleGfx, 
        .playerAttack = gSandslashPlayerAttackGfx, 
        .opponentAttack = gSandslashOpponentAttackGfx, 
        .playerHurt = gSandslashPlayerHurtGfx, 
        .opponentHurt = gSandslashOpponentHurtGfx, 
        .objectPalette = gSandslashObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gSandslashPortraitGfx, 
        .portraitPalette = gSandslashPortraitPal, 
    },

    [SPECIES_PINSIR] = 
    { 
        .baseHP = 70, 
        .basePower = 100, 
        .baseDef = 70, 
        .move = DECK_BUG_BITE, 
        .ability = DECK_ALPHA, 
    
        .playerIdle = gPinsirPlayerIdleGfx, 
        .opponentIdle = gPinsirOpponentIdleGfx, 
        .playerAttack = gPinsirPlayerAttackGfx, 
        .opponentAttack = gPinsirOpponentAttackGfx, 
        .playerHurt = gPinsirPlayerHurtGfx, 
        .opponentHurt = gPinsirOpponentHurtGfx, 
        .objectPalette = gPinsirObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gPinsirPortraitGfx, 
        .portraitPalette = gPinsirPortraitPal, 
    },

    [SPECIES_KOFFING] = 
    { 
        .baseHP = 60, 
        .basePower = 60, 
        .baseDef = 60, 
        .move = DECK_SMOKESCREEN, 
        .ability = DECK_RESILIENT, 
    
        .playerIdle = gKoffingPlayerIdleGfx, 
        .opponentIdle = gKoffingOpponentIdleGfx, 
        .playerAttack = gKoffingPlayerAttackGfx, 
        .opponentAttack = gKoffingOpponentAttackGfx, 
        .playerHurt = gKoffingPlayerHurtGfx, 
        .opponentHurt = gKoffingOpponentHurtGfx, 
        .objectPalette = gKoffingObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gKoffingPortraitGfx, 
        .portraitPalette = gKoffingPortraitPal, 
    },

    [SPECIES_WEEZING] = 
    { 
        .baseHP = 80, 
        .basePower = 90, 
        .baseDef = 80, 
        .move = DECK_SMOKESCREEN, 
        .ability = DECK_RESILIENT, 
    
        .playerIdle = gWeezingPlayerIdleGfx, 
        .opponentIdle = gWeezingOpponentIdleGfx, 
        .playerAttack = gWeezingPlayerAttackGfx, 
        .opponentAttack = gWeezingOpponentAttackGfx, 
        .playerHurt = gWeezingPlayerHurtGfx, 
        .opponentHurt = gWeezingOpponentHurtGfx, 
        .objectPalette = gWeezingObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gWeezingPortraitGfx, 
        .portraitPalette = gWeezingPortraitPal, 
    },

    [SPECIES_GOLBAT] = 
    { 
        .baseHP = 60, 
        .basePower = 80, 
        .baseDef = 60, 
        .move = DECK_CRUNCH, 
        .ability = DECK_VAMPIRIC, 
    
        .playerIdle = gGolbatPlayerIdleGfx, 
        .opponentIdle = gGolbatOpponentIdleGfx, 
        .playerAttack = gGolbatPlayerAttackGfx, 
        .opponentAttack = gGolbatOpponentAttackGfx, 
        .playerHurt = gGolbatPlayerHurtGfx, 
        .opponentHurt = gGolbatOpponentHurtGfx, 
        .objectPalette = gGolbatObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gGolbatPortraitGfx, 
        .portraitPalette = gGolbatPortraitPal, 
    },

    [SPECIES_CROBAT] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 60, 
        .move = DECK_CRUNCH, 
        .ability = DECK_VAMPIRIC, 
    
        .playerIdle = gCrobatPlayerIdleGfx, 
        .opponentIdle = gCrobatOpponentIdleGfx, 
        .playerAttack = gCrobatPlayerAttackGfx, 
        .opponentAttack = gCrobatOpponentAttackGfx, 
        .playerHurt = gCrobatPlayerHurtGfx, 
        .opponentHurt = gCrobatOpponentHurtGfx, 
        .objectPalette = gCrobatObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gCrobatPortraitGfx, 
        .portraitPalette = gCrobatPortraitPal, 
    },

    [SPECIES_SLUGMA] = 
    { 
        .baseHP = 60, 
        .basePower = 70, 
        .baseDef = 50, 
        .move = DECK_LAVA_PLUME, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gSlugmaPlayerIdleGfx, 
        .opponentIdle = gSlugmaOpponentIdleGfx, 
        .playerAttack = gSlugmaPlayerAttackGfx, 
        .opponentAttack = gSlugmaOpponentAttackGfx, 
        .playerHurt = gSlugmaPlayerHurtGfx, 
        .opponentHurt = gSlugmaOpponentHurtGfx, 
        .objectPalette = gSlugmaObjectPal, 
        .playerYOffset = -2, 
        .opponentYOffset = 2, 
    
        .portrait = gSlugmaPortraitGfx, 
        .portraitPalette = gSlugmaPortraitPal, 
    },

    [SPECIES_MAGCARGO] = 
    { 
        .baseHP = 70, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = DECK_OVERHEAT, 
        .ability = DECK_PATIENT, 
    
        .playerIdle = gMagcargoPlayerIdleGfx, 
        .opponentIdle = gMagcargoOpponentIdleGfx, 
        .playerAttack = gMagcargoPlayerAttackGfx, 
        .opponentAttack = gMagcargoOpponentAttackGfx, 
        .playerHurt = gMagcargoPlayerHurtGfx, 
        .opponentHurt = gMagcargoOpponentHurtGfx, 
        .objectPalette = gMagcargoObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gMagcargoPortraitGfx, 
        .portraitPalette = gMagcargoPortraitPal, 
    },

    [SPECIES_HITMONTOP] = 
    { 
        .baseHP = 80, 
        .basePower = 80, 
        .baseDef = 80, 
        .move = DECK_RAPID_SPIN, 
        .ability = DECK_ENERGETIC, 
    
        .playerIdle = gHitmontopPlayerIdleGfx, 
        .opponentIdle = gHitmontopOpponentIdleGfx, 
        .playerAttack = gHitmontopPlayerAttackGfx, 
        .opponentAttack = gHitmontopOpponentAttackGfx, 
        .playerHurt = gHitmontopPlayerHurtGfx, 
        .opponentHurt = gHitmontopOpponentHurtGfx, 
        .objectPalette = gHitmontopObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gHitmontopPortraitGfx, 
        .portraitPalette = gHitmontopPortraitPal, 
    },

    [SPECIES_GRIMER] = 
    { 
        .baseHP = 60, 
        .basePower = 60, 
        .baseDef = 60, 
        .move = DECK_SLUDGE_BOMB, 
        .ability = DECK_TRICKY, 
    
        .playerIdle = gGrimerPlayerIdleGfx, 
        .opponentIdle = gGrimerOpponentIdleGfx, 
        .playerAttack = gGrimerPlayerAttackGfx, 
        .opponentAttack = gGrimerOpponentAttackGfx, 
        .playerHurt = gGrimerPlayerHurtGfx, 
        .opponentHurt = gGrimerOpponentHurtGfx, 
        .objectPalette = gGrimerObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gGrimerPortraitGfx, 
        .portraitPalette = gGrimerPortraitPal, 
    },

    [SPECIES_MUK] = 
    { 
        .baseHP = 70, 
        .basePower = 90, 
        .baseDef = 100, 
        .move = DECK_GUNK_SHOT, 
        .ability = DECK_TRICKY, 
    
        .playerIdle = gMukPlayerIdleGfx, 
        .opponentIdle = gMukOpponentIdleGfx, 
        .playerAttack = gMukPlayerAttackGfx, 
        .opponentAttack = gMukOpponentAttackGfx, 
        .playerHurt = gMukPlayerHurtGfx, 
        .opponentHurt = gMukOpponentHurtGfx, 
        .objectPalette = gMukObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gMukPortraitGfx, 
        .portraitPalette = gMukPortraitPal, 
    },

    [SPECIES_NINETALES] = 
    { 
        .baseHP = 80, 
        .basePower = 100, 
        .baseDef = 70, 
        .move = DECK_MORNING_SUN, 
        .ability = DECK_SOCIAL, 
    
        .playerIdle = gNinetalesPlayerIdleGfx, 
        .opponentIdle = gNinetalesOpponentIdleGfx, 
        .playerAttack = gNinetalesPlayerAttackGfx, 
        .opponentAttack = gNinetalesOpponentAttackGfx, 
        .playerHurt = gNinetalesPlayerHurtGfx, 
        .opponentHurt = gNinetalesOpponentHurtGfx, 
        .objectPalette = gNinetalesObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gNinetalesPortraitGfx, 
        .portraitPalette = gNinetalesPortraitPal, 
    },

    [SPECIES_ROSELIA] = 
    { 
        .baseHP = 60, 
        .basePower = 90, 
        .baseDef = 50, 
        .move = DECK_VINE_WHIP, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gRoseliaPlayerIdleGfx, 
        .opponentIdle = gRoseliaOpponentIdleGfx, 
        .playerAttack = gRoseliaPlayerAttackGfx, 
        .opponentAttack = gRoseliaOpponentAttackGfx, 
        .playerHurt = gRoseliaPlayerHurtGfx, 
        .opponentHurt = gRoseliaOpponentHurtGfx, 
        .objectPalette = gRoseliaObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gRoseliaPortraitGfx, 
        .portraitPalette = gRoseliaPortraitPal, 
    },

    [SPECIES_ROSERADE] = 
    { 
        .baseHP = 70, 
        .basePower = 110, 
        .baseDef = 60, 
        .move = DECK_MAGICAL_LEAF, 
        .ability = DECK_ADAPTIVE, 
    
        .playerIdle = gRoseradePlayerIdleGfx, 
        .opponentIdle = gRoseradeOpponentIdleGfx, 
        .playerAttack = gRoseradePlayerAttackGfx, 
        .opponentAttack = gRoseradeOpponentAttackGfx, 
        .playerHurt = gRoseradePlayerHurtGfx, 
        .opponentHurt = gRoseradeOpponentHurtGfx, 
        .objectPalette = gRoseradeObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gRoseradePortraitGfx, 
        .portraitPalette = gRoseradePortraitPal, 
    },

};
