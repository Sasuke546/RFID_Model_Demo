#ifndef GOOD_H
#define GOOD_H

#include<string>

class Good
{
public:
	Good(std::string id,int reNum,int pNum);
	Good();
	std::string getGoodID();
	void setGoodID(std::string id);
	int getRegionID();
	void setRegionID(int reNum);
	int getPos();
	void setPos(int pNum);

private:
	std::string goodID;
	int region,pos;
};


#endif