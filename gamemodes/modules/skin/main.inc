fnc:skinOnCloseMenu(playerid, const callback[])
{
    forex(i, 4)
    {
        if(IsPlayerAttachedObjectSlotUsed(playerid, i)) 
        {
            RemovePlayerAttachedObject(playerid, i);
        }
    }

    SetPlayerSkin(playerid, pSkin[playerid][pSkinID]);
    if(pSkin[playerid][pSkinHat] != -1) {
        SetPlayerAttachedObject(playerid, 1, pSkin[playerid][pSkinHat], 2, pSkin[playerid][pSkinHatPos][0], pSkin[playerid][pSkinHatPos][1], pSkin[playerid][pSkinHatPos][2], 
                                pSkin[playerid][pSkinHatPos][3], pSkin[playerid][pSkinHatPos][4], pSkin[playerid][pSkinHatPos][5], 
                                pSkin[playerid][pSkinHatPos][6], pSkin[playerid][pSkinHatPos][7], pSkin[playerid][pSkinHatPos][8]);
    }

    if(pSkin[playerid][pSkinGlasses] != -1) {
        SetPlayerAttachedObject(playerid, 2, pSkin[playerid][pSkinGlasses], 2, pSkin[playerid][pSkinGlassesPos][0], pSkin[playerid][pSkinGlassesPos][1], pSkin[playerid][pSkinGlassesPos][2], 
                                pSkin[playerid][pSkinGlassesPos][3], pSkin[playerid][pSkinGlassesPos][4], pSkin[playerid][pSkinGlassesPos][5], 
                                pSkin[playerid][pSkinGlassesPos][6], pSkin[playerid][pSkinGlassesPos][7], pSkin[playerid][pSkinGlassesPos][8]);
    }

    if(pSkin[playerid][pSkinMask] != -1) {
        SetPlayerAttachedObject(playerid, 3, pSkin[playerid][pSkinMask], 2, pSkin[playerid][pSkinMaskPos][0], pSkin[playerid][pSkinMaskPos][1], pSkin[playerid][pSkinMaskPos][2], 
                                pSkin[playerid][pSkinMaskPos][3], pSkin[playerid][pSkinMaskPos][4], pSkin[playerid][pSkinMaskPos][5], 
                                pSkin[playerid][pSkinMaskPos][6], pSkin[playerid][pSkinMaskPos][7], pSkin[playerid][pSkinMaskPos][8]);
    }
    if(pSkin[playerid][pSkinBackpack] != -1) {
        SetPlayerAttachedObject(playerid, 4, pSkin[playerid][pSkinBackpack], 1, pSkin[playerid][pSkinBackpackPos][0], pSkin[playerid][pSkinBackpackPos][1], pSkin[playerid][pSkinBackpackPos][2], 
                                pSkin[playerid][pSkinBackpackPos][3], pSkin[playerid][pSkinBackpackPos][4], pSkin[playerid][pSkinBackpackPos][5], 
                                pSkin[playerid][pSkinBackpackPos][6], pSkin[playerid][pSkinBackpackPos][7], pSkin[playerid][pSkinBackpackPos][8]);
    }

    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}

fnc:skinOnSelectSkin(playerid, const callback[])
{
    new type[64], value;
    sscanf(callback, "p<|>s[64]d", type, value);

    if(!strcmp(type, "skin"))
    {
        SetPlayerSkin(playerid, value);
    }
    else if(!strcmp(type, "hat"))
    {
        if (IsPlayerAttachedObjectSlotUsed(playerid, 1)) RemovePlayerAttachedObject(playerid, 1);
        SetPlayerAttachedObject(playerid, 1, value, 2);
    }
    else if(!strcmp(type, "glasses"))
    {
        if (IsPlayerAttachedObjectSlotUsed(playerid, 2)) RemovePlayerAttachedObject(playerid, 2);
        SetPlayerAttachedObject(playerid, 2, value, 2);
    }
    else if(!strcmp(type, "mask"))
    {
        if (IsPlayerAttachedObjectSlotUsed(playerid, 3)) RemovePlayerAttachedObject(playerid, 3);
        SetPlayerAttachedObject(playerid, 3, value, 2);   
    }
    else if(!strcmp(type, "backpack"))
    {
        if (IsPlayerAttachedObjectSlotUsed(playerid, 4)) RemovePlayerAttachedObject(playerid, 4);
        SetPlayerAttachedObject(playerid, 4, value, 1);   
    }
    return 1;
}

