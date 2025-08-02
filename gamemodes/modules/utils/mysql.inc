stock MySQL:g_SQL;

#define		MYSQL_HOST 			"127.0.0.1" 
#define		MYSQL_USER 			"root" 
#define		MYSQL_PASSWORD 		""
#define		MYSQL_DATABASE 		"dcef"

stock DatabaseConnection()
{
	g_SQL = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE);
	if (g_SQL == MYSQL_INVALID_HANDLE || mysql_errno(g_SQL) != 0)
	{
		print("District Roleplay: Connection To MYSQL Failed! Server Shutting Down!");
		SendRconCommand("exit");
	}
	else
	{
		print("District Roleplay: Database successfully connected to MySQL.");
	}
	return 1;
}