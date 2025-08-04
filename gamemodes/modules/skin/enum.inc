enum e_skin
{
    pSqlID,          // Player's skin ID
    pSkinName[32],    // Player's skin name
    pSkinActive,      // Player's skin active status (0 = inactive, 1 = active)
    pSkinID,
    pSkinHat,
    Float:pSkinHatPos[9],
    pSkinGlasses,
    Float:pSkinGlassesPos[9],
    pSkinMask,
    Float:pSkinMaskPos[9],
    pSkinBackpack,
    Float:pSkinBackpackPos[9],
}

new pSkin[MAX_PLAYERS][e_skin]; // Array to hold skin data for each player