fnc:skinOnApplySkin(playerid, const callback[])
{
    new skin, hat, glasses, mask, backpack;

    sscanf(callback, "p<|>ddddd", skin, hat, glasses, mask, backpack);
    if(skin != -1)
    {
        pSkin[playerid][pSkinID] = skin;
        SetPlayerSkin(playerid, skin);
    }

    if(hat != -1)
    {
        pSkin[playerid][pSkinHat] = hat;
        if (IsPlayerAttachedObjectSlotUsed(playerid, 1)) RemovePlayerAttachedObject(playerid, 1);
        SetPlayerAttachedObject(playerid, 1, hat, 2);
    }

    if(glasses != -1)
    {
        pSkin[playerid][pSkinGlasses] = glasses;
        if (IsPlayerAttachedObjectSlotUsed(playerid, 2)) RemovePlayerAttachedObject(playerid, 2);
        SetPlayerAttachedObject(playerid, 2, glasses, 2);
    }

    if(mask != -1)
    {
        pSkin[playerid][pSkinMask] = mask;
        if (IsPlayerAttachedObjectSlotUsed(playerid, 3)) RemovePlayerAttachedObject(playerid, 3);
        SetPlayerAttachedObject(playerid, 3, mask, 2);
    }

    if(backpack != -1)
    {
        pSkin[playerid][pSkinBackpack] = backpack;
        if (IsPlayerAttachedObjectSlotUsed(playerid, 4)) RemovePlayerAttachedObject(playerid, 4);
        SetPlayerAttachedObject(playerid, 4, backpack, 1);
    }    

    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "UPDATE `krakterskin` SET `skin` = %d, `hat` = %d, `glasses` = %d, `mask` = %d, `backpack` = %d WHERE `identifier` = '%s'",
        pSkin[playerid][pSkinID], pSkin[playerid][pSkinHat], pSkin[playerid][pSkinGlasses], pSkin[playerid][pSkinMask], pSkin[playerid][pSkinBackpack], pUser[playerid][pIdentifier]);
    mysql_tquery(g_SQL, query);
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);    
    return 1;
}

fnc:skinOnBuyOutfitList(playerid, const callback[])
{
    cef_emit_event(playerid, "ui:showSkinMenu", CEFSTR(pUserD[playerid][pdGender]));    
    return 1;
}

fnc:skinOnPreviewSkinList(playerid, const callback[])
{
    new sqlID = strval(callback);
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `krakterskinlist` WHERE `identifier` = '%s' AND `id` = %d", pUser[playerid][pIdentifier], sqlID);
    mysql_tquery(g_SQL, query, "skinOnPreviewSkinListQuery", "d", playerid);
    return 1;
}

