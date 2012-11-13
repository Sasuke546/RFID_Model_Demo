#include"stdafx.h"
#include"Debug.h"

Debug::Debug()
{
	//outfile.open("Log.txt",std::ofstream::app);
}

Debug::~Debug()
{
	//outfile.close();
}

std::string Debug::getTime()
{
	time_t rawtime; 
	struct tm * timeinfo; 

	time ( &rawtime ); 
	timeinfo = localtime ( &rawtime ); 
	return std::string(asctime(timeinfo)); 
}

void Debug::write(CString s)
{
	outfile.open("Log.txt",std::ofstream::app);
	outfile<<getTime()<<" ";
	outfile<<CStringToString(s)<<std::endl;
	outfile.close();
}

void Debug::write(std::string s)
{
	outfile.open("Log.txt",std::ofstream::app);
	outfile<<getTime()<<" ";
	outfile<<s<<std::endl;
	outfile.close();
}

void Debug::closeFile()
{
	outfile.close();
}