stock AddItemToInventory(playerid, itemid, amount) {
    foreach(new i : PlayerInventory<playerid>)
    {
        if(PlayerInventory[playerid][i][pInvItemIndex] == itemid)
        {
            new query[250];
            mysql_format(g_SQL, query, sizeof(query), "UPDATE `krakterinventory` SET `amount` = %d WHERE `identifier` = '%s' AND `slot` = %d", PlayerInventory[playerid][i][pInvAmount], pUser[playerid][pIdentifier], i);
            mysql_tquery(g_SQL, query, "OnInventoryAdd", "idddd", playerid, i, itemid, amount, 0);
            return 1;
        }
    }
    new slot = Iter_Free(PlayerInventory<playerid>);
    if(slot == cellmin)
    {
        ShowNotification(playerid, "default", "Inventory", "Inventory Penuh", "error", 5000);
        return 0;
    }
    new query[250];
    mysql_format(g_SQL, query, sizeof(query), "INSERT INTO `krakterinventory` (`identifier`, `item`, `slot`, `amount`) VALUES ('%s', %d, %d, %d)", pUser[playerid][pIdentifier], itemid, slot, amount);
    mysql_tquery(g_SQL, query, "OnInventoryAdd", "idddd", playerid, slot, itemid, amount, 1);
    return 1;
}

forward OnInventoryAdd(playerid, slot, itemid, amount, type);
public OnInventoryAdd(playerid, slot, itemid, amount, type) {
    PlayerInventory[playerid][slot][pInvItemIndex] = itemid;
    PlayerInventory[playerid][slot][pInvAmount] = amount;
    if(type == 1)
    {
        Iter_Add(PlayerInventory<playerid>, slot);
    }

    ShowNotification(playerid, "default", "Inventory", "Berhasil menambahkan item ke inventory", "success", 5000);
}

