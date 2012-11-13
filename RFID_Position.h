#ifndef RFID_POSITION_H
#define RFID_POSITION_H

#include"MySQLPac.h"
#include"Good.h"
#include"Debug.h"

class RFID_Position
{
public:

	RFID_Position();
	int showAll(vector<Good>&);
	int searchGood(string GoodID);
	int getCurRegion();
	int getCurPos();
	void setCurRegion(int);
	void setCurPos(int);
	void clearFork();
	int addGood(string GoodID);
	int checkGoodID(string GoodID);
	int checkTagID(string TagID);
	int getForkliftSize();
	int putdownGoods(string TagID);
	int liftupGoods(string GoodID,string TagID);
	void showForklift(CString & output);

	~RFID_Position();

private:

	vector<Good> forklift;
	int curRegion,curPos;
	MySQLPac *mysql;
	Debug deg;

};

#endif