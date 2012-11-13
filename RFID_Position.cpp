#include"stdafx.h"
#include"RFiD_Position.h"
#include<math.h>

RFID_Position::RFID_Position()
{
	mysql = new MySQLPac("localhost","root","123456","rfid",3306);
	curRegion = -1;
	curPos = -1;
	forklift.clear();
}

void RFID_Position::setCurRegion(int tRegion)
{
	curRegion = tRegion;
}

int RFID_Position::getCurRegion()
{
	return curRegion;
}

void RFID_Position::setCurPos(int tPos)
{
	curPos = tPos;
}

int RFID_Position::getCurPos()
{
	return curPos;
}

int RFID_Position::showAll(vector<Good>& gr)
{
	// showAll��Ŀ���Ƿ���һ���������л�����Ϣ������
	
	vector<vector<string>> res = mysql->Query("select * from goods,region where goods.TagId=region.TagId;");

	for(size_t i=0;i<res.size();i++)
	{
		Good tGood;
		tGood.setGoodID(res[i][0]);
		tGood.setRegionID(atoi(res[i][4].c_str()));
		tGood.setPos(atoi(res[i][2].c_str()));
		gr.push_back(tGood);
	}

	return 0;
}

int RFID_Position::searchGood(string GoodID)
{
	// ���ݻ���ID��ѯ�������λ�ã�������curRegion,curPos

	vector<vector<string>> res = mysql->Query("select * from goods,region where goods.TagId=region.TagId and goods.GoodsId='"+GoodID+"';");

	if(res.size()==0)
		return -1; // ��ǰ���ﲻ����
	else
	{
		curRegion = atoi(res[0][4].c_str());
		curPos = atoi(res[0][2].c_str());
	}

	return 0;
}

void RFID_Position::clearFork()
{
	forklift.clear();
}

int RFID_Position::addGood(string GoodID)
{
	// ���泵�ϼ������
	// ����ֵ�� 0-�ɹ� -1 - ʧ�ܣ��泵������2�� or �����Ѿ�����

	if(forklift.size()==2)
		return -1;

	for(size_t i=0;i<forklift.size();i++)
		if(forklift[i].getGoodID()==GoodID)
			return -1;

	if(checkGoodID(GoodID)==-1)
		return -1;

	Good tGood;
	tGood.setGoodID(GoodID);
	forklift.push_back(tGood);
	return 0;
}

int RFID_Position::checkGoodID(string GoodID)
{
	// ����ID�Ƿ�ʹ�ù� -1��ʾʹ�ù� 0��ʾûʹ�ù�
	
	vector<vector<string>> res = mysql->Query("select * from goods where goods.GoodsId='"+GoodID+"';");

	if(res.size()==0)
		return 0;
	else
		return -1;
}

int RFID_Position::checkTagID(string TagID)
{
	// ���TagID�Ƿ����

	vector<vector<string>> res = mysql->Query("select * from region where region.TagId='"+TagID+"';");

	if(res.size()==0)
		return 0;
	else
		return -1;
}

int RFID_Position::getForkliftSize()
{
	return forklift.size();
}

int RFID_Position::putdownGoods(string TagID)
{

	vector<vector<string>> res = mysql->Query("SELECT * from goods where TagId = '"+TagID+"';");
	bool space[13];
	memset(space,0,sizeof space);
	for(size_t i=0;i<res.size();i++)
		space[atoi(res[i][2].c_str())]=1;

	int numGood = forklift.size();

	//���µĵ�
	int point=12;
	while(point>0)
	{
		if(space[point])
			break;
		point--;
	}
	
	int col=ceil(point/3.0);
	int surplus = (point%3==0 ? 0 : 3-point%3);

	if(surplus>=numGood || (surplus<numGood && col<4))  //�ŵ���
	{
		if(surplus>=numGood)
			point++;
		else
			point=col*3+1;

		for(size_t i=0;i<forklift.size();i++)
		{
			mysql->Query("insert into goods values('"+forklift[i].getGoodID()+
				"','"+TagID+"',"+int2str(point)+");");

			point++;
		}

		forklift.clear();
		return 0; 
	}

	return -1;
}

int RFID_Position::liftupGoods(string GoodID,string TagID)
{
	// �������ɶ������
	// 0 ��ʾ�ɹ� -1 ��ʾ�д�����

	vector<vector<string>> res = mysql->Query("select * from goods where GoodsID='"+GoodID+"';");

	if(res.size()==0 || res[0][1]!=TagID)
		return -1;  //���ﲻ���ڻ��߸û��ﲻ�ڴ�������

	int pos = atoi(res[0][2].c_str());
	
	//����Ƿ�������㣬�ҽ�����2��
	vector<vector<string>> resSP = mysql->Query("SELECT * from goods where TagId = '"+TagID+"';");
	bool space[13];
	memset(space,0,sizeof space);
	for(size_t i=0;i<resSP.size();i++)
		space[atoi(resSP[i][2].c_str())]=1;

	bool is = false;

	int n=0; 
	for(int i=pos;i<=12;i++)
		if(space[i])
			n++;
	if(n<=2)
		is=true;

	if(!is)
		return false;

	forklift.clear();  //�������������
	while(1)
	{
		Good t;
		for(size_t i=0;i<resSP.size();i++)
			if(atoi(resSP[i][2].c_str())==pos)
			{
				t.setGoodID(resSP[i][0]);
				forklift.push_back(t);
				mysql->Query("delete from goods where GoodsID='"+resSP[i][0]+"';");
				break;
			}
		if(pos%3==0)
			break;
		pos++;
	}

	return 0;
}

void RFID_Position::showForklift(CString & output)
{
	CString str = L"Forklift: ";
	for(size_t i=0;i<forklift.size();i++)
		str+=stringToCString(forklift[i].getGoodID())+L" ";
	output=str;
}

RFID_Position::~RFID_Position()
{
	delete mysql;
}
