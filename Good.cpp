#include"stdafx.h"
#include"Good.h"


Good::Good(std::string id, int reNum, int pNum)
{
	goodID=id;
	region=reNum;
	pos=pNum;
}

Good::Good()
{

}

std::string Good::getGoodID()
{
	return goodID;
}

void Good::setGoodID(std::string id)
{
	goodID = id;
}

int Good::getRegionID()
{
	return region;
}

void Good::setRegionID(int reNum)
{
	region = reNum;
}

int Good::getPos()
{
	return pos;
}

void Good::setPos(int pNum)
{
	pos = pNum;
}