fnc:skinOnPreviewSkinListQuery(playerid)
{
    new rows = cache_num_rows();
    if(rows) {
        new skin, hat, glasses, mask, backpack;
        new Float:hatPos[9], Float:glassesPos[9], Float:maskPos[9], Float:backpackPos[9];
        cache_get_value_name_int(0, "skin", skin);
        cache_get_value_name_int(0, "hat", hat);
        cache_get_value_name_float(0, "hatposX", hatPos[0]);
        cache_get_value_name_float(0, "hatposY", hatPos[1]);
        cache_get_value_name_float(0, "hatposZ", hatPos[2]);
        cache_get_value_name_float(0, "hatrotX", hatPos[3]);
        cache_get_value_name_float(0, "hatrotY", hatPos[4]);
        cache_get_value_name_float(0, "hatrotZ", hatPos[5]);
        cache_get_value_name_float(0, "hatscaleX", hatPos[6]);
        cache_get_value_name_float(0, "hatscaleY", hatPos[7]);
        cache_get_value_name_float(0, "hatscaleZ", hatPos[8]);
        cache_get_value_name_int(0, "glasses", glasses);
        cache_get_value_name_float(0, "glassesposX", glassesPos[0]);
        cache_get_value_name_float(0, "glassesposY", glassesPos[1]);
        cache_get_value_name_float(0, "glassesposZ", glassesPos[2]);
        cache_get_value_name_float(0, "glassesrotX", glassesPos[3]);
        cache_get_value_name_float(0, "glassesrotY", glassesPos[4]);
        cache_get_value_name_float(0, "glassesrotZ", glassesPos[5]);
        cache_get_value_name_float(0, "glassesscaleX", glassesPos[6]);
        cache_get_value_name_float(0, "glassesscaleY", glassesPos[7]);
        cache_get_value_name_float(0, "glassesscaleZ", glassesPos[8]);
        cache_get_value_name_int(0, "mask", mask);
        cache_get_value_name_float(0, "maskposX", maskPos[0]);
        cache_get_value_name_float(0, "maskposY", maskPos[1]);
        cache_get_value_name_float(0, "maskposZ", maskPos[2]);
        cache_get_value_name_float(0, "maskrotX", maskPos[3]);
        cache_get_value_name_float(0, "maskrotY", maskPos[4]);
        cache_get_value_name_float(0, "maskrotZ", maskPos[5]);
        cache_get_value_name_float(0, "maskscaleX", maskPos[6]);
        cache_get_value_name_float(0, "maskscaleY", maskPos[7]);
        cache_get_value_name_float(0, "maskscaleZ", maskPos[8]);
        cache_get_value_name_int(0, "backpack", backpack);
        cache_get_value_name_float(0, "backpackposX", backpackPos[0]);
        cache_get_value_name_float(0, "backpackposY", backpackPos[1]);
        cache_get_value_name_float(0, "backpackposZ", backpackPos[2]);
        cache_get_value_name_float(0, "backpackrotX", backpackPos[3]);
        cache_get_value_name_float(0, "backpackrotY", backpackPos[4]);
        cache_get_value_name_float(0, "backpackrotZ", backpackPos[5]);
        cache_get_value_name_float(0, "backpackscaleX", backpackPos[6]);
        cache_get_value_name_float(0, "backpackscaleY", backpackPos[7]);
        cache_get_value_name_float(0, "backpackscaleZ", backpackPos[8]);

        if (IsPlayerAttachedObjectSlotUsed(playerid, 1)) RemovePlayerAttachedObject(playerid, 1);
        
        if (IsPlayerAttachedObjectSlotUsed(playerid, 2)) RemovePlayerAttachedObject(playerid, 2);
            
        if (IsPlayerAttachedObjectSlotUsed(playerid, 3)) RemovePlayerAttachedObject(playerid, 3);
            
        if (IsPlayerAttachedObjectSlotUsed(playerid, 4)) RemovePlayerAttachedObject(playerid, 4);
        
        if(skin != -1)
        {
            SetPlayerSkin(playerid, skin);
        }

        if(hat != -1)
        {
            SetPlayerAttachedObject(playerid, 1, hat, 2, hatPos[0], hatPos[1], hatPos[2], hatPos[3], hatPos[4], hatPos[5], hatPos[6], hatPos[7], hatPos[8]);
        }

        if(glasses != -1)
        {
            SetPlayerAttachedObject(playerid, 2, glasses, 2, glassesPos[0], glassesPos[1], glassesPos[2], glassesPos[3], glassesPos[4], glassesPos[5], glassesPos[6], glassesPos[7], glassesPos[8]);
        }

        if(mask != -1)
        {
            SetPlayerAttachedObject(playerid, 3, mask, 2, maskPos[0], maskPos[1], maskPos[2], maskPos[3], maskPos[4], maskPos[5], maskPos[6], maskPos[7], maskPos[8]);
        }

        if(backpack != -1)
        {
            SetPlayerAttachedObject(playerid, 4, backpack, 1, backpackPos[0], backpackPos[1], backpackPos[2], backpackPos[3], backpackPos[4], backpackPos[5], backpackPos[6], backpackPos[7], backpackPos[8]);
        }
    }

    return 1;
}

