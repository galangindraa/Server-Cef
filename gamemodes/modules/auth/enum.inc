enum p_data {
    PId,
    pIdentifier[191],
    pName[MAX_PLAYER_NAME + 1],
    pActive,
} 

new pUser[MAX_PLAYERS][p_data]; // UCP data for each player

enum pd_data {
    pdFirstName[16],
    pdLastName[16],
    pdGender[7],
    pdBirtDay[64],
    pdAge,
    Float:pdPos[3],
    pdVirtualID,
    pdWorldID,

    pdSkinAtive,

    //not saved
    pdSkinIDLast,
}

new pUserD[MAX_PLAYERS][pd_data];