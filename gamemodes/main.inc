#include <open.mp>
#include <PawnPlus>
#include <omp_cmd>
#include <a_mysql>

#undef MAX_PLAYERS
#define MAX_PLAYERS                     100
#define PP_SYNTAX_AWAIT


#include <sscanf2>
#include <samp_bcrypt>
#include <cef>

#define INTERFACE_BROWSER_ID 0xABCDE
#define INTERFACE_BROWSER_URL "http://localhost:3000"

#include "modules/include"

main()
{
}

public OnGameModeInit()
{
    DatabaseConnection();
    cef_subscribe("ui:createCharacter", "authOnCreateCharacter");
    cef_subscribe("ui:selectSkin", "skinOnSelectSkin");
    cef_subscribe("ui:closeSkinMenu", "skinOnCloseMenu");
    cef_subscribe("ui:applySkin", "skinOnApplySkin");
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

public OnPlayerConnect(playerid)
{
    authCheckLauncherToken(playerid);
    return 1;
}