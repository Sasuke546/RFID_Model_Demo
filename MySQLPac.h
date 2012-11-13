#ifndef MYSQLPAC_H
#define MYSQLPAC_H

#include"mysql.h"
#include<string>
#include<vector>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")

class MySQLPac
{
public:
	MySQLPac(string tHost,string tUsr,string tPass,string tDB,int tPort);

	vector<vector<string>> Query(string sqlquery);

	int GetConnect();

	~MySQLPac();

private:

	MYSQL SQLdata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	const string Host;
	const string Password;
	const string User;
	const string DB;
	const unsigned int Port;

};


#endif MYSQLPAC_H