fnc:skinOnApplySkinList(playerid, const callback[])
{
    new sqlID = strval(callback);
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `krakterskinlist` WHERE `identifier` = '%s' AND `id` = %d", pUser[playerid][pIdentifier], sqlID);
    mysql_tquery(g_SQL, query, "skinOnApplySkinListQuery", "d", playerid);
    return 1;
}

fnc:skinOnApplySkinListQuery(playerid)
{
    new rows = cache_num_rows();
    if(rows) {
        new skin, hat, glasses, mask, backpack;
        new Float:hatPos[9], Float:glassesPos[9], Float:maskPos[9], Float:backpackPos[9];
        cache_get_value_name_int(0, "skin", skin);
        cache_get_value_name_int(0, "hat", hat);
        cache_get_value_name_float(0, "hatposX", hatPos[0]);
        cache_get_value_name_float(0, "hatposY", hatPos[1]);
        cache_get_value_name_float(0, "hatposZ", hatPos[2]);
        cache_get_value_name_float(0, "hatrotX", hatPos[3]);
        cache_get_value_name_float(0, "hatrotY", hatPos[4]);
        cache_get_value_name_float(0, "hatrotZ", hatPos[5]);
        cache_get_value_name_float(0, "hatscaleX", hatPos[6]);
        cache_get_value_name_float(0, "hatscaleY", hatPos[7]);
        cache_get_value_name_float(0, "hatscaleZ", hatPos[8]);
        cache_get_value_name_int(0, "glasses", glasses);
        cache_get_value_name_float(0, "glassesposX", glassesPos[0]);
        cache_get_value_name_float(0, "glassesposY", glassesPos[1]);
        cache_get_value_name_float(0, "glassesposZ", glassesPos[2]);
        cache_get_value_name_float(0, "glassesrotX", glassesPos[3]);
        cache_get_value_name_float(0, "glassesrotY", glassesPos[4]);
        cache_get_value_name_float(0, "glassesrotZ", glassesPos[5]);
        cache_get_value_name_float(0, "glassesscaleX", glassesPos[6]);
        cache_get_value_name_float(0, "glassesscaleY", glassesPos[7]);
        cache_get_value_name_float(0, "glassesscaleZ", glassesPos[8]);
        cache_get_value_name_int(0, "mask", mask);
        cache_get_value_name_float(0, "maskposX", maskPos[0]);
        cache_get_value_name_float(0, "maskposY", maskPos[1]);
        cache_get_value_name_float(0, "maskposZ", maskPos[2]);
        cache_get_value_name_float(0, "maskrotX", maskPos[3]);
        cache_get_value_name_float(0, "maskrotY", maskPos[4]);
        cache_get_value_name_float(0, "maskrotZ", maskPos[5]);
        cache_get_value_name_float(0, "maskscaleX", maskPos[6]);
        cache_get_value_name_float(0, "maskscaleY", maskPos[7]);
        cache_get_value_name_float(0, "maskscaleZ", maskPos[8]);
        cache_get_value_name_int(0, "backpack", backpack);
        cache_get_value_name_float(0, "backpackposX", backpackPos[0]);
        cache_get_value_name_float(0, "backpackposY", backpackPos[1]);
        cache_get_value_name_float(0, "backpackposZ", backpackPos[2]);
        cache_get_value_name_float(0, "backpackrotX", backpackPos[3]);
        cache_get_value_name_float(0, "backpackrotY", backpackPos[4]);
        cache_get_value_name_float(0, "backpackrotZ", backpackPos[5]);
        cache_get_value_name_float(0, "backpackscaleX", backpackPos[6]);
        cache_get_value_name_float(0, "backpackscaleY", backpackPos[7]);
        cache_get_value_name_float(0, "backpackscaleZ", backpackPos[8]);

        pSkin[playerid][pSkinID] = skin;
        pSkin[playerid][pSkinHat] = hat;
        pSkin[playerid][pSkinGlasses] = glasses;
        pSkin[playerid][pSkinMask] = mask;
        pSkin[playerid][pSkinBackpack] = backpack;

        forex(i, 9)
        {
            pSkin[playerid][pSkinHatPos][i] = hatPos[i];
            pSkin[playerid][pSkinGlassesPos][i] = glassesPos[i];
            pSkin[playerid][pSkinMaskPos][i] = maskPos[i];
            pSkin[playerid][pSkinBackpackPos][i] = backpackPos[i];
        }

        if (IsPlayerAttachedObjectSlotUsed(playerid, 1)) RemovePlayerAttachedObject(playerid, 1);
        
        if (IsPlayerAttachedObjectSlotUsed(playerid, 2)) RemovePlayerAttachedObject(playerid, 2);
            
        if (IsPlayerAttachedObjectSlotUsed(playerid, 3)) RemovePlayerAttachedObject(playerid, 3);
            
        if (IsPlayerAttachedObjectSlotUsed(playerid, 4)) RemovePlayerAttachedObject(playerid, 4);
        
        if(skin != -1)
        {
            SetPlayerSkin(playerid, skin);
        }

        if(hat != -1)
        {
            SetPlayerAttachedObject(playerid, 1, hat, 2, hatPos[0], hatPos[1], hatPos[2], hatPos[3], hatPos[4], hatPos[5], hatPos[6], hatPos[7], hatPos[8]);
        }

        if(glasses != -1)
        {
            SetPlayerAttachedObject(playerid, 2, glasses, 2, glassesPos[0], glassesPos[1], glassesPos[2], glassesPos[3], glassesPos[4], glassesPos[5], glassesPos[6], glassesPos[7], glassesPos[8]);
        }

        if(mask != -1)
        {
            SetPlayerAttachedObject(playerid, 3, mask, 2, maskPos[0], maskPos[1], maskPos[2], maskPos[3], maskPos[4], maskPos[5], maskPos[6], maskPos[7], maskPos[8]);
        }

        if(backpack != -1)
        {
            SetPlayerAttachedObject(playerid, 4, backpack, 1, backpackPos[0], backpackPos[1], backpackPos[2], backpackPos[3], backpackPos[4], backpackPos[5], backpackPos[6], backpackPos[7], backpackPos[8]);
        }

        new query[1024];
        mysql_format(g_SQL, query, sizeof(query),
            "UPDATE `krakterskin` SET \
            skin = %d, \
            hat = %d, hatposX = %f, hatposY = %f, hatposZ = %f, hatrotX = %f, hatrotY = %f, hatrotZ = %f, hatscaleX = %f, hatscaleY = %f, hatscaleZ = %f, \
            glasses = %d, glassesposX = %f, glassesposY = %f, glassesposZ = %f, glassesrotX = %f, glassesrotY = %f, glassesrotZ = %f, glassesscaleX = %f, glassesscaleY = %f, glassesscaleZ = %f, \
            mask = %d, maskposX = %f, maskposY = %f, maskposZ = %f, maskrotX = %f, maskrotY = %f, maskrotZ = %f, maskscaleX = %f, maskscaleY = %f, maskscaleZ = %f, \
            backpack = %d, backpackposX = %f, backpackposY = %f, backpackposZ = %f, backpackrotX = %f, backpackrotY = %f, backpackrotZ = %f, backpackscaleX = %f, backpackscaleY = %f, backpackscaleZ = %f \
            WHERE identifier = '%s'",
            
            pSkin[playerid][pSkinID],
            pSkin[playerid][pSkinHat],
            pSkin[playerid][pSkinHatPos][0], pSkin[playerid][pSkinHatPos][1], pSkin[playerid][pSkinHatPos][2],
            pSkin[playerid][pSkinHatPos][3], pSkin[playerid][pSkinHatPos][4], pSkin[playerid][pSkinHatPos][5],
            pSkin[playerid][pSkinHatPos][6], pSkin[playerid][pSkinHatPos][7], pSkin[playerid][pSkinHatPos][8],

            pSkin[playerid][pSkinGlasses],
            pSkin[playerid][pSkinGlassesPos][0], pSkin[playerid][pSkinGlassesPos][1], pSkin[playerid][pSkinGlassesPos][2],
            pSkin[playerid][pSkinGlassesPos][3], pSkin[playerid][pSkinGlassesPos][4], pSkin[playerid][pSkinGlassesPos][5],
            pSkin[playerid][pSkinGlassesPos][6], pSkin[playerid][pSkinGlassesPos][7], pSkin[playerid][pSkinGlassesPos][8],

            pSkin[playerid][pSkinMask],
            pSkin[playerid][pSkinMaskPos][0], pSkin[playerid][pSkinMaskPos][1], pSkin[playerid][pSkinMaskPos][2],
            pSkin[playerid][pSkinMaskPos][3], pSkin[playerid][pSkinMaskPos][4], pSkin[playerid][pSkinMaskPos][5],
            pSkin[playerid][pSkinMaskPos][6], pSkin[playerid][pSkinMaskPos][7], pSkin[playerid][pSkinMaskPos][8],

            pSkin[playerid][pSkinBackpack],
            pSkin[playerid][pSkinBackpackPos][0], pSkin[playerid][pSkinBackpackPos][1], pSkin[playerid][pSkinBackpackPos][2],
            pSkin[playerid][pSkinBackpackPos][3], pSkin[playerid][pSkinBackpackPos][4], pSkin[playerid][pSkinBackpackPos][5],
            pSkin[playerid][pSkinBackpackPos][6], pSkin[playerid][pSkinBackpackPos][7], pSkin[playerid][pSkinBackpackPos][8],

            pUser[playerid][pIdentifier]
        );
        mysql_query(g_SQL, query);

        cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);
    }
}

