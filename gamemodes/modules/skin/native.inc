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

Dialog:EditPropMenu(playerid, response, listitem, inputtext[])
{
    if(!response) return 1;

    switch (listitem)
    {
        case 0:
        {
            if(IsPlayerAttachedObjectSlotUsed(playerid, 1))
            {
                pSkin[playerid][pSkinEditType] = 1;
                EditAttachedObject(playerid, 1);
            }
        }
        case 1:
        {
            if(IsPlayerAttachedObjectSlotUsed(playerid, 2))
            {
                pSkin[playerid][pSkinEditType] = 2;
                EditAttachedObject(playerid, 2);
            }
        }
        case 2:
        {
            if(IsPlayerAttachedObjectSlotUsed(playerid, 3))
            {
                pSkin[playerid][pSkinEditType] = 3;
                EditAttachedObject(playerid, 3);
            }
        }
        case 3:
        {
            if(IsPlayerAttachedObjectSlotUsed(playerid, 4))
            {
                pSkin[playerid][pSkinEditType] = 4;
                EditAttachedObject(playerid, 4);
            }
        }
        default:
        {
            return 0;
        }
    }
    return 1;
}