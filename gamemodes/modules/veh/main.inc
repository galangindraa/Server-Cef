fnc:vehOnPlayerLoad(playerid)
{
    new rows = cache_num_rows();
    if(rows)
    {
        new vehicleid, spawnid;
        forex(i, rows)
        {
            vehicleid = Iter_Free(PlayerVehicles<playerid>);
            if(vehicleid == cellmin) continue;
            cache_get_value_name_int(i, "id", pVehData[playerid][vehicleid][pVehID]);
            cache_get_value_name_int(i, "model", pVehData[playerid][vehicleid][pVehModel]);
            cache_get_value_name_int(i, "color1", pVehData[playerid][vehicleid][pVehColor1]);
            cache_get_value_name_int(i, "color2", pVehData[playerid][vehicleid][pVehColor2]);
            cache_get_value_name_float(i, "posx", pVehData[playerid][vehicleid][pVehPosX]);
            cache_get_value_name_float(i, "posy", pVehData[playerid][vehicleid][pVehPosY]);
            cache_get_value_name_float(i, "posz", pVehData[playerid][vehicleid][pVehPosZ]);
            cache_get_value_name_float(i, "posa", pVehData[playerid][vehicleid][pVehPosA]);
            cache_get_value_name_float(i, "fuel", pVehData[playerid][vehicleid][pVehFuel]);
            cache_get_value_name(i, "plate", pVehData[playerid][vehicleid][pVehPlate], 32);
            cache_get_value_name_int(i, "panels", pVehData[playerid][vehicleid][pVehPanels]);
            cache_get_value_name_int(i, "doors", pVehData[playerid][vehicleid][pVehDoors]);
            cache_get_value_name_int(i, "lights", pVehData[playerid][vehicleid][pVehLights]);
            cache_get_value_name_int(i, "tires", pVehData[playerid][vehicleid][pVehTires]);
            cache_get_value_name_float(i, "engine", pVehData[playerid][vehicleid][pVehEngine]);
            cache_get_value_name_int(i, "locked", pVehData[playerid][vehicleid][pVehLocked]);
            if((spawnid = CreateVehicle(pVehData[playerid][vehicleid][pVehModel], pVehData[playerid][vehicleid][pVehPosX], pVehData[playerid][vehicleid][pVehPosY], pVehData[playerid][vehicleid][pVehPosZ], pVehData[playerid][vehicleid][pVehPosA], pVehData[playerid][vehicleid][pVehColor1], pVehData[playerid][vehicleid][pVehColor2], -1, false)) != INVALID_VEHICLE_ID  )
            {
                pVehData[playerid][vehicleid][pVehSpawnID] = spawnid;
                SetVehicleNumberPlate(spawnid, pVehData[playerid][vehicleid][pVehPlate]);
                UpdateVehicleDamageStatus(spawnid, pVehData[playerid][vehicleid][pVehPanels], pVehData[playerid][vehicleid][pVehDoors], pVehData[playerid][vehicleid][pVehLights], pVehData[playerid][vehicleid][pVehTires]);
                SetVehicleHealth(spawnid, pVehData[playerid][vehicleid][pVehEngine]);
                LockVehicle(spawnid, pVehData[playerid][vehicleid][pVehLocked]);
                Iter_Add(PlayerVehicles<playerid>, vehicleid);
            }
        }
    }
    
}

stock vehOnPlayerDisconnect(playerid)
{
    new Float:pPos[4];
    new VEHICLE_PANEL_STATUS:panels, VEHICLE_DOOR_STATUS:doors, VEHICLE_LIGHT_STATUS:lights, VEHICLE_TIRE_STATUS:tires;
    new Float:engine;
    new cQuery[2500];
    foreach(new i : PlayerVehicles<playerid>)
    {
        GetVehiclePos(pVehData[playerid][i][pVehSpawnID], pPos[0], pPos[1], pPos[2]);
        GetVehicleZAngle(pVehData[playerid][i][pVehSpawnID], pPos[3]);
        GetVehicleDamageStatus(pVehData[playerid][i][pVehSpawnID], panels, doors, lights, tires);
        GetVehicleHealth(pVehData[playerid][i][pVehSpawnID], engine);
        mysql_format(g_SQL, cQuery, sizeof(cQuery), 
            "UPDATE `krakterveh` SET \
            `color1` = %d, \
            `color2` = %d, \
            `posx` = %.2f, \
            `posy` = %.2f, \
            `posz` = %.2f, \
            `posa` = %.2f, \
            `fuel` = %.2f, \
            `plate` = '%s', \
            `panels` = %d, \
            `doors` = %d, \
            `lights` = %d, \
            `tires` = %d, \
            `engine` = %.2f, \
            `locked` = %d \
            WHERE `id` = %d",
            pVehData[playerid][i][pVehColor1],
            pVehData[playerid][i][pVehColor2],
            pPos[0],
            pPos[1],
            pPos[2],
            pPos[3],
            pVehData[playerid][i][pVehFuel],
            pVehData[playerid][i][pVehPlate],
            panels,
            doors,
            lights,
            tires,
            engine,
            pVehData[playerid][i][pVehLocked],
            pVehData[playerid][i][pVehID]
        );
        mysql_pquery(g_SQL, cQuery);
        DestroyVehicle(pVehData[playerid][i][pVehSpawnID]);
        Iter_Remove(PlayerVehicles<playerid>, i);
    }
}