fnc:skinOnpdateSkinList(playerid, const callback[])
{
    new sqlID = strval(callback);
    new query[1024];

    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `krakterskinlist` SET \
        skin = %d, \
        hat = %d, hatposX = %f, hatposY = %f, hatposZ = %f, hatrotX = %f, hatrotY = %f, hatrotZ = %f, hatscaleX = %f, hatscaleY = %f, hatscaleZ = %f, \
        glasses = %d, glassesposX = %f, glassesposY = %f, glassesposZ = %f, glassesrotX = %f, glassesrotY = %f, glassesrotZ = %f, glassesscaleX = %f, glassesscaleY = %f, glassesscaleZ = %f, \
        mask = %d, maskposX = %f, maskposY = %f, maskposZ = %f, maskrotX = %f, maskrotY = %f, maskrotZ = %f, maskscaleX = %f, maskscaleY = %f, maskscaleZ = %f, \
        backpack = %d, backpackposX = %f, backpackposY = %f, backpackposZ = %f, backpackrotX = %f, backpackrotY = %f, backpackrotZ = %f, backpackscaleX = %f, backpackscaleY = %f, backpackscaleZ = %f \
        WHERE id = %d",
        
        pSkin[playerid][pSkinID],
        pSkin[playerid][pSkinHat],
        pSkin[playerid][pSkinHatPos][0], pSkin[playerid][pSkinHatPos][1], pSkin[playerid][pSkinHatPos][2],
        pSkin[playerid][pSkinHatPos][3], pSkin[playerid][pSkinHatPos][4], pSkin[playerid][pSkinHatPos][5],
        pSkin[playerid][pSkinHatPos][6], pSkin[playerid][pSkinHatPos][7], pSkin[playerid][pSkinHatPos][8],

        pSkin[playerid][pSkinGlasses],
        pSkin[playerid][pSkinGlassesPos][0], pSkin[playerid][pSkinGlassesPos][1], pSkin[playerid][pSkinGlassesPos][2],
        pSkin[playerid][pSkinGlassesPos][3], pSkin[playerid][pSkinGlassesPos][4], pSkin[playerid][pSkinGlassesPos][5],
        pSkin[playerid][pSkinGlassesPos][6], pSkin[playerid][pSkinGlassesPos][7], pSkin[playerid][pSkinGlassesPos][8],

        pSkin[playerid][pSkinMask],
        pSkin[playerid][pSkinMaskPos][0], pSkin[playerid][pSkinMaskPos][1], pSkin[playerid][pSkinMaskPos][2],
        pSkin[playerid][pSkinMaskPos][3], pSkin[playerid][pSkinMaskPos][4], pSkin[playerid][pSkinMaskPos][5],
        pSkin[playerid][pSkinMaskPos][6], pSkin[playerid][pSkinMaskPos][7], pSkin[playerid][pSkinMaskPos][8],

        pSkin[playerid][pSkinBackpack],
        pSkin[playerid][pSkinBackpackPos][0], pSkin[playerid][pSkinBackpackPos][1], pSkin[playerid][pSkinBackpackPos][2],
        pSkin[playerid][pSkinBackpackPos][3], pSkin[playerid][pSkinBackpackPos][4], pSkin[playerid][pSkinBackpackPos][5],
        pSkin[playerid][pSkinBackpackPos][6], pSkin[playerid][pSkinBackpackPos][7], pSkin[playerid][pSkinBackpackPos][8],
        sqlID
    );

    mysql_tquery(g_SQL, query);

    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}

