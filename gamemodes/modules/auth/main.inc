// =============================================================================
// Authentication Module
// =============================================================================
// Handles player authentication, launcher token verification, and character management

// =============================================================================
// Launcher Token Verification
// =============================================================================

stock authCheckLauncherToken(playerid)
{
    if (!authValidatePlayer(playerid)) 
        return 0;
    
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), 
        "SELECT * FROM `launchertoken` WHERE `identifier` = '%s'", 
        pUser[playerid][pName]);
    mysql_tquery(g_SQL, query, "authOnLauncherTokenCheck", "d", playerid);
    return 1;
}

forward authOnLauncherTokenCheck(playerid);
public authOnLauncherTokenCheck(playerid)
{
    new rows = cache_num_rows();
    if (rows) 
    {
        authOnTokenValid(playerid);
    }
    else 
    {
        authKickPlayerWithMessage(playerid, "Silakan Login Mengunak Launcher District Roleplay!\nJika belum memiliki akun, silakan buat akun di https://districtroleplay.com");
        return 1;
    }
    return 1;   
}

stock authOnTokenValid(playerid)
{
    // Set initial spawn information
    SetSpawnInfo(playerid, NO_TEAM, 1, -2415.8567, -593.8060, 132.7245, 259.7749, 
        t_WEAPON:0, 0, t_WEAPON:0, 0, t_WEAPON:0, 0);
    SpawnPlayer(playerid);  
    
    // Set player to isolated virtual world and camera
    SetPlayerVirtualWorld(playerid, playerid + 1);
    SetPlayerCameraPos(playerid, -2410.679, -593.605, 134.165 + 90);
    SetPlayerCameraLookAt(playerid, -2403.808, -594.079, 132.648 + 90);

    SendClientMessage(playerid, -1, "Selamat datang di District Roleplay, {FFFF00}%s!", pUser[playerid][pName]);
    
    // Check if character exists
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), 
        "SELECT * FROM `krakter` WHERE `name` = '%s'", 
        pUser[playerid][pName]);
    mysql_tquery(g_SQL, query, "authOnCharacterCheck", "d", playerid);
    return 1;
}

// =============================================================================
// Character Management
// =============================================================================

forward authOnCharacterCheck(playerid);
public authOnCharacterCheck(playerid)
{
    new rows = cache_num_rows();
    if (rows) 
    {
        // Get character basic information
        cache_get_value_name_int(0, "id", pUser[playerid][PId]);
        cache_get_value_name(0, "identifier", pUser[playerid][pIdentifier], 191);
        cache_get_value_name_int(0, "ative", pUser[playerid][pActive]);
        cache_get_value_name_int(0, "admin", pUser[playerid][pAdmin]);
        cache_get_value_name_int(0, "adminpoint", pUser[playerid][pAdminPoint]);

        if (pUser[playerid][pActive] == 0) 
        {
            // Character exists but not activated - show creation UI
            SetTimerEx("authDelayedCharacterCreate", 3000, false, "d", playerid);
        }
        else
        {
            // Character is active - load character data
            new query[256];
            mysql_format(g_SQL, query, sizeof(query), 
                "SELECT * FROM `krakterdata` WHERE `identifier` = '%s'", 
                pUser[playerid][pIdentifier]);
            mysql_tquery(g_SQL, query, "authOnCharacterData", "d", playerid);
        }
        return 1;
    }
    else 
    {
        // No character found - kick player
        authKickPlayerWithMessage(playerid, "Anda belum memiliki karakter di District Roleplay!\nSilakan buat karakter baru di https://districtroleplay.com");
        return 1;
    }
}

// =============================================================================
// Character Data Loading
// =============================================================================

