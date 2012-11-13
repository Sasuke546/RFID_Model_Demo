#include "stdafx.h"
#include "MySQLPac.h"

MySQLPac::MySQLPac(string tHost,
				   string tUsr,
				   string tPass,
				   string tDB,
				   int tPort):
	Host(tHost),User(tUsr),Password(tPass),DB(tDB),Port(tPort)
{
	
/*	Host = "localhost";
	Password = "123456";
	User = "root";
	DB = "rfid";
	Port = 3306;*/
	
	if(GetConnect()!=0);

}

int MySQLPac::GetConnect()
{
	if (0 == mysql_library_init(0, NULL, NULL)) {
		//cout << "mysql_library_init() succeed" << endl;
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}

	//初始化数据结构
	if (NULL != mysql_init(&SQLdata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}


	//在连接数据库之前，设置额外的连接选项
	if (0 == mysql_options(&SQLdata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		//cout << "mysql_options() succeed" << endl;
	} else {
		//cout << "mysql_options() failed" << endl;
		return -1;
	}


	//连接数据库
	if (NULL!= mysql_real_connect(&SQLdata, Host.c_str(), User.c_str(), Password.c_str(), DB.c_str(),
		Port, NULL, 0))
	{
		//cout << "mysql_real_connect() succeed" << endl;
	} else {
		//cout << "mysql_real_connect() failed" << endl;
		return -1;
	}

	return 0;
}

vector<vector<string>> MySQLPac::Query(string sqlquery)
{
	vector<vector<string>> res;

	if(!mysql_query(&SQLdata,sqlquery.c_str()))
	{
		results=mysql_store_result(&SQLdata);
		if(results==NULL)
			return res;
		int fieldcount = mysql_num_fields(results);
		while(record=mysql_fetch_row(results))
		{
			vector<string> subres;
			for(int i=0;i<fieldcount;i++)
			{
				string field = string(record[i]);
				subres.push_back(field);
			}
			res.push_back(subres);
		}
		mysql_free_result(results);
	}

	return res;
}

MySQLPac::~MySQLPac()
{
	mysql_close(&SQLdata);
}