fnc:skinOnSaveOutfitList(playerid, const callback[])
{
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    Dialog_Show(playerid, "SaveSkinOutfitList", DIALOG_STYLE_INPUT, "Save outfit list", "Masukan Nama Outfit", "Save", "Cancel");
}

Dialog:SaveSkinOutfitList(playerid, response, listitem, inputtext[])
{
    if(!response) {
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);
    }

    if (strlen(inputtext) == 0) {
        Dialog_Show(playerid, "SaveSkinOutfitList", DIALOG_STYLE_INPUT, "Save outfit list", "Masukan Nama Outfit ", "Save", "Cancel");
        return 1;
    }

    new query[1024];
    mysql_format(g_SQL, query, sizeof(query), "INSERT INTO `krakterskinlist` SET `identifier` = '%s', `name` = '%s', `skin` = %d, `hat` = %d, `glasses` = %d, `mask` = %d, `backpack` = %d", 
        pUser[playerid][pIdentifier], inputtext, pSkin[playerid][pSkinID], pSkin[playerid][pSkinHat], pSkin[playerid][pSkinGlasses], pSkin[playerid][pSkinMask], pSkin[playerid][pSkinBackpack]);
    mysql_tquery(g_SQL, query);

    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}

fnc:skinOnDeleteSkinList(playerid, const callback[])
{
    new sqlID = strval(callback);
    new query[1024];
    mysql_format(g_SQL, query, sizeof(query), "DELETE FROM `krakterskinlist` WHERE `identifier` = '%s' AND `id` = %d", pUser[playerid][pIdentifier], sqlID);
    mysql_tquery(g_SQL, query);
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, true);
    return 1;
}