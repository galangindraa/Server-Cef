stock skinSetSkin(playerid)
{
    SetPlayerSkin(playerid, pUserD[playerid][pdSkin]);
    return 1;
}

stock skinSetHat(playerid)
{
    skinHatList(playerid, pUserD[playerid][pdHat]);
    return 1;
}

stock skinSetGlasses(playerid)
{
    
    return 1;
}

stock skinSetMask(playerid)
{

    return 1;
}

stock skinSetBackpack(playerid)
{
    return 1;
}

stock skinResetAll(playerid)
{
    skinSetSkin(playerid);
    skinSetHat(playerid);
    skinSetGlasses(playerid);
    skinSetMask(playerid);
    skinSetBackpack(playerid);
    return 1;
}