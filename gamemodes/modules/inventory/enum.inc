#define MAX_INVENTORY_SLOTS 42
#define MAX_ITEM_DEFS (256)

enum E_ITEM {
    itemID,
    itemName[64],
    itemLabel[64],
    Float:itemWeight,
    useHunger,
    useThirst,
    useAnimDict[64],
    useAnimLib[64]
};

new Items[MAX_ITEM_DEFS][E_ITEM];

enum E_PLAYER_INVENTORY {
    pInvItemIndex,
    pInvAmount
};
new PlayerInventory[MAX_PLAYERS][MAX_INVENTORY_SLOTS][E_PLAYER_INVENTORY];

new Iterator:PlayerInventory<MAX_PLAYERS, MAX_INVENTORY_SLOTS>;