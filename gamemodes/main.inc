#define CGEN_MEMORY 20000

#include <open.mp>
#undef MAX_PLAYERS
#define MAX_PLAYERS                     100

#include <YSI_Data\y_foreach>

#include <a_mysql>
#include <sscanf2>
#include <Pawn.CMD>
#include <Pawn.RakNet>

#include <cef>
#include <easyDialog>
#include <KeyListener>
#include <streamer>

#define INTERFACE_BROWSER_ID             0xABCDE
#define INTERFACE_BROWSER_URL            "http://localhost:3000"

#define fnc:%0(%1)                      forward %0(%1); public %0(%1)
#define forex(%0,%1)                    for(new %0 = 0; %0 != %1; %0++)

new gTimer[MAX_PLAYERS] = {0, ...};
new gTimerStatsUpdate[MAX_PLAYERS] = {0, ...};
new gTimerInVehicle[MAX_PLAYERS] = {0, ...};
new gTimerInVehicleUpdate[MAX_PLAYERS] = {0, ...};
new Float:gPreviousVehicleSpeed[MAX_PLAYERS] = {0.0, ...};

#include "modules/include"

main()
{
    // Main function - called when gamemode starts
}

public OnGameModeInit()
{
    DatabaseConnection();
    
    cef_subscribe("ui:exit", "uiOnExit");

    cef_subscribe("ui:createCharacter", "authOnCreateCharacter");

    cef_subscribe("ui:closeSkinMenu", "skinOnCloseMenu");
    cef_subscribe("ui:selectSkin", "skinOnSelectSkin");
    cef_subscribe("ui:applySkin", "skinOnApplySkin");

    cef_subscribe("ui:closeSkinMenuList", "skinOnCloseMenu");
    cef_subscribe("ui:buyOutfitList", "skinOnBuyOutfitList");
    cef_subscribe("ui:previewSkinList", "skinOnPreviewSkinList");
    cef_subscribe("ui:applySkinList", "skinOnApplySkinList");
    cef_subscribe("ui:updateSkinList", "skinOnpdateSkinList");
    cef_subscribe("ui:saveOutfitList", "skinOnSaveOutfitList");
    cef_subscribe("ui:deleteSkinList", "skinOnDeleteSkinList");
    return 1;
}

public OnPlayerConnect(playerid)
{
    SetSpawnInfo(playerid, NO_TEAM, 1, -2415.8567, -593.8060, 132.7245, 259.7749, 
        t_WEAPON:0, 0, t_WEAPON:0, 0, t_WEAPON:0, 0);
    SpawnPlayer(playerid);  
    SetPlayerVirtualWorld(playerid, playerid + 1);
    TogglePlayerControllable(playerid, false);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (pUser[playerid][pLogin] == 1)
    {
        authOnPlayerDisconnect(playerid);

        KillTimer(gTimer[playerid]);
        gTimer[playerid] = 0;

        KillTimer(gTimerInVehicle[playerid]);
        gTimerInVehicle[playerid] = 0;

        vehOnPlayerDisconnect(playerid);
    }
    return 1;
}
public OnCefInitialize(player_id, success) 
{
    if (success == 1) 
    {
        SendClientMessage(player_id, -1, "Cef interface initialized successfully.");
        cef_create_browser(player_id, INTERFACE_BROWSER_ID, INTERFACE_BROWSER_URL, false, false);
        return;
    }
    
    SendClientMessage(player_id, -1, "Ahh to bad you cannot see our new cool interface ...");
}

public OnCefBrowserCreated(player_id, browser_id, status_code)
{
    if(browser_id == INTERFACE_BROWSER_ID)
    {
        if(status_code == 200)
        {
            authCheckLauncherToken(player_id);
        }
        else
        {
            authKickPlayerWithMessage(player_id, "Failed to create browser");
        }
    }
}

forward uiOnExit(player_id);
public uiOnExit(player_id)
{
    cef_focus_browser(player_id, INTERFACE_BROWSER_ID, false);
    return 1;
}
