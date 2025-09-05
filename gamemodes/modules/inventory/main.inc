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

forward invOnSwapItems(playerid, const arguments[]);
public invOnSwapItems(playerid, const arguments[])
{
    new oldslot, newslot;
    sscanf(arguments, "p<|>dd", oldslot, newslot);

    if(oldslot == newslot) return 0;
    new oldItemIndex = PlayerInventory[playerid][oldslot][pInvItemIndex];
    new oldItemAmount = PlayerInventory[playerid][oldslot][pInvAmount];
    PlayerInventory[playerid][oldslot][pInvItemIndex] = -1;
    PlayerInventory[playerid][oldslot][pInvAmount] = -1;
    PlayerInventory[playerid][newslot][pInvItemIndex] = oldItemIndex;
    PlayerInventory[playerid][newslot][pInvAmount] = oldItemAmount;

    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "UPDATE `krakterinventory` SET `slot` = %d WHERE `identifier` = '%s' AND `item` = %d", newslot, pUser[playerid][pIdentifier], oldItemIndex);
    mysql_tquery(g_SQL, query, "OnInventoryItemSwapped", "d", playerid);
    return 1;
}

forward OnInventoryItemSwapped(playerid);
public OnInventoryItemSwapped(playerid)
{
    cef_emit_event(playerid, "inventory:updateInventory", CEFSTR(pUser[playerid][pIdentifier]));
    return 1;
}

forward invOnUseItem(playerid, const arguments[]);
public invOnUseItem(playerid, const arguments[])
{
    new itemslot;
    sscanf(arguments, "p<|>d", itemslot);

    new itemIndex = PlayerInventory[playerid][itemslot][pInvItemIndex];

    if(itemIndex == -1) return 0;
    
    if(itemIndex == 0)
    {
        ShowNotification(playerid, "default", "Inventory", "Use Burgger", "inform", 5000);
    }
    else if(itemIndex == 1)
    {
        ShowNotification(playerid, "default", "Inventory", "Use Water", "inform", 5000);
    }
    return 1;
}

forward invOnDropItem(playerid, const arguments[]);
public invOnDropItem(playerid, const arguments[])
{
    printf("invOnDropItem: %s", arguments);
    new slot, amount;
    sscanf(arguments, "p<|>dd", slot, amount);
    new itemIndex = PlayerInventory[playerid][slot][pInvItemIndex];
    if(itemIndex == -1) return 0;
    PlayerInventory[playerid][slot][pInvAmount] -= amount;
    new query[256];
    if(PlayerInventory[playerid][slot][pInvAmount] <= 0)
    {
        PlayerInventory[playerid][slot][pInvItemIndex] = -1;
        PlayerInventory[playerid][slot][pInvAmount] = -1;
        Iter_Remove(PlayerInventory<playerid>, slot);
        mysql_format(g_SQL, query, sizeof(query), "DELETE FROM `krakterinventory` WHERE `identifier` = '%s' AND `item` = %d", pUser[playerid][pIdentifier], itemIndex);
        mysql_tquery(g_SQL, query, "OnInventoryItemDropped", "idd", playerid, itemIndex, amount);
    }
    else
    {
        mysql_format(g_SQL, query, sizeof(query), "UPDATE `krakterinventory` SET `amount` = %d WHERE `identifier` = '%s' AND `item` = %d", PlayerInventory[playerid][slot][pInvAmount], pUser[playerid][pIdentifier], itemIndex);
        mysql_tquery(g_SQL, query, "OnInventoryItemDropped", "idd", playerid, itemIndex, amount);
    }
    return 1;
}

