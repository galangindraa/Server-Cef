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