// native OnCommandRejected(playerid, const cmdtext[], code);
// native OnCommandReceived(playerid, const cmdtext[]);
// native OnCommandPerformed(playerid, const cmdtext[]);

#if !defined _PawnPlus_included
    #error This module requires PawnPlus
#endif

#if !defined MAX_COMMAND_LENGTH
    #define MAX_COMMAND_LENGTH 20
#endif

#if !defined MAX_COMMAND_PARAM_LENGTH
    #define MAX_COMMAND_PARAM_LENGTH 128
#endif

#define CMD:%0(%1) \
    forward cmd_%0(%1); public cmd_%0(%1)

static PlayerPermissions[MAX_PLAYERS];
static Map:CommandAlternatives;
static Map:CommandPermissions;
static Map:DisabledCommands;
static commandrejected_idx;
static commandreceived_idx;
static commandperformed_idx;

#if defined COMMAND_DELAY
static Map:DelayDisabledCommandList;
static bool:PlayerCommandDelay[MAX_PLAYERS];
static bool:PlayerCommandDelayDisabled[MAX_PLAYERS];
#endif

enum
{
    COMMAND_INVALID = 1,
    COMMAND_DENIED = 2,
    COMMAND_DISABLED = 4,
    COMMAND_DELAYED = 8,
}