forward OnInventoryItemDropped(playerid, itemIndex, amount);
public OnInventoryItemDropped(playerid, itemIndex, amount)
{
    new query[256];
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `inventorydrop` WHERE `item` = %d", itemIndex);
    mysql_tquery(g_SQL, query, "CheckNearbyItems", "iddfffdd", playerid, itemIndex, amount, x, y, z, 3.0, false);
    return 1;
}
forward CheckNearbyItems(playerid, itemIndex, amount, Float:x, Float:y, Float:z, Float:range, bool:found);
public CheckNearbyItems(playerid, itemIndex, amount, Float:x, Float:y, Float:z, Float:range, bool:found)
{
    new rows = cache_num_rows();
    new query[512];
    
    if(rows > 0)
    {
        new id, Float:dx, Float:dy, Float:dz, dbAmount;
        forex(i, rows)
        {
            cache_get_value_name_int(i, "id", id);
            cache_get_value_name_float(i, "posx", dx);
            cache_get_value_name_float(i, "posy", dy);
            cache_get_value_name_float(i, "posz", dz);
            cache_get_value_name_int(i, "amount", dbAmount);
            
            if(Is3DPointInRangeOfPoint(x, y, z, dx, dy, dz, range))
            {
                mysql_format(g_SQL, query, sizeof(query), "UPDATE `inventorydrop` SET `amount` = `amount` + %d WHERE `id` = %d", amount, id);
                mysql_tquery(g_SQL, query, "OnWorldItemCreated", "d", playerid);
                return 1;
            }
        }
    }

    new Float:randomey =   100.0 / RandomMinMax(50, 100);
    
    new objctid = CreateDynamicObject(2969, x, y + randomey, z - 0.9, 0.0, 0.0, 0.0, -1, -1, -1, 100.0, 100.0, -1, 0);
    mysql_format(g_SQL, query, sizeof(query), "INSERT INTO `inventorydrop` (`identifier`, `item`, `amount`, `posx`, `posy`, `posz`, `objctid`) VALUES ('%s', %d, %d, %f, %f, %f, %d)", 
        pUser[playerid][pIdentifier], itemIndex, amount, x, y, z, objctid);
    mysql_tquery(g_SQL, query, "OnWorldItemCreated", "d", playerid);
    return 1;
}

forward OnWorldItemCreated(playerid);
public OnWorldItemCreated(playerid)
{
    cef_emit_event(playerid, "inventory:updateInventory", CEFSTR(pUser[playerid][pIdentifier]));
    showRightInventory(playerid, "drop");
    return 1;
}


forward invOnTakeItem(playerid, const arguments[]);
public invOnTakeItem(playerid, const arguments[])
{
    new itemid, amount;
    sscanf(arguments, "p<|>dd", itemid, amount);
    printf("invOnTakeItem: %d, %d", itemid, amount);
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `inventorydrop` WHERE `item` = %d", itemid);
    mysql_pquery(g_SQL, query, "OnTakeItemFromWorld", "idd", playerid, itemid, amount);
    return 1;      
}

forward OnTakeItemFromWorld(playerid, itemid, amount);
public OnTakeItemFromWorld(playerid, itemid, amount)
{
    new rows = cache_num_rows();
    new query[512];
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    const Float:range = 3.0;
    
    if(rows > 0)
    {
        new id, Float:dx, Float:dy, Float:dz, dbAmount, dbObjctid;
        forex(i, rows)
        {
            cache_get_value_name_int(i, "id", id);
            cache_get_value_name_float(i, "posx", dx);
            cache_get_value_name_float(i, "posy", dy);
            cache_get_value_name_float(i, "posz", dz);
            cache_get_value_name_int(i, "amount", dbAmount);
            cache_get_value_name_int(i, "objctid", dbObjctid);

            if(Is3DPointInRangeOfPoint(x, y, z, dx, dy, dz, range))
            {
                printf("OnTakeItemFromWorld: %d, %d", itemid, amount);
                new totalammount = dbAmount - amount;
                if((dbAmount -= amount) <= 0)
                {
                    DestroyDynamicObject(dbObjctid);
                    mysql_format(g_SQL, query, sizeof(query), "DELETE FROM `inventorydrop` WHERE `id` = %d", id);
                    mysql_tquery(g_SQL, query, "OnWorldItemCreatedTaken", "iii", playerid, itemid, amount);
                    return 1;
                }
                mysql_format(g_SQL, query, sizeof(query), "UPDATE `inventorydrop` SET `amount` = %d WHERE `id` = %d", totalammount, id);
                mysql_tquery(g_SQL, query, "OnWorldItemCreatedTaken", "iii", playerid, itemid, amount);
                return 1;
            }
        }
    }
    return 1;
}

forward OnWorldItemCreatedTaken(playerid, itemid, amount);
public OnWorldItemCreatedTaken(playerid, itemid, amount)
{
    printf("OnWorldItemCreatedTaken: %d, %d", itemid, amount);
    AddItemToInventory(playerid, itemid, amount);
    cef_emit_event(playerid, "inventory:updateInventory", CEFSTR(pUser[playerid][pIdentifier]));
    showRightInventory(playerid, "drop");
    return 1;
}
