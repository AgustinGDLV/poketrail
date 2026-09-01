const struct DeckSpeciesInfo gDeckSpeciesInfo[NUM_SPECIES] =
{
    [SPECIES_CATERPIE] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
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
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
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
        .baseHP = 100, 
        .basePower = 100,
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
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
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

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

    [SPECIES_EKANS] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
        .playerIdle = gEkansPlayerIdleGfx, 
        .opponentIdle = gEkansOpponentIdleGfx, 
        .playerAttack = gEkansPlayerAttackGfx, 
        .opponentAttack = gEkansOpponentAttackGfx, 
        .playerHurt = gEkansPlayerHurtGfx, 
        .opponentHurt = gEkansOpponentHurtGfx, 
        .objectPalette = gEkansObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gEkansPortraitGfx, 
        .portraitPalette = gEkansPortraitPal, 
    },

    [SPECIES_SPEAROW] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

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

    [SPECIES_VULPIX] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
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
        .move = MOVE_VINE_WHIP,
        .ability = ABILITY_NONE,

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

    [SPECIES_ZUBAT] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

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
        .move = MOVE_TACKLE,
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
        .move = MOVE_SURF,
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
        .move = MOVE_CRABHAMMER,
        .ability = ABILITY_NONE,

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
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
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

    [SPECIES_MAREEP] =
    {
        .baseHP = 55,
        .basePower = 55,
        .baseDef = 45,
        .move = MOVE_HELPING_HAND,
        .ability = ABILITY_NONE,

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

    [SPECIES_HOPPIP] = 
    { 
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
        .playerIdle = gHoppipPlayerIdleGfx, 
        .opponentIdle = gHoppipOpponentIdleGfx, 
        .playerAttack = gHoppipPlayerAttackGfx, 
        .opponentAttack = gHoppipOpponentAttackGfx, 
        .playerHurt = gHoppipPlayerHurtGfx, 
        .opponentHurt = gHoppipOpponentHurtGfx, 
        .objectPalette = gHoppipObjectPal, 
        .playerYOffset = 0, 
        .opponentYOffset = 0, 
    
        .portrait = gHoppipPortraitGfx, 
        .portraitPalette = gHoppipPortraitPal, 
    },

    [SPECIES_CLEFFA] =
    {
        .baseHP = 35,
        .basePower = 80,
        .baseDef = 25,
        .move = MOVE_HELPING_HAND,
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
        .move = MOVE_EARTHQUAKE,
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
        .move = MOVE_TACKLE,
        .ability = ABILITY_NONE,

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

    [SPECIES_SWABLU] =
    {
        .baseHP = 45,
        .basePower = 40,
        .baseDef = 70,
        .move = MOVE_HELPING_HAND,
        .ability = ABILITY_NONE,

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

    [SPECIES_BALTOY] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
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
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
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
        .move = MOVE_HELPING_HAND,
        .ability = ABILITY_NONE,

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

    [SPECIES_DEINO] =
    {
        .baseHP = 50,
        .basePower = 75,
        .baseDef = 35,
        .move = MOVE_TACKLE,
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
        .move = MOVE_TACKLE,
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
        .move = MOVE_FLORAL_HEALING,
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
        .move = MOVE_TACKLE,
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
        .move = MOVE_TACKLE,
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
        .baseHP = 100, 
        .basePower = 100, 
        .baseDef = 100, 
        .move = MOVE_TACKLE, 
        .ability = ABILITY_NONE, 
    
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
};
