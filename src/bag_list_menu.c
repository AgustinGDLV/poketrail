#include "global.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "bg.h"
#include "bag_list_menu.h"
#include "debug.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "field_screen_effect.h"
#include "frontier_pass.h"
#include "frontier_util.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "link.h"
#include "line_break.h"
#include "load_save.h"
#include "main.h"
#include "map_gen.h"
#include "malloc.h"
#include "menu.h"
#include "new_game.h"
#include "option_menu.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "party_menu_custom.h"
#include "pokedex.h"
#include "pokenav.h"
#include "safari_zone.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "start_menu.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_card.h"
#include "window.h"
#include "union_room.h"
#include "dexnav.h"
#include "wild_encounter.h"
#include "constants/battle_frontier.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// constants
#define MAX_LIST_COUNT   10

// const data
static const struct ListMenuItem sBagListItems[MAX_LIST_COUNT] =
{
    {COMPOUND_STRING(""), 0},
    {COMPOUND_STRING(""), 1},
    {COMPOUND_STRING(""), 2},
    {COMPOUND_STRING(""), 3},
    {COMPOUND_STRING(""), 4},
    {COMPOUND_STRING(""), 5},
    {COMPOUND_STRING(""), 6},
    {COMPOUND_STRING(""), 7},
    {COMPOUND_STRING(""), 8},
    {COMPOUND_STRING(""), 9},
};

static const struct WindowTemplate sBagListMenuWinTemplates[] =
{
    // from Start Menu
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 14,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 8,
    },
    // from Trail Interface
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 3,
        .width = 14,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 1 + 14*2 + 14*2, // no overlap with trail interface
    },
};

struct BagListMenu
{
    u8 windowId;
    u8 listTaskId;
    struct ListMenuTemplate template;
    u16 items[MAX_LIST_COUNT]; // used for itemIds
    u8 itemsCount;
    u16 selectedItemId;
    u8 windowBg;
};

// vars
EWRAM_DATA static struct BagListMenu sBagListMenu = {0};

// forward declarations
static u32 GetItemIdQuantity(u32 itemId);
static void InitBagListMenuData(void);
static void InitItemListMenu(void);
static void InitPartyListMenu(void);
static void Task_ItemListMenu(u8 taskId);
static void Task_PartyListMenu(u8 taskId);
static void Task_CloseBagListMenu(u8 taskId);
static void BagListMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void BagListMenu_PartyPrintFunc(u8 windowId, u32 itemId, u8 y);
static bool32 UseItemOnPartyIndex(u32 itemId, u32 partyIndex);

// copied from item_menu.c
static u16 GetBagItemQuantity(u16 *quantity)
{
    return gSaveBlock2Ptr->encryptionKey ^ *quantity;
}

// copied from item_menu.c
static u32 GetItemIdQuantity(u32 itemId)
{
    u32 totalQuantity = 0;
    u32 pocket = ItemId_GetPocket(itemId) - 1;
    struct BagPocket *itemPocket = &gBagPockets[pocket];

    for (u32 i = 0; i < itemPocket->capacity; i++)
    {
        if (itemPocket->itemSlots[i].itemId == itemId)
            totalQuantity += GetBagItemQuantity(&itemPocket->itemSlots[i].quantity);
    }

    return totalQuantity;
}

// Init bag list menu task from another task.
void SwitchTaskToBagListMenu(u8 taskId, u32 windowBg)
{
    if (windowBg > 1)
        sBagListMenu.windowBg = 1;
    else
        sBagListMenu.windowBg = windowBg;
    InitBagListMenuData();
    SetTaskFuncWithFollowupFunc(taskId, Task_ItemListMenu, gTasks[taskId].func);
}

