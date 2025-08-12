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
            cache_get_value_name_int(i, "id", pVehData[vehicleid][pVehID]);
            cache_get_value_name_int(i, "model", pVehData[vehicleid][pVehModel]);
            cache_get_value_name_int(i, "color1", pVehData[vehicleid][pVehColor1]);
            cache_get_value_name_int(i, "color2", pVehData[vehicleid][pVehColor2]);
            cache_get_value_name_float(i, "posx", pVehData[vehicleid][pVehPosX]);
            cache_get_value_name_float(i, "posy", pVehData[vehicleid][pVehPosY]);
            cache_get_value_name_float(i, "posz", pVehData[vehicleid][pVehPosZ]);
            cache_get_value_name_float(i, "posa", pVehData[vehicleid][pVehPosA]);
            cache_get_value_name(i, "plate", pVehData[vehicleid][pVehPlate], 32);
            cache_get_value_name_int(i, "panels", pVehData[vehicleid][pVehPanels]);
            cache_get_value_name_int(i, "doors", pVehData[vehicleid][pVehDoors]);
            cache_get_value_name_int(i, "lights", pVehData[vehicleid][pVehLights]);
            cache_get_value_name_int(i, "tires", pVehData[vehicleid][pVehTires]);
            cache_get_value_name_float(i, "engine", pVehData[vehicleid][pVehEngine]);
            if((spawnid = CreateVehicle(pVehData[vehicleid][pVehModel], pVehData[vehicleid][pVehPosX], pVehData[vehicleid][pVehPosY], pVehData[vehicleid][pVehPosZ], pVehData[vehicleid][pVehPosA], pVehData[vehicleid][pVehColor1], pVehData[vehicleid][pVehColor2], -1, false)) != INVALID_VEHICLE_ID  )
            {
                pVehData[vehicleid][pVehSpawnID] = spawnid;
                SetVehicleNumberPlate(spawnid, pVehData[vehicleid][pVehPlate]);
                UpdateVehicleDamageStatus(spawnid, pVehData[vehicleid][pVehPanels], pVehData[vehicleid][pVehDoors], pVehData[vehicleid][pVehLights], pVehData[vehicleid][pVehTires]);
                SetVehicleHealth(spawnid, pVehData[vehicleid][pVehEngine]);
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
        GetVehiclePos(pVehData[i][pVehSpawnID], pPos[0], pPos[1], pPos[2]);
        GetVehicleZAngle(pVehData[i][pVehSpawnID], pPos[3]);
        GetVehicleDamageStatus(pVehData[i][pVehSpawnID], panels, doors, lights, tires);
        GetVehicleHealth(pVehData[i][pVehSpawnID], engine);
        mysql_format(g_SQL, cQuery, sizeof(cQuery), 
            "UPDATE `krakterveh` SET \
            `color1` = %d, \
            `color2` = %d, \
            `posx` = %.2f, \
            `posy` = %.2f, \
            `posz` = %.2f, \
            `posa` = %.2f, \
            `plate` = '%s', \
            `panels` = %d, \
            `doors` = %d, \
            `lights` = %d, \
            `tires` = %d, \
            `engine` = %.2f \
            WHERE `id` = %d",
            pVehData[i][pVehColor1],
            pVehData[i][pVehColor2],
            pPos[0],
            pPos[1],
            pPos[2],
            pPos[3],
            pVehData[i][pVehPlate],
            panels,
            doors,
            lights,
            tires,
            engine,
            pVehData[i][pVehID]
        );
        mysql_pquery(g_SQL, cQuery);
        DestroyVehicle(pVehData[i][pVehSpawnID]);
        Iter_Remove(PlayerVehicles<playerid>, i);
    }
}