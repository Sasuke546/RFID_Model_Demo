
#include<winsock2.h>
//����socket
#include<iostream>
#include "mysql.h"
#pragma comment( lib, "libmysql.lib")
//�˾仰���ڸ���������������libmysql.lib �Ĺ���һ��
 
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
				mysql_query(&mysql,"select * from province");	//����ѯ������浽res��

				res = mysql_store_result( &mysql );	//�洢��¼��

				int j = 0;
				while ( row = mysql_fetch_row( res ) )
				{
					j = mysql_num_fields( res );	//��ȡÿ����¼���ֶ���
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
