stock AddItemToInventory(playerid, itemid, amount) {
    foreach(new i : PlayerInventory<playerid>)
    {
        if(PlayerInventory[playerid][i][pInvItemIndex] == itemid)
        {
            PlayerInventory[playerid][i][pInvAmount] += amount;
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
    if(type == 1)
    {
        Iter_Add(PlayerInventory<playerid>, slot);
    }

    ShowNotification(playerid, "default", "Inventory", "Berhasil menambahkan item ke inventory", "success", 5000);
}

stock showRightInventory(playerid, const type[])
{
    new json[256], Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    format(json, sizeof(json), "{\"x\": %f, \"y\": %f, \"z\": %f, \"identifier\": \"%s\", \"type\": \"%s\"}",
        x, y, z, pUser[playerid][pIdentifier], type);
    cef_emit_event(playerid, "openInventoryRight", CEFSTR(json));
}