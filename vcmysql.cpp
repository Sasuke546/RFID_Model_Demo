
#include<winsock2.h>
//定义socket
#include<iostream>
#include "mysql.h"
#pragma comment( lib, "libmysql.lib")
//此句话和在附加依赖项中增加libmysql.lib 的功能一样
 
using namespace std;
int main(char **args)
{
		MYSQL * myData;
		MYSQL_RES * res;
		MYSQL_ROW row;
        MYSQL mysql;
        mysql_init(&mysql);
//		myData = mysql_init((MYSQL*) 0); 
        if (mysql_real_connect(&mysql,"localhost","root","123456","test",3306,0,0))
        {
				mysql_query(&mysql,"select * from province");	//将查询结果保存到res中

				res = mysql_store_result( &mysql );	//存储记录集

				int j = 0;
				while ( row = mysql_fetch_row( res ) )
				{
					j = mysql_num_fields( res );	//获取每条记录的字段数
					for ( int k = 0; k < j ; k++ )
					{
						cout<<row[k]<<endl;
					}

				}
                cout<<"ok"<<endl;
                return 0;
        }
        else
        {
                int i = mysql_errno(&mysql);
                const char * s = mysql_error(&mysql);
                cout << s<<endl;
				return 0;
        }
}
