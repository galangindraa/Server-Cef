/**
 * Validates if a player is connected and has a valid name
 * @param playerid - The player ID to validate
 * @return 1 if valid, 0 if invalid
 */
stock authValidatePlayer(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    
    GetPlayerName(playerid, pUser[playerid][pName]);
    if(strlen(pUser[playerid][pName]) < 1) return 0;
    
    return 1;
}

/**
 * Sends a formatted message to player and kicks them
 * @param playerid - The player ID
 * @param message - The message to send
 * @return 1 on success
 */
stock authKickPlayerWithMessage(playerid, const message[])
{
    SendClientMessage(playerid, -1, message);
    Kick(playerid);
    return 1;
}