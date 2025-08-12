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
    speedometer_set(playerid);
    printf("playerTimerInVehicle");
    return 1;
}