forward authOnCharacterData(playerid);
public authOnCharacterData(playerid)
{
    new rows = cache_num_rows();
    if (rows) 
    {
        // Load basic character information
        cache_get_value_name(0, "firstname", pUserD[playerid][pdFirstName], 16);
        cache_get_value_name(0, "lastname", pUserD[playerid][pdLastName], 16);
        SetRpName(playerid, pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName]);
        
        cache_get_value_name(0, "birthday", pUserD[playerid][pdBirtDay], 64);
        cache_get_value_name_int(0, "age", pUserD[playerid][pdAge]);
        cache_get_value_name(0, "sex", pUserD[playerid][pdGender], 7);

        // Load position and world data
        cache_get_value_name_float(0, "posx", pUserD[playerid][pdPos][0]);
        cache_get_value_name_float(0, "posy", pUserD[playerid][pdPos][1]);
        cache_get_value_name_float(0, "posz", pUserD[playerid][pdPos][2]);
        cache_get_value_name_int(0, "virtualid", pUserD[playerid][pdVirtualID]);
        cache_get_value_name_int(0, "worldid", pUserD[playerid][pdWorldID]);

        // Load financial and status data
        cache_get_value_name_int(0, "money", pUserD[playerid][pdMoney]);
        cache_get_value_name_int(0, "bank", pUserD[playerid][pdMoneyBank]);
        cache_get_value_name_float(0, "health", pUserD[playerid][pdHealth]);
        cache_get_value_name_float(0, "armor", pUserD[playerid][pdArmour]);
        cache_get_value_name_float(0, "hunger", pUserD[playerid][pdHunger]);
        cache_get_value_name_float(0, "thirst", pUserD[playerid][pdThirst]);
        cache_get_value_name_float(0, "stress", pUserD[playerid][pdStrees]);

        // Load skin data
        new query[256];
        mysql_format(g_SQL, query, sizeof(query), 
            "SELECT * FROM `krakterskin` WHERE `identifier` = '%s'", 
            pUser[playerid][pIdentifier]);
        mysql_tquery(g_SQL, query, "authOnCharacterSkinData", "d", playerid);
    }
    return 1;
}

// =============================================================================
// Skin Data Loading
// =============================================================================

