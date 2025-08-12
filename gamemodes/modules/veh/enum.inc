enum pVeh {
    pVehID,
    pVehSpawnID,
    pVehModel,
    pVehColor1,
    pVehColor2,
    Float:pVehPosX,
    Float:pVehPosY,
    Float:pVehPosZ,
    Float:pVehPosA
}

new pVehData[MAX_VEHICLES][pVeh];

new Iterator:PlayerVehicles<MAX_PLAYERS, MAX_VEHICLES>;