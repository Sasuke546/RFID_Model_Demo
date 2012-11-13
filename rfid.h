#ifndef RFID_H
#define RFID_H

#include <vector>
#include <winsock.h>
#include <iostream>
#include <string>
#include "mysql.h"
#define HOST_IP "localhost"  	//ʵ����IP��125.216.243.125
#define PASSWORD "123456"           //ʵ�������룺123456
#define REGION_SIZE 12
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")

struct GoodsRecord{
	string GoodsId;     // id
	int region_num,position;  
};

//����һ�������ID�ͱ�ǩ��ID��ϵͳ���Զ��������ݿ⡣������Զ��ж����˻����������˻���ȥ
//����-1��ʾ���ݿ�����ʧ�ܣ�����1��ʾ�Ӳֿ���ȡ�����
//����2��ʾ�ڲֿ��У����Ǹû����������������л�������ƶ�
//����3��ʾ�ֿ����ˣ�����4��ʾ����������ֿ�
int Update(string GoodsId,string TagId)
{
	//�ر���һ�����ݽṹ
	MYSQL mydata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	//��ʼ�����ݿ�
	if (0 == mysql_library_init(0, NULL, NULL)) {
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}
	//��ʼ�����ݽṹ
	if (NULL != mysql_init(&mydata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}

	//���������ݿ�֮ǰ�����ö��������ѡ��
	//�������õ�ѡ��ܶ࣬���������ַ����������޷���������
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		//cout << "mysql_options() succeed" << endl;
	} else {
		//cout << "mysql_options() failed" << endl;
		return -1;
	}


	//�������ݿ�
	if (NULL!= mysql_real_connect(&mydata, HOST_IP, "root", PASSWORD, "rfid",
		3306, NULL, 0))
		//����ĵ�ַ���û��������룬�˿ڿ��Ը����Լ����ص��������
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

//�������record
int getAllRegion(vector<GoodsRecord>& gr)
{

	//�ر���һ�����ݽṹ
	MYSQL mydata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	//��ʼ�����ݿ�
	if (0 == mysql_library_init(0, NULL, NULL)) {
		//cout << "mysql_library_init() succeed" << endl;
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}


	//��ʼ�����ݽṹ
	if (NULL != mysql_init(&mydata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}


	//���������ݿ�֮ǰ�����ö��������ѡ��
	//�������õ�ѡ��ܶ࣬���������ַ����������޷���������
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		//cout << "mysql_options() succeed" << endl;
	} else {
		//cout << "mysql_options() failed" << endl;
		return -1;
	}


	//�������ݿ�
	if (NULL!= mysql_real_connect(&mydata, HOST_IP, "root", PASSWORD, "rfid",
		3306, NULL, 0))
		//����ĵ�ַ���û��������룬�˿ڿ��Ը����Լ����ص��������
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

//����-1��ʾ���ݿ����Ӵ��󣬷���0��ʾ��ѯ�Ļ��ﲻ���ڣ�����1��ʾ��ѯ�ɹ�
//�������ĸ�region������һ��λ��1-12
int query(string GoodsId,int& reg_num,int& pos)
{

	//�ر���һ�����ݽṹ
	MYSQL mydata;
	MYSQL_ROW record;
	MYSQL_RES * results;

	//��ʼ�����ݿ�
	if (0 == mysql_library_init(0, NULL, NULL)) {
		//cout << "mysql_library_init() succeed" << endl;
	} else {
		//cout << "mysql_library_init() failed" << endl;
		return -1;
	}


	//��ʼ�����ݽṹ
	if (NULL != mysql_init(&mydata)) {
		//cout << "mysql_init() succeed" << endl;
	} else {
		//cout << "mysql_init() failed" << endl;
		return -1;
	}


	//���������ݿ�֮ǰ�����ö��������ѡ��
	//�������õ�ѡ��ܶ࣬���������ַ����������޷���������
	if (0 == mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "utf8")) {
		cout << "mysql_options() succeed" << endl;
	} else {
		cout << "mysql_options() failed" << endl;
		return -1;
	}


	//�������ݿ�
	if (NULL!= mysql_real_connect(&mydata, HOST_IP, "root", PASSWORD, "rfid",
		3306, NULL, 0))
		//����ĵ�ַ���û��������룬�˿ڿ��Ը����Լ����ص��������
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