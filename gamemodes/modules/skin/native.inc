stock showSkinMenu(playerid)
{   
    cef_emit_event(playerid, "ui:showSkinMenuList", CEFSTR(pUser[playerid][pIdentifier]));
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, true);
    TogglePlayerControllable(playerid, false);
    
    new Float:x, Float:y, Float:z, Float:angle;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, angle);

    new Float:camX = x + 5.0 * floatsin(-angle, degrees);
    new Float:camY = y + 5.0 * floatcos(-angle, degrees);
    new Float:camZ = z + 0.5;

    SetPlayerCameraPos(playerid, camX, camY, camZ);
    SetPlayerCameraLookAt(playerid, x, y, z);
    
    return 1;
}