public OnGameModeInit()
{
    CommandAlternatives = map_new();
    CommandPermissions = map_new();
    DisabledCommands = map_new();
    commandrejected_idx = funcidx(#OnCommandRejected);
    commandreceived_idx = funcidx(#OnCommandReceived);
    commandperformed_idx = funcidx(#OnCommandPerformed);

    #if defined COMMAND_DELAY
    DelayDisabledCommandList = map_new();
    #endif

	#if defined cmds_OnGameModeInit
        return cmds_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined cmds_OnGameModeInit
    forward cmds_OnGameModeInit();
#endif
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit cmds_OnGameModeInit

public OnPlayerConnect(playerid)
{
    PlayerPermissions[playerid] = 0;

    #if defined COMMAND_DELAY
    PlayerCommandDelay[playerid] = false;
    PlayerCommandDelayDisabled[playerid] = false;
    #endif

	#if defined cmds_OnPlayerConnect
        return cmds_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined cmds_OnPlayerConnect
    forward cmds_OnPlayerConnect(playerid);
#endif
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect cmds_OnPlayerConnect

public OnGameModeExit()
{
    map_delete(CommandAlternatives);
    map_delete(CommandPermissions);
    map_delete(DisabledCommands);
    commandrejected_idx = -1;
    commandreceived_idx = -1;
    commandperformed_idx = -1;

    #if defined COMMAND_DELAY
    map_delete(DelayDisabledCommandList);
    #endif

	#if defined cmds_OnGameModeExit
        return cmds_OnGameModeExit();
    #else
        return 1;
    #endif
}
#if defined cmds_OnGameModeExit
    forward cmds_OnGameModeExit();
#endif
#if defined _ALS_OnGameModeExit
    #undef OnGameModeExit
#else
    #define _ALS_OnGameModeExit
#endif
#define OnGameModeExit cmds_OnGameModeExit

stock SanitizeCommand(const command[], callback[], maxlen = sizeof callback, bool:allow_alternative = true)
{
    format(callback, maxlen, "cmd_%s", command);
    for(new i = 4, j = strlen(callback); i < j; i++)
	{
		switch(callback[i])
		{
			case 'a'..'z': continue;
			case '0'..'9': continue;
			case 'A'..'Z':
            {
                callback[i] = tolower(callback[i]);
            }
			default: return 0;
		}
	}
    if(funcidx(callback) == -1)
    {
        if(!allow_alternative || !map_has_str_key(CommandAlternatives, callback))
            return 0;
        map_str_get_str(CommandAlternatives, callback, callback, maxlen);
    }
    return 1;
}

stock ExecuteCommand(playerid, const cmdtext[])
{
    new callback[MAX_COMMAND_LENGTH + 4], command[MAX_COMMAND_LENGTH], params[MAX_COMMAND_PARAM_LENGTH],
        spacepos = strfind(cmdtext, " "),
        len = strlen(cmdtext);
    if(spacepos == -1)
        strmid(command, cmdtext, 1, len, sizeof command);
    else
    {
        strmid(command, cmdtext, 1, spacepos, sizeof command);
        if(len > spacepos)
            strmid(params, cmdtext, spacepos + 1, len, sizeof params);
    }

    if(SanitizeCommand(command, callback, sizeof callback))
        return CallLocalFunction(callback, "is", playerid, params);
    return 0;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    // Disables command for NPC
    #if !defined COMMANDS_ALLOW_NPC
    if(IsPlayerNPC(playerid)) return 1;
    #endif

    new len = strlen(cmdtext);
    if(len == 1 || cmdtext[0] != '/') // For handling the '/' command
    {
        if(commandrejected_idx != -1)
            return CallLocalFunction(#OnCommandRejected, "isi", playerid, cmdtext, COMMAND_INVALID);
        else return 0;
    }

    #if defined COMMAND_DELAY
    if(PlayerCommandDelay[playerid])
    {
        if(commandrejected_idx != -1)
            return CallLocalFunction(#OnCommandRejected, "isi", playerid, cmdtext, COMMAND_DELAYED);
        else return 0;
    }
    #endif

    new command[MAX_COMMAND_LENGTH], params[MAX_COMMAND_PARAM_LENGTH],
        spacepos = strfind(cmdtext, " ");
    if(spacepos == -1)
        strmid(command, cmdtext, 1, len, sizeof command);
    else
    {
        strmid(command, cmdtext, 1, spacepos, sizeof command);
        if(len > spacepos)
            strmid(params, cmdtext, spacepos + 1, len, sizeof params);
    }
    
    // Check for invalid characters and convert to lowercase
    for(new i = 0, j = strlen(command); i < j; i++)
	{
		switch(command[i])
		{
			case 'a'..'z': continue;
			case '0'..'9': continue;
			case 'A'..'Z':
            {
                command[i] = tolower(command[i]);
            }
			default:
            {
                if(commandrejected_idx != -1)
                    return CallLocalFunction(#OnCommandRejected, "isi", playerid, cmdtext, COMMAND_INVALID);
                else return 0;
            }
		}
	}

    new cmd_func[MAX_COMMAND_LENGTH + 4];
    format(cmd_func, sizeof cmd_func, "cmd_%s", command);
    if(funcidx(cmd_func) == -1)
    {
        if(map_has_str_key(CommandAlternatives, cmd_func))
        {
            map_str_get_str(CommandAlternatives, cmd_func, cmd_func, sizeof cmd_func);
            strmid(command, cmd_func, 4, strlen(cmd_func), sizeof command);
        }
        else if(commandrejected_idx != -1)
            return CallLocalFunction(#OnCommandRejected, "isi", playerid, cmdtext, COMMAND_INVALID);
        else return 0;
    }

    if(map_has_str_key(DisabledCommands, cmd_func))
    {
        if(commandreceived_idx != -1)
            return CallLocalFunction(#OnCommandRejected, "isi", playerid, cmdtext, COMMAND_DISABLED);
        else return 0;
    }

    if(map_has_str_key(CommandPermissions, cmd_func))
    {
        new flag = map_str_get(CommandPermissions, cmd_func);
        if((PlayerPermissions[playerid] & flag) != flag)
        {
            if(commandreceived_idx != -1)
                return CallLocalFunction(#OnCommandRejected, "isi", playerid, cmdtext, COMMAND_DENIED);
            else return 0;
        }
    }

    if(commandreceived_idx != -1 && CallLocalFunction(#OnCommandReceived, "is", playerid, cmdtext) == 0)
        return 1;

    CallLocalFunction(cmd_func, "is", playerid, params);

    if(commandperformed_idx != -1)
        CallLocalFunction(#OnCommandPerformed, "is", playerid, cmdtext);

    #if defined COMMAND_DELAY
    if(!PlayerCommandDelayDisabled[playerid] && !map_has_str_key(DelayDisabledCommandList, cmd_func))
    {
        PlayerCommandDelay[playerid] = true;
        SetTimerEx(#CommandDelayTimer, COMMAND_DELAY, false, "i", playerid);
    }
    #endif

    #if defined cmds_OnPlayerCommandText
        return cmds_OnPlayerCommandText(playerid, cmdtext);
    #else
        return 1;
    #endif
}
#if defined cmds_OnPlayerCommandText
    forward cmds_OnPlayerCommandText(playerid, cmdtext[]);
#endif
#if defined _ALS_OnPlayerCommandText
    #undef OnPlayerCommandText
#else
    #define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText cmds_OnPlayerCommandText

#if defined COMMAND_DELAY
forward CommandDelayTimer(playerid);
public CommandDelayTimer(playerid)
{
    PlayerCommandDelay[playerid] = false;
    return 1;
}
#endif

stock AddCommandAlt(const command[], const alternative[])
{
    new alt[MAX_COMMAND_LENGTH + 4];
    new callback[MAX_COMMAND_LENGTH + 4];
    format(alt, sizeof alt, "cmd_%s", alternative);
    format(callback, sizeof callback, "cmd_%s", command);
    if(funcidx(callback) == -1) return 0;
    
    map_str_set_str(CommandAlternatives, alt, callback);
    return 1;
}

stock ToggleCommand(const command[], bool:enabled)
{
    new callback[MAX_COMMAND_LENGTH + 4];

    if(SanitizeCommand(command, callback, sizeof callback) == 0)
        return 0;

    if(!enabled && map_has_str_key(DisabledCommands, callback))
        map_str_remove(DisabledCommands, callback);
    else if(enabled)
        map_str_set(DisabledCommands, callback, true);

    return 1;
}

stock SetCommandPermissionFlag(const command[], flag)
{
    new callback[MAX_COMMAND_LENGTH + 4];

    if(SanitizeCommand(command, callback, sizeof callback) == 0)
        return 0;

    if(flag == 0 && map_has_str_key(CommandPermissions, callback))
        map_str_remove(CommandPermissions, callback);
    else if(flag > 0)
        map_str_set(CommandPermissions, callback, flag);

    return 1;
}

stock SetPlayerCommandPermissionFlag(playerid, flags)
{
    if(!IsPlayerConnected(playerid)) return 0;
    PlayerPermissions[playerid] = flags;
    return 1;
}

#if defined COMMAND_DELAY
stock TogglePlayerCommandDelay(playerid, bool:toggle)
{
    if(!IsPlayerConnected(playerid)) return 0;
    PlayerCommandDelayDisabled[playerid] = !toggle;
    return 1;
}
stock ToggleCommandDelay(const command[], bool:toggle)
{
    new callback[MAX_COMMAND_LENGTH + 4];
    if(SanitizeCommand(command, callback, sizeof callback) == 0)
        return 0;

    if(toggle && map_has_str_key(DelayDisabledCommandList, callback))
        map_str_remove(DelayDisabledCommandList, callback);
    else if(!toggle)
        map_str_set(DelayDisabledCommandList, callback, true);
    return 1;
}
#endif

stock GetRealCommandName(const command[], real[], maxlen = sizeof real)
{
    new callback[MAX_COMMAND_LENGTH + 4];
    #pragma unused callback
    if(SanitizeCommand(command, callback, sizeof callback) == 0)
        return 0;
    strmid(real, callback, 4, strlen(callback), maxlen);
    return 1;
}

stock IsValidCommand(const command[], bool:include_alternative = true)
{
    new callback[MAX_COMMAND_LENGTH + 4];
    #pragma unused callback
    if(SanitizeCommand(command, callback, sizeof callback, include_alternative) == 0)
        return 0;
    return 1;
}
