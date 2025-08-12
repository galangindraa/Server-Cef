forward playerTimer(playerid);
public playerTimer(playerid)
{
    hbe_setHudData(playerid);
    hbe_updateEnergy(playerid);
    return 1;
}

forward playerTimerInVehicle(playerid);
public playerTimerInVehicle(playerid)
{
    new Float:speed = GetVehicleSpeed(GetPlayerVehicleID(playerid));
    speedometer_set(playerid, speed);
    return 1;
}
