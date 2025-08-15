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
        ShowNotification(playerid, "default", "Give Vehicle", "Failed to give a vehicle to the player.", "error", 5000);
    }
    return 1;
}   
