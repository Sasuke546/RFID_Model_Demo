#ifndef RFID_H
#define RFID_H

#include <vector>
#include <winsock.h>
#include <iostream>
#include <string>
#include "mysql.h"
#define HOST_IP "localhost"  	//实验室IP：125.216.243.125
#define PASSWORD "123456"           //实验室密码：123456
#define REGION_SIZE 12
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")

struct GoodsRecord{
	string GoodsId;     // id
	int region_num,position;  
};

//传入一个货物的ID和标签的ID，系统会自动更新数据库。程序会自动判断是运货进来还是运货出去
//返回-1表示数据库连接失败，返回1表示从仓库中取出货物，
//返回2表示在仓库中，但是该货物在里层或者上面有货物，不能移动
//返回3表示仓库满了，返回4表示将货物运入仓库
int Update(string GoodsId,string TagId)
{
	//必备的一个数据结构
	MYSQL mydata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	//初始化数据库
	if (0 == mysql_library_init(0, NULL, NULL)) {
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}
	//初始化数据结构
	if (NULL != mysql_init(&mydata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}

	//在连接数据库之前，设置额外的连接选项
	//可以设置的选项很多，这里设置字符集，否则无法处理中文
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		//cout << "mysql_options() succeed" << endl;
	} else {
		//cout << "mysql_options() failed" << endl;
		return -1;
	}


	//连接数据库
	if (NULL!= mysql_real_connect(&mydata, HOST_IP, "root", PASSWORD, "rfid",
		3306, NULL, 0))
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改
	{
		//cout << "mysql_real_connect() succeed" << endl;
	} else {
		//cout << "mysql_real_connect() failed" << endl;
		return -1;
	}

	string my_query;
	bool checkIn=false;
	int cnt=0,goodsLevel=-1;
	my_query="select * from goods where TagId='"+TagId+"'";
	if(!mysql_query(&mydata,my_query.c_str())){
		results=mysql_store_result(&mydata);
		int fieldcount = mysql_num_fields(results);
		while(record=mysql_fetch_row(results)){
			cnt++;
			cout<<record[0]<<endl;
			if(checkIn) continue;
			if(GoodsId==string(record[0])){
				checkIn=true;
				goodsLevel=atoi(record[2]);
			}		
		}
	}
	if(checkIn){
		if(cnt>goodsLevel) {
			mysql_free_result(results);
			mysql_close(&mydata);
			return 2;
		}else {
			string del="delete from goods where GoodsId='"+GoodsId+"'";
			if(!mysql_query(&mydata,del.c_str())){
				mysql_free_result(results);
				mysql_close(&mydata);
				return 1;
			}
		}
	}else{
		if(cnt==REGION_SIZE){
			cout<<"The region is full"<<endl;
			mysql_free_result(results);
			mysql_close(&mydata);
			return 3;
		}else {
			char ccnt[20];
			itoa(cnt+1,ccnt,10);
			string ins ="INSERT INTO goods VALUES( '"+GoodsId+"' , '"+TagId+"' , "+string(ccnt)+" );";
			mysql_query(&mydata,ins.c_str());

			mysql_free_result(results);
			mysql_close(&mydata);
			return 4;
		}
	}

	mysql_free_result(results);
	mysql_close(&mydata);

}

//获得所有record
int getAllRegion(vector<GoodsRecord>& gr)
{

	//必备的一个数据结构
	MYSQL mydata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	//初始化数据库
	if (0 == mysql_library_init(0, NULL, NULL)) {
		//cout << "mysql_library_init() succeed" << endl;
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}


	//初始化数据结构
	if (NULL != mysql_init(&mydata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}


	//在连接数据库之前，设置额外的连接选项
	//可以设置的选项很多，这里设置字符集，否则无法处理中文
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		//cout << "mysql_options() succeed" << endl;
	} else {
		//cout << "mysql_options() failed" << endl;
		return -1;
	}


	//连接数据库
	if (NULL!= mysql_real_connect(&mydata, HOST_IP, "root", PASSWORD, "rfid",
		3306, NULL, 0))
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改
	{
		//cout << "mysql_real_connect() succeed" << endl;
	} else {
		//cout << "mysql_real_connect() failed" << endl;
		return -1;
	}

	string my_query;
	bool checkIn=false;
	int cnt=0;
	my_query="select * from goods order by TagId";
	if(!mysql_query(&mydata,my_query.c_str())){
		results=mysql_store_result(&mydata);
		int fieldcount = mysql_num_fields(results);
		while(record=mysql_fetch_row(results)){
			GoodsRecord tem;
			tem.GoodsId=string(record[0]);
			string check="select * from region where TagId= '"+string(record[1])+"' ";
			mysql_query(&mydata,check.c_str());
			MYSQL_RES * result2=mysql_store_result(&mydata);
			MYSQL_ROW record2=mysql_fetch_row(result2);
			tem.region_num=atoi(record2[1]);
			tem.position=atoi(record[2]);
			gr.push_back(tem);
		}
	}
	mysql_free_result(results);
	mysql_close(&mydata);
}

//返回-1表示数据库连接错误，返回0表示查询的货物不存在，返回1表示查询成功
//返回在哪个region，在哪一个位置1-12
int query(string GoodsId,int& reg_num,int& pos)
{

	//必备的一个数据结构
	MYSQL mydata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	//初始化数据库
	if (0 == mysql_library_init(0, NULL, NULL)) {
		//cout << "mysql_library_init() succeed" << endl;
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}


	//初始化数据结构
	if (NULL != mysql_init(&mydata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}


	//在连接数据库之前，设置额外的连接选项
	//可以设置的选项很多，这里设置字符集，否则无法处理中文
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		cout << "mysql_options() succeed" << endl;
	} else {
		cout << "mysql_options() failed" << endl;
		return -1;
	}


	//连接数据库
	if (NULL!= mysql_real_connect(&mydata, HOST_IP, "root", PASSWORD, "rfid",
		3306, NULL, 0))
		//这里的地址，用户名，密码，端口可以根据自己本地的情况更改
	{
		cout << "mysql_real_connect() succeed" << endl;
	} else {
		cout << "mysql_real_connect() failed" << endl;
		return -1;
	}

	string my_query;
	bool checkIn=false;
	int cnt=0;
	my_query="select * from goods where GoodsId='"+GoodsId+"'";
	if(!mysql_query(&mydata,my_query.c_str())){
		results=mysql_store_result(&mydata);
		if(record=mysql_fetch_row(results)){		
			string check="select * from region where TagId= '"+string(record[1])+"' ";
			mysql_query(&mydata,check.c_str());
			MYSQL_RES * result2=mysql_store_result(&mydata);
			MYSQL_ROW record2=mysql_fetch_row(result2);
			reg_num=atoi(record2[1]);
			pos=atoi(record[2]);
			mysql_free_result(results);
			mysql_close(&mydata);
			return 1;
		}else 
		{
			cout<<"The goods doesn't exist "<<endl;
			mysql_free_result(results);
			mysql_close(&mydata);
			return 0;
		}

	}

	mysql_free_result(results);
	mysql_close(&mydata);
}


#endif