// Init common fields for bag list menu.
static void InitBagListMenuData(void)
{
    // Create window.
        sBagListMenu.windowId = AddWindow(&sBagListMenuWinTemplates[sBagListMenu.windowBg]);
    DrawStdWindowFrame(sBagListMenu.windowId, TRUE);

    // Create list menu template.
    sBagListMenu.template.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
    sBagListMenu.template.items = sBagListItems;
    sBagListMenu.template.totalItems = 0;
    sBagListMenu.template.maxShowed = 3;
    sBagListMenu.template.windowId = sBagListMenu.windowId;
    sBagListMenu.template.item_X = 8;
    sBagListMenu.template.upText_Y = 1;
    sBagListMenu.template.cursorPal = 1;
    sBagListMenu.template.fillValue = 15;
    sBagListMenu.template.cursorShadowPal = 15;
    sBagListMenu.template.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
    sBagListMenu.template.fontId = FONT_NORMAL;
    sBagListMenu.listTaskId = 0xFF;

    // Zero list menu items.
    for (u32 i = 0; i < MAX_LIST_COUNT; ++i)
    {
        sBagListMenu.items[i] = 0;
    }
}

// Destroy list menu task and clear data.
static void ResetBagListMenuData(void)
{
    if (sBagListMenu.listTaskId != 0xFF)
    {
        struct ListMenu *list = (void *) gTasks[sBagListMenu.listTaskId].data;
        DestroyListMenuTask(sBagListMenu.listTaskId, &list->scrollOffset, &list->selectedRow);
    }
    sBagListMenu.itemsCount = 0;
}

// Init data for item list.
static void InitItemListMenu(void)
{
    // Destroy previous list menu task.
    ResetBagListMenuData();

    // Update items.
    if (GetItemIdQuantity(ITEM_POTION) > 0)
    {
        sBagListMenu.items[sBagListMenu.itemsCount] = ITEM_POTION;
        ++sBagListMenu.itemsCount;
    }
    if (GetItemIdQuantity(ITEM_SUPER_POTION) > 0)
    {
        sBagListMenu.items[sBagListMenu.itemsCount] = ITEM_SUPER_POTION;
        ++sBagListMenu.itemsCount;
    }
    if (GetItemIdQuantity(ITEM_HYPER_POTION) > 0)
    {
        sBagListMenu.items[sBagListMenu.itemsCount] = ITEM_HYPER_POTION;
        ++sBagListMenu.itemsCount;
    }
    if (GetItemIdQuantity(ITEM_REVIVE) > 0)
    {
        sBagListMenu.items[sBagListMenu.itemsCount] = ITEM_REVIVE;
        ++sBagListMenu.itemsCount;
    }

    // Init new list menu task.
    sBagListMenu.template.totalItems = sBagListMenu.itemsCount;
    sBagListMenu.template.itemPrintFunc = BagListMenu_ItemPrintFunc;
    sBagListMenu.listTaskId = ListMenuInit(&sBagListMenu.template, 0, 0);
    CopyWindowToVram(sBagListMenu.windowId, COPYWIN_FULL);
    if (sBagListMenu.windowBg)
        CopyBgTilemapBufferToVram(0);
    else
        CopyBgTilemapBufferToVram(1);
}

// Init data for party list.
static void InitPartyListMenu(void)
{
    // Destroy previous list menu task.
    ResetBagListMenuData();

    // Update items.
   sBagListMenu.itemsCount = CalculatePlayerPartyCount();

    // Init new list menu task.
    sBagListMenu.template.totalItems = sBagListMenu.itemsCount;
    sBagListMenu.template.itemPrintFunc = BagListMenu_PartyPrintFunc;
    sBagListMenu.listTaskId = ListMenuInit(&sBagListMenu.template, 0, 0);
    CopyWindowToVram(sBagListMenu.windowId, COPYWIN_FULL);
    CopyBgTilemapBufferToVram(sBagListMenu.windowBg);
}

#define tState data[0]
#define tTimer data[1]

static void Task_ItemListMenu(u8 taskId)
{
    u32 input;
    switch (gTasks[taskId].tState)
    {
    case 0: // Init list menu.
        InitItemListMenu();
        if (sBagListMenu.itemsCount == 0)
            gTasks[taskId].func = Task_CloseBagListMenu;
        else
            ++gTasks[taskId].tState;
        break;
    case 1: // Process input.
        input = ListMenu_ProcessInput(sBagListMenu.listTaskId);
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tState = 0;
            gTasks[taskId].func = Task_PartyListMenu;
            sBagListMenu.selectedItemId = sBagListMenu.items[input];
        }
        if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tState = 0;
            gTasks[taskId].func = Task_CloseBagListMenu;
        }
        break;
    }
}

