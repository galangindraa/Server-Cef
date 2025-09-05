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

#include <requests>

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
    new RequestsClient:clientSocket = RequestsClient("http://localhost:3001/");
    RequestJSON(clientSocket, "getitemlist", HTTP_METHOD_GET, "OnReceiveRequestData", .headers = RequestHeaders());
    printf("[init] Requests socket: %i, validity check: %i", _:clientSocket, IsValidRequestsClient(clientSocket));

}

public OnGameModeInit()
{

    DatabaseConnection();
    
    new qury[256];
    mysql_format(g_SQL, qury, sizeof(qury), "DELETE FROM `inventorydrop`");
    mysql_tquery(g_SQL, qury);

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

    cef_subscribe("inventory:swapItems", "invOnSwapItems");
    cef_subscribe("inventory:useItem", "invOnUseItem");
    cef_subscribe("inventory:giveItem", "invOnGiveItem");
    cef_subscribe("inventory:dropItem", "invOnDropItem");
    cef_subscribe("inventory:takeItem", "invOnTakeItem");
    return 1;
}

public OnPlayerConnect(playerid)
{
    PreloadAnimLibs(playerid);
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
            authOnTokenValid(player_id);
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

forward OnReceiveRequestData(Request:id, E_HTTP_STATUS:status, Node:node);
public OnReceiveRequestData(Request:id, E_HTTP_STATUS:status, Node:node)
{
    printf("[requests] Callback fired. id:%d status:%d", _:id, _:status);
    if (status != HTTP_STATUS_OK)
    {
        printf("[requests] Non-OK status: %d. Aborting parse.", _:status);
        return 1;
    }
    new Node:items;
    JsonGetObject(node, "items", items);
    new len;
    JsonArrayLength(items, len);
    printf("[requests] Items count: %d", len);
    printf("[requests] ---- Items Detail ----");

    for (new i = 0; i < len; i++)
    {
        if (i >= MAX_ITEM_DEFS)
        {
            printf("[requests] Reached MAX_ITEM_DEFS limit (%d), stopping.", MAX_ITEM_DEFS);
            break;
        }
        new Node:item;
        JsonArrayObject(items, i, item);

        new animJson[128];
        JsonStringify(item, animJson, sizeof(animJson));
        printf("[requests] Item %d: %s", i, animJson);


        new itemId;
        new name[64];
        new label[64];
        new Float:weight;

        JsonGetInt(item, "id", itemId);
        JsonGetString(item, "name", name, 64);
        JsonGetString(item, "label", label, 64);
        JsonGetFloat(item, "weight", weight);

        new Node:output;
        JsonGetObject(item, "use", output);
        new dict[64], lib[64];
        JsonGetString(output, "dict", dict, 64);
        JsonGetString(output, "lib", lib, 64);
        new Node:statusObj;
        JsonGetObject(output, "status", statusObj);
        new hunger, thirst;
        JsonGetInt(statusObj, "hunger", hunger);
        JsonGetInt(statusObj, "thirst", thirst);

        Items[i][itemID] = itemId;
        Items[i][itemName] = name;
        Items[i][itemWeight] = weight; 
        Items[i][useHunger] = hunger;
        Items[i][useThirst] = thirst;
        Items[i][useAnimDict] = dict;
        Items[i][useAnimLib] = lib;
        printf("[requests] #%d | id:%d | name:%s | label:%s | weight:%.2f | hunger:%d | thirst:%d | dict:%s | lib:%s", i + 1, itemId, name, label, weight, hunger, thirst, dict, lib);
    }
    printf("[requests] -----------------------");
    return 1;
}