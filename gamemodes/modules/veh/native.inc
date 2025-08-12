stock Float:GetVehicleSpeed(vehicleid) //credits to whoever made this.
{
	new Float:x,Float:y,Float:z;
	GetVehicleVelocity(vehicleid,x,y,z);
	return floatmul(VectorSize(x,y,z),170.0);
}

stock CreatePlayerVehicle(playerid, modelid, color1, color2, respawn_delay, bool:add_bonus)
{
    new newid = Iter_Free(PlayerVehicles<playerid>);
    if(newid != cellmin) {
        new Float:x, Float:y, Float:z, Float:a;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        new vehicleid = CreateVehicle(modelid, x, y, z, a, color1, color2, respawn_delay, add_bonus);
        if (vehicleid != INVALID_VEHICLE_ID) {
            
            new VEHICLE_PANEL_STATUS:panels, VEHICLE_DOOR_STATUS:doors, VEHICLE_LIGHT_STATUS:lights, VEHICLE_TIRE_STATUS:tires;
            new Float:engine;
            GetVehicleHealth(vehicleid, engine);
            GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
            SetVehicleNumberPlate(vehicleid, "NONE");
            pVehData[newid][pVehModel] = modelid;
            pVehData[newid][pVehColor1] = color1;
            pVehData[newid][pVehColor2] = color2;
            pVehData[newid][pVehPosX] = x;
            pVehData[newid][pVehPosY] = y;
            pVehData[newid][pVehPosZ] = z;
            pVehData[newid][pVehPosA] = a;
            pVehData[newid][pVehPanels] = panels;
            pVehData[newid][pVehDoors] = doors;
            pVehData[newid][pVehLights] = lights;
            pVehData[newid][pVehTires] = tires;
            pVehData[newid][pVehEngine] = engine;
            strcpy(pVehData[newid][pVehPlate], "NONE", 32);
            pVehData[newid][pVehSpawnID] = vehicleid;
            new query[562];
            mysql_format(g_SQL, query, sizeof(query), "INSERT INTO `krakterveh` (`identifier`, `model`, `color1`, `color2`, `posx`, `posy`, `posz`, `posa`, `plate`, `panels`, `doors`, `lights`, `tires`, `engine`) VALUES ('%s', %d, %d, %d, %.2f, %.2f, %.2f, %.2f, 'NONE', %d, %d, %d, %d, %.2f)", pUser[playerid][pIdentifier], modelid, color1, color2, x, y, z, a, panels, doors, lights, tires, engine);
            mysql_tquery(g_SQL, query, "OnPlayerVehicleSpawn", "dd", playerid, newid);
            Iter_Add(PlayerVehicles<playerid>, newid);
            return 1;
        }
    }
    return 0;
}

forward OnPlayerVehicleSpawn(playerid, newid);
public OnPlayerVehicleSpawn(playerid, newid) {
    if (cache_affected_rows())
    {
        pVehData[newid][pVehID] = cache_insert_id();
    }
    else 
    {
        DestroyVehicle(pVehData[newid][pVehSpawnID]);
        Iter_Remove(PlayerVehicles<playerid>, newid);
    }
}