static void Task_PartyListMenu(u8 taskId)
{
    u32 input;
    switch (gTasks[taskId].tState)
    {
    case 0: // Init list menu.
        InitPartyListMenu();
        ++gTasks[taskId].tState;
        break;
    case 1: // Process input.
        input = ListMenu_ProcessInput(sBagListMenu.listTaskId);
        if (JOY_NEW(A_BUTTON))
        {
            if (UseItemOnPartyIndex(sBagListMenu.selectedItemId, input))
            {
                PlaySE(SE_USE_ITEM);
                ++gTasks[taskId].tState;
            }
            else
            {
                PlaySE(SE_FAILURE);
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gTasks[taskId].tState = 0;
            gTasks[taskId].func = Task_ItemListMenu;
        }
        break;
    case 2: // Update party list.
        RemoveBagItem(sBagListMenu.selectedItemId, 1);
        if (GetItemIdQuantity(sBagListMenu.selectedItemId) == 0)
        {
            gTasks[taskId].tState = 0;
            gTasks[taskId].func = Task_ItemListMenu;
        }
        else
        {
            RedrawListMenu(sBagListMenu.listTaskId);
            gTasks[taskId].tState = 1;
        }
        break;
    }
}

static void Task_CloseBagListMenu(u8 taskId)
{
    ClearStdWindowAndFrame(sBagListMenu.windowId, TRUE);
    RemoveWindow(sBagListMenu.windowId);
    ResetBagListMenuData();
    SwitchTaskToFollowupFunc(taskId);
}

#undef tState
#undef tTimer

static void BagListMenu_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, 1, 8};
    AddTextPrinterParameterized4(windowId, FONT_NORMAL, 8, y, 0, 0, textColor, TEXT_SKIP_DRAW, ItemId_GetName(sBagListMenu.items[itemId]));
    ConvertIntToDecimalStringN(gStringVar1, GetItemIdQuantity(sBagListMenu.items[itemId]), STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized4(windowId, FONT_NORMAL, 8 + GetStringRightAlignXOffset(FONT_NORMAL, gStringVar1, 96), y, 0, 0, textColor, TEXT_SKIP_DRAW, gStringVar1);
}

static void BagListMenu_PartyPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    struct Pokemon *mon = &gPlayerParty[itemId];
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 hp = GetMonData(mon, MON_DATA_HP);
    u32 maxHP = GetMonData(mon, MON_DATA_MAX_HP);
    const u8 textColor[] = {TEXT_COLOR_TRANSPARENT, 1, 8};

    AddTextPrinterParameterized4(windowId, FONT_NORMAL, 8, y, 0, 0, textColor, TEXT_SKIP_DRAW, GetSpeciesName(species));
    ConvertIntToDecimalStringN(gStringVar2, hp, STR_CONV_MODE_LEFT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, maxHP, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_2}/{STR_VAR_3}"));
    AddTextPrinterParameterized4(windowId, FONT_NORMAL, 8 + GetStringRightAlignXOffset(FONT_NORMAL, gStringVar1, 96), y, 0, 0, textColor, TEXT_SKIP_DRAW, gStringVar1);
}

static bool32 UseItemOnPartyIndex(u32 itemId, u32 partyIndex)
{
    struct Pokemon *mon = &gPlayerParty[partyIndex];
    u32 hp = GetMonData(mon, MON_DATA_HP);
    u32 maxHP = GetMonData(mon, MON_DATA_MAX_HP);

    // Check for invalid item use; TODO: assumes only healing items
    if (itemId == ITEM_REVIVE && hp != 0)
        return FALSE;
    else if (hp == 0 || hp == maxHP)
        return FALSE;

    // Heal party member; TODO: use item params.
    switch (itemId)
    {
    case ITEM_POTION:
        hp += 20;
        break;
    case ITEM_SUPER_POTION:
        hp += 60;
        break;
    case ITEM_HYPER_POTION:
        hp += 200;
        break;
    case ITEM_REVIVE:
        hp += (maxHP * 50) / 100;
        break;
    }

    if (hp > maxHP)
        hp = maxHP;
    SetMonData(mon, MON_DATA_HP, &hp);

    return TRUE;
}