forward authOnCharacterSkinData(playerid);
public authOnCharacterSkinData(playerid)
{
    new rows = cache_num_rows();
    if (rows) 
    {
        // Load basic skin information
        cache_get_value_name_int(0, "skin", pSkin[playerid][pSkinID]);
        
        // Load hat attachment data
        cache_get_value_name_int(0, "hat", pSkin[playerid][pSkinHat]);
        cache_get_value_name_float(0, "hatposX", pSkin[playerid][pSkinHatPos][0]);
        cache_get_value_name_float(0, "hatposY", pSkin[playerid][pSkinHatPos][1]);
        cache_get_value_name_float(0, "hatposZ", pSkin[playerid][pSkinHatPos][2]);
        cache_get_value_name_float(0, "hatrotX", pSkin[playerid][pSkinHatPos][3]);
        cache_get_value_name_float(0, "hatrotY", pSkin[playerid][pSkinHatPos][4]);
        cache_get_value_name_float(0, "hatrotZ", pSkin[playerid][pSkinHatPos][5]);
        cache_get_value_name_float(0, "hatscaleX", pSkin[playerid][pSkinHatPos][6]);
        cache_get_value_name_float(0, "hatscaleY", pSkin[playerid][pSkinHatPos][7]);
        cache_get_value_name_float(0, "hatscaleZ", pSkin[playerid][pSkinHatPos][8]);
        
        // Load glasses attachment data
        cache_get_value_name_int(0, "glasses", pSkin[playerid][pSkinGlasses]);
        cache_get_value_name_float(0, "glassesposX", pSkin[playerid][pSkinGlassesPos][0]);
        cache_get_value_name_float(0, "glassesposY", pSkin[playerid][pSkinGlassesPos][1]);
        cache_get_value_name_float(0, "glassesposZ", pSkin[playerid][pSkinGlassesPos][2]);
        cache_get_value_name_float(0, "glassesrotX", pSkin[playerid][pSkinGlassesPos][3]);
        cache_get_value_name_float(0, "glassesrotY", pSkin[playerid][pSkinGlassesPos][4]);
        cache_get_value_name_float(0, "glassesrotZ", pSkin[playerid][pSkinGlassesPos][5]);
        cache_get_value_name_float(0, "glassesscaleX", pSkin[playerid][pSkinGlassesPos][6]);
        cache_get_value_name_float(0, "glassesscaleY", pSkin[playerid][pSkinGlassesPos][7]);
        cache_get_value_name_float(0, "glassesscaleZ", pSkin[playerid][pSkinGlassesPos][8]);
        
        // Load mask attachment data
        cache_get_value_name_int(0, "mask", pSkin[playerid][pSkinMask]);
        cache_get_value_name_float(0, "maskposX", pSkin[playerid][pSkinMaskPos][0]);
        cache_get_value_name_float(0, "maskposY", pSkin[playerid][pSkinMaskPos][1]);
        cache_get_value_name_float(0, "maskposZ", pSkin[playerid][pSkinMaskPos][2]);
        cache_get_value_name_float(0, "maskrotX", pSkin[playerid][pSkinMaskPos][3]);
        cache_get_value_name_float(0, "maskrotY", pSkin[playerid][pSkinMaskPos][4]);
        cache_get_value_name_float(0, "maskrotZ", pSkin[playerid][pSkinMaskPos][5]);
        cache_get_value_name_float(0, "maskscaleX", pSkin[playerid][pSkinMaskPos][6]);
        cache_get_value_name_float(0, "maskscaleY", pSkin[playerid][pSkinMaskPos][7]);
        cache_get_value_name_float(0, "maskscaleZ", pSkin[playerid][pSkinMaskPos][8]);
        
        // Load backpack attachment data
        cache_get_value_name_int(0, "backpack", pSkin[playerid][pSkinBackpack]);
        cache_get_value_name_float(0, "backpackposX", pSkin[playerid][pSkinBackpackPos][0]);
        cache_get_value_name_float(0, "backpackposY", pSkin[playerid][pSkinBackpackPos][1]);
        cache_get_value_name_float(0, "backpackposZ", pSkin[playerid][pSkinBackpackPos][2]);
        cache_get_value_name_float(0, "backpackrotX", pSkin[playerid][pSkinBackpackPos][3]);
        cache_get_value_name_float(0, "backpackrotY", pSkin[playerid][pSkinBackpackPos][4]);
        cache_get_value_name_float(0, "backpackrotZ", pSkin[playerid][pSkinBackpackPos][5]);
        cache_get_value_name_float(0, "backpackscaleX", pSkin[playerid][pSkinBackpackPos][6]);
        cache_get_value_name_float(0, "backpackscaleY", pSkin[playerid][pSkinBackpackPos][7]);
        cache_get_value_name_float(0, "backpackscaleZ", pSkin[playerid][pSkinBackpackPos][8]);
        
        // Set player properties and spawn
        SetPlayerInterior(playerid, pUserD[playerid][pdVirtualID]);
        SetPlayerVirtualWorld(playerid, pUserD[playerid][pdWorldID]);
        TogglePlayerControllable(playerid, true);
        SetPlayerSkin(playerid, 1);
        SetPlayerPos(playerid, pUserD[playerid][pdPos][0], pUserD[playerid][pdPos][1], pUserD[playerid][pdPos][2]);
        SetCameraBehindPlayer(playerid);
        SetPlayerHealth(playerid, pUserD[playerid][pdHealth]);
        SetPlayerArmour(playerid, pUserD[playerid][pdArmour]);

        //get player vehicles
        new query[256];
        mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `krakterveh` WHERE `identifier` = '%s'", pUser[playerid][pIdentifier]);
        mysql_tquery(g_SQL, query, "vehOnPlayerLoad", "d", playerid);
        // Initialize UI and attachments
        SetTimerEx("authUIReady", 3000, false, "d", playerid);
        SetTimerEx("authDelayedCharacterAttachProp", 500, false, "i", playerid);
    }
    return 1;
}

// =============================================================================
// Character Attachment Management
// =============================================================================

