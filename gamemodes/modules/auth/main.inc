stock authCheckLauncherToken(playerid)
{
    if(!authValidatePlayer(playerid)) return 0;
    
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `launchertoken` WHERE `identifier` = '%s'", pUser[playerid][pName]);
    mysql_tquery(g_SQL, query, "authOnLauncherTokenCheck", "d", playerid);
    return 1;
}

forward authOnLauncherTokenCheck(playerid);
public authOnLauncherTokenCheck(playerid)
{
    new rows = cache_num_rows();
    if(rows) {
        new query[256];
        mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `launchertoken` WHERE `identifier` = '%s'", pUser[playerid][pName]);
        mysql_tquery(g_SQL, query, "authOnTokenValid", "d", playerid);
    }
    else {
        authKickPlayerWithMessage(playerid, "Silakan Login Mengunak Launcher District Roleplay!\nJika belum memiliki akun, silakan buat akun di https://districtroleplay.com");
        return 1;
    }
    return 1;   
}

forward authOnTokenValid(playerid);
public authOnTokenValid(playerid)
{
    SetSpawnInfo(playerid, NO_TEAM, 1, -2415.8567, -593.8060, 132.7245, 259.7749, t_WEAPON:0, 0, t_WEAPON:0, 0, t_WEAPON:0, 0);
    SpawnPlayer(playerid);  
    
    SetPlayerVirtualWorld(playerid, playerid + 1);
    SetPlayerCameraPos(playerid, -2410.679, -593.605, 134.165 + 90);
    SetPlayerCameraLookAt(playerid, -2403.808, -594.079, 132.648 + 90);

    SendClientMessage(playerid, -1, "Selamat datang di District Roleplay, {FFFF00}%s!", pUser[playerid][pName]);
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `krakter` WHERE `name` = '%s'", pUser[playerid][pName]);
    mysql_tquery(g_SQL, query, "authOnCharacterCheck", "d", playerid);
    return 1;
}
forward authOnCharacterCheck(playerid);
public authOnCharacterCheck(playerid)
{
    new rows = cache_num_rows();
    if(rows) {
        cache_get_value_name_int(0, "id", pUser[playerid][PId]);
        cache_get_value_name(0, "identifier", pUser[playerid][pIdentifier], 191);
        cache_get_value_name_int(0, "ative", pUser[playerid][pActive]);

        if(pUser[playerid][pActive] == 0) {
            SetTimerEx("authDelayedCharacterCreate", 3000, false, "d", playerid);
        }
        else
        {
            new query[256];
            mysql_format(g_SQL, query, sizeof(query), "SELECT * FROM `krakterdata` WHERE `identifier` = '%s'", pUser[playerid][pIdentifier]);
            mysql_tquery(g_SQL, query, "authOnCharacterData", "d", playerid);
        }

        return 1;
    }
    else {
        authKickPlayerWithMessage(playerid, "Anda belum memiliki karakter di District Roleplay!\nSilakan buat karakter baru di https://districtroleplay.com");
        return 1;
    }
}

// GET CHARACTER DATA
forward authOnCharacterData(playerid);
public authOnCharacterData(playerid)
{
    new rows = cache_num_rows();
    if(rows) {
        cache_get_value_name(0, "firstname", pUserD[playerid][pdFirstName], 16);
        cache_get_value_name(0, "lastname", pUserD[playerid][pdLastName], 16);
        // set nama karakter
        SetRpName(playerid, pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName]);
        cache_get_value_name(0, "birthday", pUserD[playerid][pdBirtDay], 64);
        cache_get_value_name(0, "age", pUserD[playerid][pdAge]);
        cache_get_value_name(0, "sex", pUserD[playerid][pdGender], 7);

        SetSpawnInfo(playerid, NO_TEAM, 1, -2415.8567, -593.8060, 132.7245, 259.7749, t_WEAPON:0, 0, t_WEAPON:0, 0, t_WEAPON:0, 0);
        SpawnPlayer(playerid);  
        SetPlayerVirtualWorld(playerid, 0);
        SetCameraBehindPlayer(playerid);
    }
    return 1;
}

// CREATE CHARACTER
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
    sscanf(arguments, "p<|>s[64]s[16]s[16]s[7]", pUserD[playerid][pdBirtDay], pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName], pUserD[playerid][pdGender]);
    pUserD[playerid][pdAge] = calculateAgeFromDate(pUserD[playerid][pdBirtDay]);
    
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "UPDATE `krakterdata` SET `firstname` = '%s', `lastname` = '%s', `birthday` = '%s', `age` = %d, `sex` = '%s' WHERE `identifier` = '%s'", 
        pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName], pUserD[playerid][pdBirtDay], pUserD[playerid][pdAge], pUserD[playerid][pdGender], pUser[playerid][pIdentifier]);
    mysql_tquery(g_SQL, query, "authOnCharacterDataUpdate", "d", playerid);
    
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, false);
    return 1;
}

forward authOnCharacterDataUpdate(playerid);
public authOnCharacterDataUpdate(playerid)
{
    pUser[playerid][pActive] = 1;
    
    new query[256];
    mysql_format(g_SQL, query, sizeof(query), "UPDATE `krakter` SET `ative` = 1 WHERE `identifier` = '%s'", pUser[playerid][pIdentifier]);
    mysql_tquery(g_SQL, query, "authOnCharacterActivated", "d", playerid);
    return 1;
}

forward authOnCharacterActivated(playerid);
public authOnCharacterActivated(playerid)
{
    SetPlayerPos(playerid, -2415.8567, -593.8060, 132.7245);
    SetCameraBehindPlayer(playerid);
    return 1;
}
