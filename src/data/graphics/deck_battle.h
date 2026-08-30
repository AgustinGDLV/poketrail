static const u16 sDeckBattleInterfacePalette[] = INCBIN_U16("graphics/deck_battle_interface/tiles.gbapal");
static const u32 sDeckBattleInterfaceTiles[] = INCBIN_U32("graphics/deck_battle_interface/tiles.4bpp.lz");
static const u32 sDeckBattleInterfaceTilemap[] = INCBIN_U32("graphics/deck_battle_interface/map.bin.lz");

const struct OamData sOamData_8x8 =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 1,
};

const struct OamData sOamData_Battler =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

const struct OamData sOamData_Shadow =
{
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

const struct OamData sOamData_Portrait =
{
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 2,
};

const u8 sShadowGfx[] = INCBIN_U8("graphics/deck_battle_interface/shadow.4bpp");
const u8 sCursorGfx[] = INCBIN_U8("graphics/deck_battle_interface/cursor.4bpp");
const u8 sNumberGfx[] = INCBIN_U8("graphics/deck_battle_interface/number.4bpp");
const u16 sMiscGfxPal[] = INCBIN_U16("graphics/deck_battle_interface/number.gbapal");

const struct SpriteSheet gShadowSpriteSheet = { sShadowGfx, sizeof(sShadowGfx), TAG_SHADOW };
const struct SpritePalette gMiscGfxSpritePalette = { sMiscGfxPal, TAG_MISC_PAL };
const struct SpriteTemplate gShadowSpriteTemplate =
{
	.tileTag = TAG_SHADOW,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_Shadow,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Shadow,
};

const struct SpriteSheet gCursorSpriteSheet = { sCursorGfx, sizeof(sCursorGfx), TAG_CURSOR };
const struct SpriteTemplate gCursorSpriteTemplate =
{
	.tileTag = TAG_CURSOR,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_8x8,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Cursor,
};

static const struct SpriteSheet sNumberSpriteSheet = { sNumberGfx, sizeof(sNumberGfx), TAG_NUMBER };
static const struct SpriteTemplate sNumberSpriteTemplate =
{
	.tileTag = TAG_NUMBER,
	.paletteTag = TAG_MISC_PAL,
	.oam = &sOamData_8x8,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_DamageNumber,
};

static const union AnimCmd sAnim_Paused[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Idle[] =
{
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(16, 12),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_Attack[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(16, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Hurt[] =
{
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Battler[] =
{
    sAnim_Paused,
    sAnim_Idle,
    sAnim_Attack,
    sAnim_Hurt,
};