forward authDelayedCharacterAttachProp(playerid);
public authDelayedCharacterAttachProp(playerid)
{
    // Remove any existing attachments
    forex(i, 4)
    {
        if (IsPlayerAttachedObjectSlotUsed(playerid, i)) 
        {
            RemovePlayerAttachedObject(playerid, i);
        }
    }

    // Set player skin
    SetPlayerSkin(playerid, pSkin[playerid][pSkinID]);
    
    // Attach hat if available
    if (pSkin[playerid][pSkinHat] != -1) 
    {
        SetPlayerAttachedObject(playerid, 1, pSkin[playerid][pSkinHat], 2, 
            pSkin[playerid][pSkinHatPos][0], pSkin[playerid][pSkinHatPos][1], pSkin[playerid][pSkinHatPos][2], 
            pSkin[playerid][pSkinHatPos][3], pSkin[playerid][pSkinHatPos][4], pSkin[playerid][pSkinHatPos][5], 
            pSkin[playerid][pSkinHatPos][6], pSkin[playerid][pSkinHatPos][7], pSkin[playerid][pSkinHatPos][8]);
    }

    // Attach glasses if available
    if (pSkin[playerid][pSkinGlasses] != -1) 
    {
        SetPlayerAttachedObject(playerid, 2, pSkin[playerid][pSkinGlasses], 2, 
            pSkin[playerid][pSkinGlassesPos][0], pSkin[playerid][pSkinGlassesPos][1], pSkin[playerid][pSkinGlassesPos][2], 
            pSkin[playerid][pSkinGlassesPos][3], pSkin[playerid][pSkinGlassesPos][4], pSkin[playerid][pSkinGlassesPos][5], 
            pSkin[playerid][pSkinGlassesPos][6], pSkin[playerid][pSkinGlassesPos][7], pSkin[playerid][pSkinGlassesPos][8]);
    }

    // Attach mask if available
    if (pSkin[playerid][pSkinMask] != -1) 
    {
        SetPlayerAttachedObject(playerid, 3, pSkin[playerid][pSkinMask], 2, 
            pSkin[playerid][pSkinMaskPos][0], pSkin[playerid][pSkinMaskPos][1], pSkin[playerid][pSkinMaskPos][2], 
            pSkin[playerid][pSkinMaskPos][3], pSkin[playerid][pSkinMaskPos][4], pSkin[playerid][pSkinMaskPos][5], 
            pSkin[playerid][pSkinMaskPos][6], pSkin[playerid][pSkinMaskPos][7], pSkin[playerid][pSkinMaskPos][8]);
    }
    
    // Attach backpack if available
    if (pSkin[playerid][pSkinBackpack] != -1) 
    {
        SetPlayerAttachedObject(playerid, 4, pSkin[playerid][pSkinBackpack], 1, 
            pSkin[playerid][pSkinBackpackPos][0], pSkin[playerid][pSkinBackpackPos][1], pSkin[playerid][pSkinBackpackPos][2], 
            pSkin[playerid][pSkinBackpackPos][3], pSkin[playerid][pSkinBackpackPos][4], pSkin[playerid][pSkinBackpackPos][5], 
            pSkin[playerid][pSkinBackpackPos][6], pSkin[playerid][pSkinBackpackPos][7], pSkin[playerid][pSkinBackpackPos][8]);
    }
    return 1;
}

// =============================================================================
// Character Creation
// =============================================================================

forward authDelayedCharacterCreate(playerid);
public authDelayedCharacterCreate(playerid)
{
    cef_emit_event(playerid, "ui:showcreatecharacter", CEFSTR("true"));
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, true);
    return 1;
}

forward authOnCreateCharacter(playerid, const arguments[]);
public authOnCreateCharacter(playerid, const arguments[])
{
    // Parse character creation data
    sscanf(arguments, "p<|>s[64]s[16]s[16]s[7]", 
        pUserD[playerid][pdBirtDay], 
        pUserD[playerid][pdFirstName], 
        pUserD[playerid][pdLastName], 
        pUserD[playerid][pdGender]);
    
    // Calculate age from birthday
    pUserD[playerid][pdAge] = calculateAgeFromDate(pUserD[playerid][pdBirtDay]);
    
    // Update character data
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), 
        "UPDATE `krakterdata` SET `firstname` = '%s', `lastname` = '%s', `birthday` = '%s', `age` = %d, `sex` = '%s' WHERE `identifier` = '%s'", 
        pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName], pUserD[playerid][pdBirtDay], 
        pUserD[playerid][pdAge], pUserD[playerid][pdGender], pUser[playerid][pIdentifier]);
    mysql_tquery(g_SQL, query, "authOnCharacterDataUpdate", "d", playerid);
    
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    return 1;
}

