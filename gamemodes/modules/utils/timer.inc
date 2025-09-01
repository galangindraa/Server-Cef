forward LoadingFirstUITimer(playerid);
public LoadingFirstUITimer(playerid)
{
    ShowNotification(playerid, "firstui", "Title", "All UI Loaded");
    cef_emit_event(playerid, "ui:showhud");
    pUser[playerid][pLogin] = 1;
    gTimer[playerid] = SetTimerEx("playerTimer", 1000, true, "i", playerid);
    
    TogglePlayerControllable(playerid, true);
    return 1;
}

forward playerTimer(playerid);
public playerTimer(playerid)
{
    hbe_setHudData(playerid);
    hbe_updateEnergy(playerid);

    if(++gTimerStatsUpdate[playerid] >= 60)
    {
        if(pUserD[playerid][pdHunger] <= 20.0 || pUserD[playerid][pdThirst] <= 20.0)
        {
            ShowNotification(playerid, "stats", "Stats", "Kamu terlalu lapar dan kehausan", "info");
            new Float:health;
            GetPlayerHealth(playerid, health);
            SetPlayerHealth(playerid, health - RandomFloat(2.0, 5.0));
        }

        if(pUserD[playerid][pdStrees] >= 60.0)
        {
            if(!pUserD[playerid][pDrunkEffect])
            {
                SetPlayerDrunkEffect(playerid, 4000);
            }
        }
        gTimerStatsUpdate[playerid] = 0;
    }

    if(++gTimerInVehicleUpdate[playerid] >= 45)
    {
        foreach(new i : PlayerVehicles<playerid>)
        {
            if(IsValidVehicle(pVehData[playerid][i][pVehSpawnID]) && IsEngineVehicle(pVehData[playerid][i][pVehSpawnID]) && GetEngineStatus(pVehData[playerid][i][pVehSpawnID]))
            {
                if(pVehData[playerid][i][pVehFuel] > 0.0)
                {
                    pVehData[playerid][i][pVehFuel] -= RandomFloat(0.03, 0.25);
                    if(pVehData[playerid][i][pVehFuel] <= 0.0)
                    {
                        pVehData[playerid][i][pVehFuel] = 0.0;
                        EngineVehicle(pVehData[playerid][i][pVehSpawnID], false);
                        
                        if(GetVehicleDriver(pVehData[playerid][i][pVehSpawnID]) != INVALID_PLAYER_ID)
                            ShowNotification(playerid, "default", "Vehicle", "Bensin kendaraan telah habis!", "error", 5000);
                    }
                }
            }
            gTimerInVehicleUpdate[playerid] = 0;
        }
    }
    return 1;
}

forward playerTimerInVehicle(playerid, vehicleid);
public playerTimerInVehicle(playerid, vehicleid)
{
    new Float:speed = GetVehicleSpeed(pVehData[playerid][vehicleid][pVehSpawnID]);
    speedometer_set(playerid, vehicleid, speed);
    gPreviousVehicleSpeed[playerid] = speed;
    return 1;
}