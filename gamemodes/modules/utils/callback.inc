public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER) // Pemain menaiki kendaraan (Wajib Mobil) dan menjadi supir
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        cef_emit_event(playerid, "ui:showSpeedometer");
        foreach(new i : PlayerVehicles<playerid>)
        {
            if(vehicleid != pVehData[i][pVehSpawnID]) continue;
            pUserD[playerid][pdInVehicle] = i;
            gTimerInVehicle[playerid] = SetTimerEx("playerTimerInVehicle", 100, true, "id", playerid, i);
            gPreviousVehicleSpeed[playerid] = 0.0;
        }
    }
    return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(!ispassenger)
    {
        EngineVehicle(vehicleid, false);
    }
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if (pUserD[playerid][pSeatBelt] && IsDoorVehicle(vehicleid))
    {
        pUserD[playerid][pSeatBelt] = false;
    }
    
    if (gTimerInVehicle[playerid] != 0) 
    {
        KillTimer(gTimerInVehicle[playerid]);
        gTimerInVehicle[playerid] = 0;
        cef_emit_event(playerid, "ui:hideSpeedometer");   
        EngineVehicle(vehicleid, false);
    }
    gPreviousVehicleSpeed[playerid] = 0.0;
    
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

    if(key == 85)
    {
        new vehicleid = GetClosestVehicle(player);
        if(vehicleid != -1)
        {
            if(pVehData[vehicleid][pVehLocked] == 0)
            {
                PlayerPlaySound(player, 24600, 0.0, 0.0, 0.0);
                ShowNotification(player, "default", "Kendaraan", "Kendaraan telah terkunci", "error", 5000);
                LockVehicle(pVehData[vehicleid][pVehSpawnID], 1);
                pVehData[vehicleid][pVehLocked] = 1;
            }
            else
            {
                PlayerPlaySound(player, 24600, 0.0, 0.0, 0.0);
                ShowNotification(player, "default", "Kendaraan", "Kendaraan telah terbuka", "success", 5000);
                LockVehicle(pVehData[vehicleid][pVehSpawnID], 0);
                pVehData[vehicleid][pVehLocked] = 0;
            }
        }
    }
    if(key == 71)
    {
        if(IsPlayerInAnyVehicle(player) && GetPlayerState(player) == PLAYER_STATE_DRIVER)
		{
            new vehicleid = GetPlayerVehicleID(player);
            new i = pUserD[player][pdInVehicle];
            if(pVehData[i][pVehSpawnID] == vehicleid)
            {
                if(pVehData[i][pVehFuel] <= 0.0)
                {
                    ShowNotification(player, "default", "Kendaraan", "Bensin kendaraan telah habis", "error", 5000);
                    return 1;
                }

                if(!IsEngineVehicle(vehicleid))
                    return ShowNotification(player, "default", "Kendaraan", "Kendaraan tidak memiliki mesin", "error", 5000);
                if(GetEngineStatus(vehicleid))
                {
                    ShowNotification(player, "default", "Kendaraan", "Mesin kendaraan telah dimatikan", "error", 5000);   
                    EngineVehicle(vehicleid, false);
                }
                else
                {
                    EngineVehicle(vehicleid, true);
                    ShowNotification(player, "default", "Kendaraan", "Mesin kendaraan telah dinyalakan", "success", 5000);
                }
            }
        }
    }

    if(key == 66)
    {
        if(IsPlayerInAnyVehicle(player) && GetPlayerState(player) == PLAYER_STATE_DRIVER)
		{   
            new vehicleid = GetPlayerVehicleID(player);
            if(IsDoorVehicle(vehicleid))
            {
                pUserD[player][pSeatBelt] = !pUserD[player][pSeatBelt];
                    
                if(pUserD[player][pSeatBelt])
                {
                    ShowNotification(player, "default", "Kendaraan", "Sabuk pengaman telah dikencangkan", "success", 5000);
                }
                else
                {
                    ShowNotification(player, "default", "Kendaraan", "Sabuk pengaman telah dilepas", "error", 5000);
                }
                
                cef_emit_event(player, "ui:toggleSeatbelt", CEFSTR(pUserD[player][pSeatBelt]));
            }
        }
    }
        
    return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
    new Float:health;
    GetVehicleHealth(vehicleid, health);
    if(health <= 8000.0)
    {
        if(IsDoorVehicle(vehicleid))
        {
            if(gPreviousVehicleSpeed[playerid] > 60.0 && !pUserD[playerid][pSeatBelt])
            {
                new Float:hp, amount = RandomMinMax(1, 3);
                GetPlayerHealth(playerid, hp);
                SetPlayerHealth(playerid, hp-amount);
            }
        }
    }
    return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
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