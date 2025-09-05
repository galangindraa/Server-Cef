stock calculateAgeFromDate(const date[])
{
    new year, month, day;
    sscanf(date, "p<->dd", year, month, day);
    
    new currentYear, currentMonth, currentDay;
    getdate(currentYear, currentMonth, currentDay);
    
    new age = currentYear - year;
    
    if (currentMonth < month || (currentMonth == month && currentDay < day)) {
        age--;
    }
    
    return age;
}

stock SetRpName(playerid, const firstname[], const lastname[])
{
    new name[MAX_PLAYER_NAME];
    format(name, sizeof(name), "%s_%s", firstname, lastname);
    SetPlayerName(playerid, name);
}

stock GetRpName(playerid)
{
    new name[MAX_PLAYER_NAME];
    format(name, sizeof(name), "%s_%s", pUserD[playerid][pdFirstName], pUserD[playerid][pdLastName]);
    return name;
}

stock SetHealth(playerid, amount)
{
    new Float:health = float(amount);
    SetPlayerHealth(playerid, health);
    pUserD[playerid][pdHealth] = amount;
}

stock SetPlayerDrunkEffect(playerid, level)
{
    if(level == 0)
    {
        pUserD[playerid][pDrunkEffect] = false;
        SetPlayerDrunkLevel(playerid, 0);
        return 1;
    }

    SetPlayerDrunkLevel(playerid, level);
    pUserD[playerid][pDrunkEffect] = true;
    return 1;
}



stock Is3DPointInRangeOfPoint(Float:x, Float:y, Float:z, Float:x2, Float:y2, Float:z2, Float:range)
{
    x2 -= x;
    y2 -= y;
    z2 -= z;
    return ((x2 * x2) + (y2 * y2) + (z2 * z2)) < (range * range);
}