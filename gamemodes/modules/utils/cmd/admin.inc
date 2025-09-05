CMD:givevehicle(playerid, params[]) {
    new targetid, modelid, color1, color2;
    if(pUser[playerid][pAdmin] < 4) {
        ShowNotification(playerid, "default", "Give Vehicle", "You are not authorized to use this command.", "error", 5000);
        return 1;
    }
    if(sscanf(params, "uiii", targetid, modelid, color1, color2)) {
        ShowNotification(playerid, "default", "Give Vehicle", "Usage: /givevehicle [playerid] [modelid] [color1] [color2]", "error", 5000);
        return 1;
    }
    if(targetid == INVALID_PLAYER_ID) {
        ShowNotification(playerid, "default", "Give Vehicle", "Invalid playerid.");
        return 1;
    }
    if(modelid < 400 || modelid > 611) {
        ShowNotification(playerid, "default", "Give Vehicle", "Invalid modelid.");
        return 1;
    }
    if(CreatePlayerVehicle(targetid, modelid, color1, color2, -1, false)) {
        ShowNotification(playerid, "default", "Give Vehicle", "You have given a vehicle to the player.", "success", 5000);
    } else {
        ShowNotification(playerid, "default", "Give Vehicle", "You have reached the maximum number of vehicles.", "error");
    }
    return 1;
}   

CMD:skinmenu(playerid, params[]) {
    new targetid;
    if(pUser[playerid][pAdmin] < 2) {
        ShowNotification(playerid, "default", "Give Skin", "You are not authorized to use this command.", "error", 5000);
        return 1;
    }
    if(sscanf(params, "u", targetid)) {
        ShowNotification(playerid, "default", "Give Skin", "Usage: /giveskin [playerid]", "error", 5000);
        return 1;
    }
    if(targetid == INVALID_PLAYER_ID) {
        ShowNotification(playerid, "default", "Give Skin", "Invalid playerid.", "error", 5000);
        return 1;
    }
    
    showSkinMenu(targetid);
    return 1;
}

CMD:sethp(playerid, params[]) {
    new targetid, amount;
    if(pUser[playerid][pAdmin] < 2) {
        ShowNotification(playerid, "default", "Set HP", "You are not authorized to use this command.", "error", 5000);
        return 1;
    }
    if(sscanf(params, "ui", targetid, amount)) {
        ShowNotification(playerid, "default", "Set HP", "Usage: /sethp [playerid] [amount]", "error", 5000);
        return 1;
    }
    if(targetid == INVALID_PLAYER_ID) {
        ShowNotification(playerid, "default", "Set HP", "Invalid playerid.", "error", 5000);
        return 1;
    }
    pUserD[targetid][pdHunger] = amount;
    pUserD[targetid][pdThirst] = amount;
    SetHealth(targetid, amount);
    return 1;
}

CMD:clearstrees(playerid, params[]) {
    new targetid;
    if(pUser[playerid][pAdmin] < 2) {
        ShowNotification(playerid, "default", "Clear Stress", "You are not authorized to use this command.", "error", 5000);
        return 1;
    }
    if(sscanf(params, "u", targetid)) {
        ShowNotification(playerid, "default", "Clear Stress", "Usage: /clearstrees [playerid]", "error", 5000);
        return 1;
    }
    if(targetid == INVALID_PLAYER_ID) {
        ShowNotification(playerid, "default", "Clear Stress", "Invalid playerid.", "error", 5000);
        return 1;
    }
    pUserD[targetid][pdStrees] = 0;
    if(pUserD[targetid][pDrunkEffect])
    {
        SetPlayerDrunkEffect(targetid, 0);
    }
    return 1;
}   

CMD:devtools(playerid, params[]) {
    cef_toggle_dev_tools(playerid, INTERFACE_BROWSER_ID, true);
    return 1;
}

CMD:giveitem(playerid, params[]) {
    new targetid, itemname[64], amount;
    if(pUser[playerid][pAdmin] < 2) {
        ShowNotification(playerid, "default", "Give Item", "You are not authorized to use this command.", "error", 5000);
        return 1;
    }
    if(sscanf(params, "us[64]i", targetid, itemname, amount)) {   
        ShowNotification(playerid, "default", "Give Item", "Usage: /giveitem [playerid] [itemname] [amount]", "error", 5000);
        return 1;
    }
    if(targetid == INVALID_PLAYER_ID) {
        ShowNotification(playerid, "default", "Give Item", "Invalid playerid.", "error", 5000);
        return 1;
    }

    forex(i, sizeof(Items))
    {
        printf("[DEBUG] Comparing %s with %s", itemname, Items[i][itemName]);
        if(!strcmp(itemname, Items[i][itemName]))
        {
            printf("[DEBUG] Item found: %s", Items[i][itemName]);
            AddItemToInventory(targetid, Items[i][itemID], amount);
            break;
        }
    }
    return 1;
}