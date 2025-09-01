LoadInventory(playerid) {
    new query[250];
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `krakterinventory` WHERE `identifier` = '%s'", pUser[playerid][pIdentifier]);
    mysql_pquery(g_SQL, query, "OnInventoryLoad", "i", playerid);
}

forward OnInventoryLoad(playerid);
public OnInventoryLoad(playerid) {
    new rows = cache_num_rows();
    if(rows)
    {
        Iter_Clear(PlayerInventory<playerid>);
        new slot, item, amount;
        forex(i, rows)
        {
            cache_get_value_name_int(i, "item", item);
            cache_get_value_name_int(i, "slot", slot);
            cache_get_value_name_int(i, "amount", amount);

            PlayerInventory[playerid][slot][pInvItemIndex] = item;
            PlayerInventory[playerid][slot][pInvAmount] = amount;

            Iter_Add(PlayerInventory<playerid>, slot);
        }
    }
}

