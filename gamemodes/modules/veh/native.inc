stock Float:GetVehicleSpeed(vehicleid)
{
	new Float:x,Float:y,Float:z;
	GetVehicleVelocity(vehicleid,x,y,z);
	return floatmul(VectorSize(x,y,z),170.0);
}

stock IsDoorVehicle(vehicleid)
{
	switch (GetVehicleModel(vehicleid))
	{
		case 400..424, 426..429, 431..440, 442..445, 451, 455, 456, 458, 459, 466, 467, 470, 474, 475:
		    return 1;

		case 477..480, 482, 483, 486, 489, 490..492, 494..496, 498..500, 502..508, 514..518, 524..529, 533..536:
		    return 1;

		case 540..547, 549..552, 554..562, 565..568, 573, 575, 576, 578..580, 582, 585, 587..589, 596..605, 609:
			return 1;
	}
	return 0;
}


stock LockVehicle(vehicleid, status)
{
	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	SetVehicleParamsEx(vehicleid,engine,lights,alarm,status,bonnet,boot,objective);
}

stock IsEngineVehicle(vehicleid)
{
	new const g_aEngineStatus[] = {
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
	    1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1,
	    1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0
	};
    new modelid = GetVehicleModel(vehicleid);

    if (modelid < 400 || modelid > 611)
        return 0;

    return (g_aEngineStatus[modelid - 400]);
}

stock GetEngineStatus(vehicleid)
{
	static
	engine,
	lights,
	alarm,
	doors,
	bonnet,
	boot,
	objective;

	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	if(engine != 1)
		return 0;

	return 1;
}


stock EngineVehicle(vehicleid, bool:status)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
    SetVehicleParamsEx(vehicleid,status,lights,alarm,doors,bonnet,boot,objective);
}

stock GetClosestVehicle(playerid)
{
    new Float:X, Float:Y, Float:Z;
    GetPlayerPos(playerid, X, Y, Z);
    
    foreach(new i : PlayerVehicles<playerid>)
    {
        new Float:distance = GetVehicleDistanceFromPoint(pVehData[playerid][i][pVehSpawnID], X, Y, Z);
        if(distance < 8.0 && distance < 99999.0)
        {
            return i;
        }
    }
    
    return -1;
}

stock CreatePlayerVehicle(playerid, modelid, color1, color2, respawn_delay, bool:add_bonus)
{
    new newid = Iter_Free(PlayerVehicles<playerid>);
    if(newid != cellmin) {
        if(newid >= pUserD[playerid][pdMaxVeh]) {
            return 0;
        }
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
            pVehData[playerid][newid][pVehModel] = modelid;
            pVehData[playerid][newid][pVehColor1] = color1;
            pVehData[playerid][newid][pVehColor2] = color2;
            pVehData[playerid][newid][pVehPosX] = x;
            pVehData[playerid][newid][pVehPosY] = y;
            pVehData[playerid][newid][pVehPosZ] = z;
            pVehData[playerid][newid][pVehPosA] = a;
            pVehData[playerid][newid][pVehPanels] = panels;
            pVehData[playerid][newid][pVehDoors] = doors;
            pVehData[playerid][newid][pVehLights] = lights;
            pVehData[playerid][newid][pVehTires] = tires;
            pVehData[playerid][newid][pVehEngine] = engine;
            strcpy(pVehData[playerid][newid][pVehPlate], "NONE", 32);
            pVehData[playerid][newid][pVehSpawnID] = vehicleid;
            pVehData[playerid][newid][pVehLocked] = 1;
            pVehData[playerid][newid][pVehFuel] = 100.0;
            LockVehicle(vehicleid, 1);
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
        pVehData[playerid][newid][pVehID] = cache_insert_id();
    }
    else 
    {
        DestroyVehicle(pVehData[playerid][newid][pVehSpawnID]);
        Iter_Remove(PlayerVehicles<playerid>, newid);
    }
}