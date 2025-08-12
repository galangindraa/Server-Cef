fnc:vehOnPlayerLoad(playerid)
{
    new rows = cache_num_rows();
    if(rows)
    {
        new vehicleid, spawnid;
        forex(i, rows)
        {
            vehicleid = Iter_FreeMulti(PlayerVehicles<playerid>);
            if(vehicleid == cellmin) continue;
            cache_get_value_name_int(i, "id", pVehData[vehicleid][pVehID]);
            cache_get_value_name_int(i, "model", pVehData[vehicleid][pVehModel]);
            cache_get_value_name_int(i, "color1", pVehData[vehicleid][pVehColor1]);
            cache_get_value_name_int(i, "color2", pVehData[vehicleid][pVehColor2]);
            cache_get_value_name_float(i, "posx", pVehData[vehicleid][pVehPosX]);
            cache_get_value_name_float(i, "posy", pVehData[vehicleid][pVehPosY]);
            cache_get_value_name_float(i, "posz", pVehData[vehicleid][pVehPosZ]);
            cache_get_value_name_float(i, "posa", pVehData[vehicleid][pVehPosA]);
            if((spawnid = CreateVehicle(pVehData[vehicleid][pVehModel], pVehData[vehicleid][pVehPosX], pVehData[vehicleid][pVehPosY], pVehData[vehicleid][pVehPosZ], pVehData[vehicleid][pVehPosA], pVehData[vehicleid][pVehColor1], pVehData[vehicleid][pVehColor2], -1, false)) != INVALID_VEHICLE_ID  )
            {
                pVehData[vehicleid][pVehSpawnID] = spawnid;
                printf("playerid: %d, vehid: %d", playerid, pVehData[vehicleid][pVehID]);   
                Iter_Add(PlayerVehicles<playerid>, vehicleid);
            }
        }
    }
    
}