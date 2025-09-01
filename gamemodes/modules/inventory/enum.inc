#define MAX_INVENTORY_SLOTS 42

enum E_ITEM {
    itemID,
    itemName[64],
    Float:itemWeight
};

new const Items[][E_ITEM] = {
    {0,"burger", 1.0},
    {1,"water", 1.0}
};

enum E_PLAYER_INVENTORY {
    pInvItemIndex,
    pInvAmount
};
new PlayerInventory[MAX_PLAYERS][MAX_INVENTORY_SLOTS][E_PLAYER_INVENTORY];

new Iterator:PlayerInventory<MAX_PLAYERS, MAX_INVENTORY_SLOTS>;