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

#define INTERFACE_BROWSER_ID             0xABCDE
#define INTERFACE_BROWSER_URL            "http://localhost:3000"

#define fnc:%0(%1)                      forward %0(%1); public %0(%1)
#define forex(%0,%1)                    for(new %0 = 0; %0 != %1; %0++)

new gTimer[MAX_PLAYERS] = {0, ...};
new gTimerInVehicle[MAX_PLAYERS] = {0, ...};

#include "modules/include"

main()
{
    // Main function - called when gamemode starts
}

public OnGameModeInit()
{
    DatabaseConnection();
    
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

public OnCefInitialize(player_id, success) 
{
    if (success == 1) 
    {
        SendClientMessage(player_id, -1, "Cef interface initialized successfully.");
        cef_create_browser(player_id, INTERFACE_BROWSER_ID, INTERFACE_BROWSER_URL, false, false);
        pUser[player_id][pLogin] = 1;
        gTimer[player_id] = SetTimerEx("playerTimer", 1000, true, "i", player_id);
        return;
    }
    
    SendClientMessage(player_id, -1, "Ahh to bad you cannot see our new cool interface ...");
}

public OnPlayerConnect(playerid)
{
    authCheckLauncherToken(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (pUser[playerid][pLogin] == 1)
    {
        authOnPlayerDisconnect(playerid);

        KillTimer(gTimer[playerid]);
        gTimer[playerid] = 0;
    }    
    return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER) // Pemain menaiki kendaraan (Wajib Mobil) dan menjadi supir
    {
        cef_emit_event(playerid, "ui:showSpeedometer");
        gTimerInVehicle[playerid] = SetTimerEx("playerTimerInVehicle", 100, true, "i", playerid);
    }
    return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if (gTimerInVehicle[playerid] != 0) 
    {
        KillTimer(gTimerInVehicle[playerid]);
        gTimerInVehicle[playerid] = 0;
        cef_emit_event(playerid, "ui:hideSpeedometer");
    }
    return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, 
    Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, 
    Float:fRotX, Float:fRotY, Float:fRotZ, 
    Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    if (pUser[playerid][pLogin] == 1)
    {
        if (response)
        {
            if (pSkin[playerid][pSkinEditType] != 0)
            {
                // Handle different attachment types
                switch (pSkin[playerid][pSkinEditType])
                {
                    case 1: // Hat
                    {
                        pSkin[playerid][pSkinHat] = modelid;
                        pSkin[playerid][pSkinHatPos][0] = fOffsetX;
                        pSkin[playerid][pSkinHatPos][1] = fOffsetY;
                        pSkin[playerid][pSkinHatPos][2] = fOffsetZ;
                        pSkin[playerid][pSkinHatPos][3] = fRotX;
                        pSkin[playerid][pSkinHatPos][4] = fRotY;
                        pSkin[playerid][pSkinHatPos][5] = fRotZ;
                        pSkin[playerid][pSkinHatPos][6] = fScaleX;
                        pSkin[playerid][pSkinHatPos][7] = fScaleY;
                        pSkin[playerid][pSkinHatPos][8] = fScaleZ;
                    }
                    case 2: // Glasses
                    {
                        pSkin[playerid][pSkinGlasses] = modelid;
                        pSkin[playerid][pSkinGlassesPos][0] = fOffsetX;
                        pSkin[playerid][pSkinGlassesPos][1] = fOffsetY;
                        pSkin[playerid][pSkinGlassesPos][2] = fOffsetZ;
                        pSkin[playerid][pSkinGlassesPos][3] = fRotX;
                        pSkin[playerid][pSkinGlassesPos][4] = fRotY;
                        pSkin[playerid][pSkinGlassesPos][5] = fRotZ;
                        pSkin[playerid][pSkinGlassesPos][6] = fScaleX;
                        pSkin[playerid][pSkinGlassesPos][7] = fScaleY;
                        pSkin[playerid][pSkinGlassesPos][8] = fScaleZ;
                    }
                    case 3: // Mask
                    {
                        pSkin[playerid][pSkinMask] = modelid;
                        pSkin[playerid][pSkinMaskPos][0] = fOffsetX;
                        pSkin[playerid][pSkinMaskPos][1] = fOffsetY;
                        pSkin[playerid][pSkinMaskPos][2] = fOffsetZ;
                        pSkin[playerid][pSkinMaskPos][3] = fRotX;
                        pSkin[playerid][pSkinMaskPos][4] = fRotY;
                        pSkin[playerid][pSkinMaskPos][5] = fRotZ;
                        pSkin[playerid][pSkinMaskPos][6] = fScaleX;
                        pSkin[playerid][pSkinMaskPos][7] = fScaleY;
                        pSkin[playerid][pSkinMaskPos][8] = fScaleZ;
                    }
                    case 4: // Backpack
                    {
                        pSkin[playerid][pSkinBackpack] = modelid;
                        pSkin[playerid][pSkinBackpackPos][0] = fOffsetX;
                        pSkin[playerid][pSkinBackpackPos][1] = fOffsetY;
                        pSkin[playerid][pSkinBackpackPos][2] = fOffsetZ;
                        pSkin[playerid][pSkinBackpackPos][3] = fRotX;
                        pSkin[playerid][pSkinBackpackPos][4] = fRotY;
                        pSkin[playerid][pSkinBackpackPos][5] = fRotZ;
                        pSkin[playerid][pSkinBackpackPos][6] = fScaleX;
                        pSkin[playerid][pSkinBackpackPos][7] = fScaleY;
                        pSkin[playerid][pSkinBackpackPos][8] = fScaleZ;
                    }
                }
                
                // Update database with new skin data
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
            }
        }
        else
        {
            // Restore previous attachment if editing was cancelled
            switch (pSkin[playerid][pSkinEditType])
            {
                case 1: // Hat
                {
                    SetPlayerAttachedObject(playerid, 1, pSkin[playerid][pSkinHat], 2, 
                        pSkin[playerid][pSkinHatPos][0], pSkin[playerid][pSkinHatPos][1], pSkin[playerid][pSkinHatPos][2], 
                        pSkin[playerid][pSkinHatPos][3], pSkin[playerid][pSkinHatPos][4], pSkin[playerid][pSkinHatPos][5], 
                        pSkin[playerid][pSkinHatPos][6], pSkin[playerid][pSkinHatPos][7], pSkin[playerid][pSkinHatPos][8]);
                }
                case 2: // Glasses
                {
                    SetPlayerAttachedObject(playerid, 2, pSkin[playerid][pSkinGlasses], 2, 
                        pSkin[playerid][pSkinGlassesPos][0], pSkin[playerid][pSkinGlassesPos][1], pSkin[playerid][pSkinGlassesPos][2], 
                        pSkin[playerid][pSkinGlassesPos][3], pSkin[playerid][pSkinGlassesPos][4], pSkin[playerid][pSkinGlassesPos][5], 
                        pSkin[playerid][pSkinGlassesPos][6], pSkin[playerid][pSkinGlassesPos][7], pSkin[playerid][pSkinGlassesPos][8]);
                }
                case 3: // Mask
                {
                    SetPlayerAttachedObject(playerid, 3, pSkin[playerid][pSkinMask], 2, 
                        pSkin[playerid][pSkinMaskPos][0], pSkin[playerid][pSkinMaskPos][1], pSkin[playerid][pSkinMaskPos][2], 
                        pSkin[playerid][pSkinMaskPos][3], pSkin[playerid][pSkinMaskPos][4], pSkin[playerid][pSkinMaskPos][5], 
                        pSkin[playerid][pSkinMaskPos][6], pSkin[playerid][pSkinMaskPos][7], pSkin[playerid][pSkinMaskPos][8]);
                }
                case 4: // Backpack
                {
                    SetPlayerAttachedObject(playerid, 4, pSkin[playerid][pSkinBackpack], 2, 
                        pSkin[playerid][pSkinBackpackPos][0], pSkin[playerid][pSkinBackpackPos][1], pSkin[playerid][pSkinBackpackPos][2], 
                        pSkin[playerid][pSkinBackpackPos][3], pSkin[playerid][pSkinBackpackPos][4], pSkin[playerid][pSkinBackpackPos][5], 
                        pSkin[playerid][pSkinBackpackPos][6], pSkin[playerid][pSkinBackpackPos][7], pSkin[playerid][pSkinBackpackPos][8]);
                }
            }
        }
    }
    return 1;
}

public void:OnPlayerKeyDown(player, key)
{
	new buffer[64];
	format(buffer, sizeof(buffer), "KeyDown: %d", key);
	SendClientMessage(player, -1, buffer);
}
public void:OnPlayerKeyUp(player, key)
{
	new buffer[64];
	format(buffer, sizeof(buffer), "KeyUp: %d", key);
	SendClientMessage(player, -1, buffer);
}