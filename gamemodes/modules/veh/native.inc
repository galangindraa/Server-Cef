stock Float:GetVehicleSpeed(vehicleid) //credits to whoever made this.
{
	new Float:x,Float:y,Float:z;
	GetVehicleVelocity(vehicleid,x,y,z);
	return floatmul(VectorSize(x,y,z),170.0);
}

stock speedometer_set(playerid)
{
    new jsonFormat[256];
    
    format(jsonFormat, sizeof(jsonFormat),
        "{\"speed\":%d,\"fuel\":%d}",
        floatround(GetVehicleSpeed(GetPlayerVehicleID(playerid))),
        100
    );
    cef_emit_event(playerid, "ui:updateSpedoData", CEFSTR(jsonFormat));
}
