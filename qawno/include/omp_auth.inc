#if defined _inc_omp_auth
    #endinput
#endif

static stock MySQL:Account_g_SqlHandle;

static stock
    bool:Account_g_LoggedIn[MAX_PLAYERS],
    Cache:Account_g_Cache[MAX_PLAYERS],
    Account_g_Id[MAX_PLAYERS],
    Account_g_Ucp[MAX_PLAYERS][255 + 1];

stock bool:Account_Exists(playerid)
{
    if (!IsPlayerConnected(playerid)) return false;
    
    return (!!Account_g_Id[playerid]);
}

stock bool:Account_IsLoggedIn(playerid)
{
    if (!Account_Exists(playerid)) return false;
    
    return (Account_g_LoggedIn[playerid]);
}

stock bool:Account_GetId(playerid, &id)
{
    if (!Account_IsLoggedIn(playerid)) return false;
    
    id = Account_g_Id[playerid];
    return true;
}

stock bool:Account_GetUcp(playerid, ucp[], len = sizeof ucp)
{
    if (!Account_IsLoggedIn(playerid)) return false;
    
    strcopy(ucp, Account_g_Ucp[playerid], len);
    return true;
}

stock bool:Account_SetUcp(playerid, const newUcp[])
{
    if (!Account_IsLoggedIn(playerid)) return false;
    
    new oldUcp[255 + 1];
    strcopy(oldUcp, Account_g_Ucp[playerid]);
    
    new szSqlUpdate[256];
    mysql_format(Account_g_SqlHandle, szSqlUpdate, sizeof szSqlUpdate, 
        "UPDATE characterucp SET ucp = '%e' WHERE ucp = '%e'", 
        newUcp,
        oldUcp
    );
    mysql_pquery(Account_g_SqlHandle, szSqlUpdate);    
    
    strcopy(Account_g_Ucp[playerid], newUcp, 255);
    return true;
}

stock bool:Account_CheckPlayer(playerid)
{
    if (!IsPlayerConnected(playerid)) return false;

    new playerName[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, playerName);
    
    await mysql_aquery_s(Account_g_SqlHandle, str_format("SELECT * FROM characterucp WHERE ucp = '%e'", playerName));

    new rows;
    cache_get_row_count(rows);
    
    if (!rows) return true;
    
    CallRemoteFunction("OnPlayerAccountChecked", "ii", playerid, rows);

    Account_g_Cache[playerid] = cache_save();
    cache_get_value_int(0, "id", Account_g_Id[playerid]);
    cache_get_value(0, "ucp", Account_g_Ucp[playerid]);
    cache_unset_active();
    return true;
}

stock bool:Account_Login(playerid, const password[])
{
    if (!Account_Exists(playerid))
    {
        print("[ACCOUNT-SYSTEM ERROR]: You shouldn't call this when the user doesn't have the account data!");
        return false;
    }
    
    if (IsNull(password))
    {
        return false;
    }
    
    if (!cache_is_valid(Account_g_Cache[playerid]))
    {
        new playerName[MAX_PLAYER_NAME + 1];
        GetPlayerName(playerid, playerName);
        await mysql_aquery_s(Account_g_SqlHandle, str_format("SELECT * FROM characterucp WHERE ucp = '%e'", playerName));

        if (!cache_num_rows())
        {
            printf("[ACCOUNT-SYSTEM ERROR]: User account for %s is gone!", playerName);
            return 0;
        }

        Account_g_Cache[playerid] = cache_save();
        cache_get_value_int(0, "id", Account_g_Id[playerid]);
        cache_get_value(0, "ucp", Account_g_Ucp[playerid]);
    }
    else
    {
        cache_set_active(Account_g_Cache[playerid]);
    }
    
    new hash[BCRYPT_HASH_LENGTH];
    cache_get_value(0, "password", hash);
    cache_unset_active();
    
    new Task:bcrypt_task = task_new();
    new ret = bcrypt_verify(playerid, "OnAccountVerifyPass__", "d", _:bcrypt_task);
    
    if (!ret)
    {
        task_set_error_ms(bcrypt_task, amx_err_exit, 0);
        return false;
    }
    
    new success = await bcrypt_task;
    
    if (success)
    {
        // Update last login timestamp
        mysql_format(Account_g_SqlHandle, szQuery, sizeof szQuery, 
            "UPDATE characterucp SET lastlogin = %d WHERE id = %d", 
            gettime(), 
            Account_g_Id[playerid]
        );
        mysql_pquery(Account_g_SqlHandle, szQuery);
        
        cache_delete(Account_g_Cache[playerid]);
        Account_g_Cache[playerid] = MYSQL_INVALID_CACHE;
        Account_g_LoggedIn[playerid] = true;
        CallRemoteFunction("OnPlayerAccountLogin", "ii", playerid, Account_g_Id[playerid]);
        return true;
    }
    
    return false;
}

stock bool:Account_Register(playerid, const ucp[], const password[])
{
    if (IsNull(ucp) || IsNull(password)) return false;
    
    new Task:bcrypt_task = task_new();
    bcrypt_hash(playerid, "OnAccountHashedPass__", "d", _:bcrypt_task, password, BCRYPT_COST);
    
    new hash[BCRYPT_HASH_LENGTH];
    task_await_arr(bcrypt_task, hash);
    
    new szQuery[512];
    mysql_format(Account_g_SqlHandle, szQuery, sizeof szQuery,
        "INSERT INTO characterucp (userid, ucp, password, active, codeverify, coin, lastlogin, createdAt, updatedAt) \
        VALUES ('%e', '%s', '%e', 0, 0, 0, %d, NOW(), NOW())",
        "temp_userid",
        ucp,
        hash,
        gettime()
    );
    
    await mysql_aquery_s(Account_g_SqlHandle, szQuery);
    
    Account_g_Id[playerid] = cache_insert_id();
    strcopy(Account_g_Ucp[playerid], ucp, 255);
    Account_g_LoggedIn[playerid] = true;
    
    CallRemoteFunction("OnPlayerAccountRegister", "ii", playerid, Account_g_Id[playerid]);
    return true;
}

stock void:Account_Logout(playerid)
{
    if (cache_is_valid(Account_g_Cache[playerid]))
    {
        cache_delete(Account_g_Cache[playerid]);
    }
    
    new id = Account_g_Id[playerid];
    
    Account_g_Id[playerid] = 0;
    Account_g_Ucp[playerid][0] = 0;
    Account_g_LoggedIn[playerid] = false;
    
    CallRemoteFunction("OnPlayerAccountLogout", "ii", playerid, id);
}

forward OnAccountVerifyPass__(playerid, success, Task:t);
forward OnAccountHashedPass__(playerid, hashid, Task:t);

public OnAccountVerifyPass__(playerid, success, Task:t)
{
    task_set_result(t, success);
    return 0;
}

public OnAccountHashedPass__(playerid, hashid, Task:t)
{
    new hash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hash);
    task_set_result_arr(t, hash);
    return 0;
}