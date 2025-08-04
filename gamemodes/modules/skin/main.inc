forward skinOnCloseMenu(playerid, const callback[]);
public skinOnCloseMenu(playerid, const callback[])
{
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);

    RemovePlayerAttachedObject(playerid, 1); // Remove hat
    RemovePlayerAttachedObject(playerid, 2); // Remove glasses
    RemovePlayerAttachedObject(playerid, 3); // Remove mask
    RemovePlayerAttachedObject(playerid, 4); // Remove backpack
    
    SetPlayerSkin(playerid, pSkin[playerid][pSkinID]);
    if(pSkin[playerid][pSkinHat] != -1)
    {
        SetPlayerAttachedObject(playerid, 1, pSkin[playerid][pSkinHat], 2, pSkin[playerid][pSkinHatPos][0], pSkin[playerid][pSkinHatPos][1], pSkin[playerid][pSkinHatPos][2], pSkin[playerid][pSkinHatPos][3], pSkin[playerid][pSkinHatPos][4], pSkin[playerid][pSkinHatPos][5], pSkin[playerid][pSkinHatPos][6], pSkin[playerid][pSkinHatPos][7], pSkin[playerid][pSkinHatPos][8]);
    }
    if(pSkin[playerid][pSkinGlasses] != -1)
    {
        SetPlayerAttachedObject(playerid, 2, pSkin[playerid][pSkinGlasses], 2, pSkin[playerid][pSkinGlassesPos][0], pSkin[playerid][pSkinGlassesPos][1], pSkin[playerid][pSkinGlassesPos][2], pSkin[playerid][pSkinGlassesPos][3], pSkin[playerid][pSkinGlassesPos][4], pSkin[playerid][pSkinGlassesPos][5], pSkin[playerid][pSkinGlassesPos][6], pSkin[playerid][pSkinGlassesPos][7], pSkin[playerid][pSkinGlassesPos][8]);
    }
    if(pSkin[playerid][pSkinMask] != -1)
    {
        SetPlayerAttachedObject(playerid, 3, pSkin[playerid][pSkinMask], 2, pSkin[playerid][pSkinMaskPos][0], pSkin[playerid][pSkinMaskPos][1], pSkin[playerid][pSkinMaskPos][2], pSkin[playerid][pSkinMaskPos][3], pSkin[playerid][pSkinMaskPos][4], pSkin[playerid][pSkinMaskPos][5], pSkin[playerid][pSkinMaskPos][6], pSkin[playerid][pSkinMaskPos][7], pSkin[playerid][pSkinMaskPos][8]);
    }

    if(pSkin[playerid][pSkinBackpack] != -1)
    {
        SetPlayerAttachedObject(playerid, 4, pSkin[playerid][pSkinBackpack], 1, pSkin[playerid][pSkinBackpackPos][0], pSkin[playerid][pSkinBackpackPos][1], pSkin[playerid][pSkinBackpackPos][2], pSkin[playerid][pSkinBackpackPos][3], pSkin[playerid][pSkinBackpackPos][4], pSkin[playerid][pSkinBackpackPos][5], pSkin[playerid][pSkinBackpackPos][6], pSkin[playerid][pSkinBackpackPos][7], pSkin[playerid][pSkinBackpackPos][8]);
    }
    return 1;
}

forward skinOnBuyOutfitList(playerid, const callback[]);
public skinOnBuyOutfitList(playerid, const callback[])
{
    cef_emit_event(playerid, "ui:showSkinMenu", CEFSTR(pUserD[playerid][pdGender]));
    return 1;
}

