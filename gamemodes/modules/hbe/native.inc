stock GetHealth(playerid)
{
    new Float:health;
    GetPlayerHealth(playerid, health);
    return floatround(health);
}

stock GetArmor(playerid)
{
    new Float:armour;
    GetPlayerArmour(playerid, armour);
    return floatround(armour);
}