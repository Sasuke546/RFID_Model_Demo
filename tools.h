#ifndef TOOLS_H
#define TOOLS_H

#include<string>

inline std::string CStringToString( const CString & cstr ){   //CString to string
	std::string s;
	s.resize( cstr.GetLength() );
	for( int i=0; i<cstr.GetLength(); i++ )
		s[i] = (char)cstr[i];
	return s;
}

inline CString stringToCString( const std::string & s ){   //string to CString
	return CString( s.c_str() );
}

inline std::string int2str(int x)
{
	std::string res;
	while(x>0)
	{
		res+= x%10+'0';
		x/=10;
	}
	int len = res.length();
	for(size_t i=0;i<res.length()/2;i++)
	{
		char t = res[i];
		res[i]=res[len-i-1];
		res[len-i-1]=t;
	}
	return res;
}

#endif