forward skinOnSelectSkin(playerid, const arguments[]);
public skinOnSelectSkin(playerid, const arguments[])
{
    new type[7], value;
    sscanf(arguments, "p<|>s[7]d", type, value);
    if(!strcmp(type, "skin"))
    {
        SetPlayerSkin(playerid, value);
    }
    else if(!strcmp(type, "hat"))
    {
        skinHatList(playerid, value);
    }
    return 1;
}

forward skinOnApplySkin(playerid, const arguments[]);
public skinOnApplySkin(playerid, const arguments[])
{

    // TODO: Implement money check
    //jika ada money 
    // if(jika player ada uang)
    // {
    //     
    // }

    new skin, hat, glasses, mask, backpack;
    sscanf(arguments, "p<|>ddddd", skin, hat, glasses, mask, backpack);

    printf("Player %d applied - Skin: %d, Hat: %d, Glasses: %d, Mask: %d, Backpack: %d", playerid, skin, hat, glasses, mask, backpack);
    
    if(skin != -1)
    {
        pUserD[playerid][pdSkin] = skin;
        skinSetSkin(playerid);
    }
    if(hat != -1)
    {
        pUserD[playerid][pdHat] = hat;
        skinSetHat(playerid);
    }
    if(glasses != -1)
    {
        pUserD[playerid][pdGlasses] = glasses;
        skinSetGlasses(playerid);
    }
    if(mask != -1)
    {
        pUserD[playerid][pdMask] = mask;
        skinSetMask(playerid);
    }
    if(backpack != -1)
    {
        pUserD[playerid][pdBackpack] = backpack;
        skinSetBackpack(playerid);
    }

    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    return 1;
}

forward skinOnCloseMenu(playerid);
public skinOnCloseMenu(playerid)
{
    skinResetAll(playerid);
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    return 1;
}