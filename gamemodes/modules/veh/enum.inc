enum pVeh {
    pVehID,
    pVehSpawnID,
    pVehModel,
    pVehColor1,
    pVehColor2,
    Float:pVehPosX,
    Float:pVehPosY,
    Float:pVehPosZ,
    Float:pVehPosA, 
    pVehPlate[32],
    VEHICLE_PANEL_STATUS:pVehPanels,
    VEHICLE_DOOR_STATUS:pVehDoors,
    VEHICLE_LIGHT_STATUS:pVehLights,
    VEHICLE_TIRE_STATUS:pVehTires,
    Float:pVehEngine,
    pVehLocked,
    Float:pVehFuel
}

new pVehData[MAX_VEHICLES][pVeh];

new Iterator:PlayerVehicles<MAX_PLAYERS, MAX_VEHICLES>;