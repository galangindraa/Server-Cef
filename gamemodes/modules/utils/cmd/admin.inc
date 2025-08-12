CMD:givevehicle(playerid, params[]) {
    new targetid, modelid, color1, color2;
    if(pUser[playerid][pAdmin] < 4) {
        SendClientMessage(playerid, -1, "You are not authorized to use this command.");
        return 1;
    }
    if(sscanf(params, "uiii", targetid, modelid, color1, color2)) {
        SendClientMessage(playerid, -1, "Usage: /givevehicle [playerid] [modelid] [color1] [color2]");
        return 1;
    }
    if(targetid == INVALID_PLAYER_ID) {
        SendClientMessage(playerid, -1, "Invalid playerid.");
        return 1;
    }
    if(modelid < 400 || modelid > 611) {
        SendClientMessage(playerid, -1, "Invalid modelid.");
        return 1;
    }
    if(CreatePlayerVehicle(targetid, modelid, color1, color2, -1, false)) {
        SendClientMessage(playerid, -1, "You have given a vehicle to the player.");
    } else {
        SendClientMessage(playerid, -1, "Failed to give a vehicle to the player.");
    }
    return 1;
}   
