stock skinHatList(playerid, modelid)
{
    if(modelid == -1) return 0;

    if(modelid == 19098)
    {
        SetPlayerAttachedObject(playerid, 1, 19098, 2, 0.165, 0.008, -0.001, -180.0, -180.0, 179.0, 1.0, 1.0, 1.0);
    }
    else if(modelid == 19137)
    {
        SetPlayerAttachedObject(playerid, 1, 19137, 2, 0.165, 0.008, -0.001, -180.0, -180.0, 179.0, 1.0, 1.0, 1.0);
    }

    return 1;
}