forward authOnCharacterDataUpdate(playerid);
public authOnCharacterDataUpdate(playerid)
{
    pUser[playerid][pActive] = 1;
    
    // Activate character
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), 
        "UPDATE `krakter` SET `ative` = 1 WHERE `identifier` = '%s'", 
        pUser[playerid][pIdentifier]);
    mysql_tquery(g_SQL, query, "authOnCharacterActivated", "d", playerid);
    return 1;
}

forward authOnCharacterActivated(playerid);
public authOnCharacterActivated(playerid)
{
    // Insert character data
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), 
        "INSERT INTO `krakterdata` (`identifier`, `firstname`, `lastname`, `birthday`, `age`, `sex`) VALUES ('%s', '%s', '%s', '%s', %d, '%s')", 
        pUser[playerid][pIdentifier], pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName], 
        pUserD[playerid][pdBirtDay], pUserD[playerid][pdAge], pUserD[playerid][pdGender]);
    mysql_tquery(g_SQL, query, "authOnCharacterDataInsert", "d", playerid);
    return 1;
}

forward authOnCharacterDataInsert(playerid);
public authOnCharacterDataInsert(playerid)
{
    // Set player properties for new character
    SetPlayerInterior(playerid, pUserD[playerid][pdVirtualID]);
    SetPlayerVirtualWorld(playerid, pUserD[playerid][pdWorldID]);
    SetPlayerSkin(playerid, 1);
    SetPlayerPos(playerid, -2415.8567, -593.8060, 132.7245);
    SetCameraBehindPlayer(playerid);
    
    // Show skin selection menu
    showSkinMenu(playerid);
    return 1;
}

// =============================================================================
// Player Disconnect Handling
// =============================================================================

stock authOnPlayerDisconnect(playerid)
{
    // Save current player position and status
    GetPlayerPos(playerid, pUserD[playerid][pdPos][0], pUserD[playerid][pdPos][1], pUserD[playerid][pdPos][2]);
    GetPlayerHealth(playerid, pUserD[playerid][pdHealth]);
    GetPlayerArmour(playerid, pUserD[playerid][pdArmour]);
    
    // Update character data in database
    new cQuery[2500];
    mysql_format(g_SQL, cQuery, sizeof(cQuery), 
        "UPDATE `krakterdata` SET \
        `money` = %d, \
        `bank` = %d, \
        `posx` = %.2f, \
        `posy` = %.2f, \
        `posz` = %.2f, \
        `virtualid` = %d, \
        `worldid` = %d, \
        `health` = %.2f, \
        `armor` = %.2f, \
        `hunger` = %.2f, \
        `thirst` = %.2f, \
        `stress` = %.2f \
        WHERE `identifier` = '%s'",
        pUserD[playerid][pdMoney], 
        pUserD[playerid][pdMoneyBank], 
        pUserD[playerid][pdPos][0], 
        pUserD[playerid][pdPos][1], 
        pUserD[playerid][pdPos][2], 
        pUserD[playerid][pdVirtualID], 
        pUserD[playerid][pdWorldID], 
        pUserD[playerid][pdHealth], 
        pUserD[playerid][pdArmour], 
        pUserD[playerid][pdHunger], 
        pUserD[playerid][pdThirst], 
        pUserD[playerid][pdStrees], 
        pUser[playerid][pIdentifier]);
    mysql_tquery(g_SQL, cQuery);
    return 1;
}

forward authUIReady(playerid);
public authUIReady(playerid)
{
    cef_emit_event(playerid, "ui:showhud");
    return 1;
}