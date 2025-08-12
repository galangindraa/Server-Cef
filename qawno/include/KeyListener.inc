// Ported to Pawn.RakNet by edgy
#if defined _keylistener_included
	#endinput
#endif
#define _keylistener_included

#tryinclude <open.mp>

#if !defined _INC_open_mp
	#include <a_samp>
#endif

#include <Pawn.RakNet>

#define KL_CURRENT_VERSION "1.1.2-pr"

forward void:OnPlayerKeyDown(player, key);
forward void:OnPlayerKeyUp(player, key);

static bool:s_Keys[MAX_PLAYERS + 1][256 char];

const KL_PACKET_KEY_DOWN = 244;
const KL_PACKET_KEY_UP = 245;

IPacket:KL_PACKET_KEY_DOWN(playerid, BitStream:bs)
{
    new key;

    BS_ReadValue(bs,
        PR_IGNORE_BITS, 8,
        PR_UINT8, key
    );

    if (!s_Keys[playerid]{key})
    {
        CallRemoteFunction("OnPlayerKeyDown", "ii", playerid, key);
        s_Keys[playerid]{key} = true;
    }

    return 1;
}

IPacket:KL_PACKET_KEY_UP(playerid, BitStream:bs)
{
    new key;

    BS_ReadValue(bs,
        PR_IGNORE_BITS, 8,
        PR_UINT8, key
    );

    if (s_Keys[playerid]{key})
    {
        CallRemoteFunction("OnPlayerKeyUp", "ii", playerid, key);
        s_Keys[playerid]{key} = false;
    }

    return 1;
}

static stock KL_Init()
{
    return printf("KeyListener v%s by MOR & edgy loaded", KL_CURRENT_VERSION);
}

#if defined FILTERSCRIPT
    public OnFilterScriptInit()
    {
        KL_Init();
        
        #if defined KL_OnFilterScriptInit
            return KL_OnFilterScriptInit();
        #else
            return 1;
        #endif
    }
    #if defined _ALS_OnFilterScriptInit
        #undef OnFilterScriptInit
    #else
        #define _ALS_OnFilterScriptInit
    #endif
    #if defined KL_OnFilterScriptInit
        forward KL_OnFilterScriptInit();
    #endif
#else
    public OnGameModeInit()
    {
        KL_Init();
        
        #if defined KL_OnGameModeInit
            return KL_OnGameModeInit();
        #else
            return 1;
        #endif
    }
    #if defined KL_OnGameModeInit
        forward KL_OnGameModeInit();
    #endif
    #if defined _ALS_OnGameModeInit
        #undef OnGameModeInit
    #else
        #define _ALS_OnGameModeInit
    #endif
    #define OnGameModeInit KL_OnGameModeInit
#endif

public OnPlayerDisconnect(playerid, reason)
{
    s_Keys[playerid] = s_Keys[MAX_PLAYERS];
    
    #if defined KL_OnPlayerDisconnect
        return KL_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined KL_OnPlayerDisconnect
    forward KL_OnPlayerDisconnect(playerid, reason);
#endif
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect KL_OnPlayerDisconnect
