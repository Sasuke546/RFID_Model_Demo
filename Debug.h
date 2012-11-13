#ifndef DEBUG_H
#define DEBUG_H

#include"tools.h"
#include <time.h> 
#include<fstream>

class Debug
{
public:
	Debug();
	void write(std::string s);
	void write(CString s);
	std::string getTime();
	void closeFile();
	~Debug();

private:
	std::ofstream outfile;

};

#endif