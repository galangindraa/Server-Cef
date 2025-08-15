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

    gTimerInVehicleUpdate[playerid]++;
    if(gTimerInVehicleUpdate[playerid] >= 45)
    {
        foreach(new i : PlayerVehicles<playerid>)
        {
            if(IsValidVehicle(pVehData[i][pVehSpawnID]) && IsEngineVehicle(pVehData[i][pVehSpawnID]) && GetEngineStatus(pVehData[i][pVehSpawnID]))
            {
                if(pVehData[i][pVehFuel] > 0.0)
                {
                    pVehData[i][pVehFuel] -= RandomFloat(0.03, 0.25);
                    if(pVehData[i][pVehFuel] <= 0.0)
                    {
                        pVehData[i][pVehFuel] = 0.0;
                        EngineVehicle(pVehData[i][pVehSpawnID], false);
                        
                        if(GetVehicleDriver(i) != INVALID_PLAYER_ID)
                            GameTextForPlayer(GetVehicleDriver(i), "Bensin kendaraan telah habis!", 3000, 3);
                    }
                }
                printf("Fuel: %f", pVehData[i][pVehFuel]);
            }
            gTimerInVehicleUpdate[playerid] = 0;
        }
    }
    return 1;
}

forward playerTimerInVehicle(playerid, vehicleid);
public playerTimerInVehicle(playerid, vehicleid)
{
    new Float:speed = GetVehicleSpeed(pVehData[vehicleid][pVehSpawnID]);
    speedometer_set(playerid, vehicleid, speed);
    gPreviousVehicleSpeed[playerid] = speed;
    return 1;
}