forward skinOnSelectSkin(playerid, const callback[]);
public skinOnSelectSkin(playerid, const callback[])
{
    new type[17], value;
    sscanf(callback, "p<|>s[17]d", type, value);
    if(!strcmp(type, "skin"))
    {
        SetPlayerSkin(playerid, value);
        return 1;
    }
    else if(!strcmp(type, "hat"))
    {
        RemovePlayerAttachedObject(playerid, 1); // Remove previous hat if any
        SetPlayerAttachedObject(playerid, 1, value, 2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        return 1;
    }
    else if(!strcmp(type, "glasses"))
    {
        RemovePlayerAttachedObject(playerid, 2); // Remove previous glasses if any
        SetPlayerAttachedObject(playerid, 2, value, 2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        return 1;
    }
    else if(!strcmp(type, "mask"))
    {
        RemovePlayerAttachedObject(playerid, 3); // Remove previous mask if any
        SetPlayerAttachedObject(playerid, 3, value, 2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        return 1;
    }
    else if(!strcmp(type, "backpack"))
    {
        RemovePlayerAttachedObject(playerid, 4); // Remove previous backpack if any
        SetPlayerAttachedObject(playerid, 4, value, 1, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        return 1;
    }
    return 1;
}

forward skinOnApplySkin(playerid, const callback[]);
public skinOnApplySkin(playerid, const callback[])
{
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    new skin, hat, glasses, mask, backpack;
    sscanf(callback, "p<|>ddddd", skin, hat, glasses, mask, backpack);
    printf("skin: %d, hat: %d, glasses: %d, mask: %d, backpack: %d\n", skin, hat, glasses, mask, backpack);
    if(skin != -1)
    {
        pSkin[playerid][pSkinID] = skin; // Save the skin to the player's data
        SetPlayerSkin(playerid, skin);
    }
    if(hat != -1) // Check if hat is not -1 (meaning no hat)
    {
        RemovePlayerAttachedObject(playerid, 1); // Remove previous hat if any
        pSkin[playerid][pSkinHat] = hat; // Save the hat to the player's data
        pSkin[playerid][pSkinHatPos][0] = 0.6; // Default position for the hat
        pSkin[playerid][pSkinHatPos][1] = 0.0;
        pSkin[playerid][pSkinHatPos][2] = 0.0;
        pSkin[playerid][pSkinHatPos][3] = 0.0;
        pSkin[playerid][pSkinHatPos][4] = 0.0;
        pSkin[playerid][pSkinHatPos][5] = 0.0;
        pSkin[playerid][pSkinHatPos][6] = 1.0; // Default scale for the hat
        pSkin[playerid][pSkinHatPos][7] = 1.0;
        pSkin[playerid][pSkinHatPos][8] = 1.0;
        SetPlayerAttachedObject(playerid, 1, hat, 2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    }
    if(glasses != -1) // Check if glasses is not -1 (meaning no glasses)
    {
        RemovePlayerAttachedObject(playerid, 2); // Remove previous glasses if any
        pSkin[playerid][pSkinGlasses] = glasses; // Save the glasses to the player's data
        pSkin[playerid][pSkinGlassesPos][0] = 0.6; // Default position for the glasses
        pSkin[playerid][pSkinGlassesPos][1] = 0.0;
        pSkin[playerid][pSkinGlassesPos][2] = 0.0;
        pSkin[playerid][pSkinGlassesPos][3] = 0.0;
        pSkin[playerid][pSkinGlassesPos][4] = 0.0;
        pSkin[playerid][pSkinGlassesPos][5] = 0.0;
        pSkin[playerid][pSkinGlassesPos][6] = 1.0; // Default scale for the glasses
        pSkin[playerid][pSkinGlassesPos][7] = 1.0;
        pSkin[playerid][pSkinGlassesPos][8] = 1.0;
        SetPlayerAttachedObject(playerid, 2, glasses, 2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    }
    if(mask != -1) // Check if mask is not -1 (meaning no mask)
    {
        RemovePlayerAttachedObject(playerid, 3); // Remove previous mask if any
        pSkin[playerid][pSkinMask] = mask; // Save the mask to the player's data
        pSkin[playerid][pSkinMaskPos][0] = 0.6; // Default position for the mask
        pSkin[playerid][pSkinMaskPos][1] = 0.0;
        pSkin[playerid][pSkinMaskPos][2] = 0.0;
        pSkin[playerid][pSkinMaskPos][3] = 0.0;
        pSkin[playerid][pSkinMaskPos][4] = 0.0;
        pSkin[playerid][pSkinMaskPos][5] = 0.0;
        pSkin[playerid][pSkinMaskPos][6] = 1.0; // Default scale for the mask
        pSkin[playerid][pSkinMaskPos][7] = 1.0;
        pSkin[playerid][pSkinMaskPos][8] = 1.0;
        SetPlayerAttachedObject(playerid, 3, mask, 2, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    }
    if(backpack != -1)
    {
        RemovePlayerAttachedObject(playerid, 4); // Remove previous backpack if any
        pSkin[playerid][pSkinBackpack] = backpack; // Save the backpack to the player's data
        pSkin[playerid][pSkinBackpackPos][0] = 0.6; // Default position for the backpack
        pSkin[playerid][pSkinBackpackPos][1] = 0.0;
        pSkin[playerid][pSkinBackpackPos][2] = 0.0;
        pSkin[playerid][pSkinBackpackPos][3] = 0.0;
        pSkin[playerid][pSkinBackpackPos][4] = 0.0;
        pSkin[playerid][pSkinBackpackPos][5] = 0.0;
        pSkin[playerid][pSkinBackpackPos][6] = 1.0; // Default scale for the backpack
        pSkin[playerid][pSkinBackpackPos][7] = 1.0;
        pSkin[playerid][pSkinBackpackPos][8] = 1.0;
        // Attach the backpack to the player
        RemovePlayerAttachedObject(playerid, 4); // Remove previous backpack if any
        SetPlayerAttachedObject(playerid, 4, backpack, 1, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    }
    return 1;
}