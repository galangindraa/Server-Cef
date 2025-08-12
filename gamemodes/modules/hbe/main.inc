stock hbe_updateEnergy(playerid)
{
    new animidx = GetPlayerAnimationIndex(playerid),
        KEY:k,
        ud,
        lr,
        Float:adjustment,
        Float:adjustment1;    

    GetPlayerKeys(playerid, k, ud, lr);

    if(animidx == 43) {
        adjustment = (0.06 * 0.4);
        adjustment1 = (0.06 * 0.3); // Sitting
    } else if(animidx == 1159) {
        adjustment = (0.06 * 0.7);
        adjustment1 = (0.06 * 0.7); // Crouching
    } else if(animidx == 1195) {
        adjustment = (0.06 * 1.9);
        adjustment1 = (0.06 * 1.5); // Jumping
    } else if(animidx == 1231) {
        if(k & KEY_WALK) {
            adjustment = (0.06 * 0.08);
            adjustment1 = (0.06 * 0.07); // Walking
        } else if(k & KEY_SPRINT) {
            adjustment = (0.06 * 1.5);
            adjustment1 = (0.06 * 1.2); // Sprinting
        } else if(k & KEY_JUMP) {
            adjustment = (0.06 * 1.8);
            adjustment1 = (0.06 * 1.7); // Jumping
        } else {
            adjustment = (0.06 * 0.8);
            adjustment1 = (0.06 * 0.4); // Jogging
        }
    } else {
        adjustment = 0.05;
        adjustment1 = 0.04;
    }

    adjustment *= 0.2;
    adjustment1 *= 0.2;

    if(pUserD[playerid][pdHunger] > 0) {
       pUserD[playerid][pdHunger] -= adjustment;
    }
    if(pUserD[playerid][pdThirst] > 0){ 
        pUserD[playerid][pdThirst] -= adjustment1;
    }
}

stock hbe_setHudData(playerid)
{
    new jsonFormat[256];
    
    format(jsonFormat, sizeof(jsonFormat),
        "{\"health\":%d,\"armor\":%d,\"hunger\":%d,\"thirst\":%d,\"stress\":%d}",
        GetHealth(playerid),
        GetArmor(playerid),
        floatround(pUserD[playerid][pdHunger]),
        floatround(pUserD[playerid][pdThirst]),
        floatround(pUserD[playerid][pdStrees])
    );
    cef_emit_event(playerid, "ui:hudsetdata", CEFSTR(jsonFormat));
}

stock speedometer_set(playerid, Float:speed)
{
    new jsonFormat[256];
    
    format(jsonFormat, sizeof(jsonFormat),
        "{\"speed\":%d,\"fuel\":%d}",
        floatround(speed),
        100
    );
    cef_emit_event(playerid, "ui:updateSpedoData", CEFSTR(jsonFormat));
}