enum p_data {
    pLogin,
    PId,
    pIdentifier[191],
    pName[MAX_PLAYER_NAME + 1],
    pActive,
    pAdmin,
    pAdminPoint,
} 

new pUser[MAX_PLAYERS][p_data]; // UCP data for each player

enum pd_data {
    pdMoney,
    pdMoneyBank,
    pdFirstName[16],
    pdLastName[16],
    pdGender[7],
    pdBirtDay[64],
    pdAge,
    Float:pdPos[3],
    pdVirtualID,
    pdWorldID,

    Float:pdHealth,
    Float:pdArmour,
    pdHunger,
    pdThirst,
    pdStrees,

    //not saved
    pdSkinIDLast,
}

new pUserD[MAX_PLAYERS][pd_data];