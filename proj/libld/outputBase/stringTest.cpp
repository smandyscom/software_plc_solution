#include<iostream>
#include<string>
#include<sstream>
using namespace std;

int main()
{
	string source = "?1 = ?2 ";
	string str;

	size_t m = source.find("?2",0);

	source.replace(m,m+2,"X0.00");

	m = source.find("?1",0);
	source.replace(m,strlen("?1"),"???%$#@");

	str = "?";
	//char tmp[2];
	//str.append(itoa(1,tmp,10));

	stringstream ss;
	ss << 1;					// use this to do number-string conversion
	str.append(ss.str());

	char string[64];
	sprintf(string,"%s = %s","X0.00","X0.01");	// quiet a simple implementaion way


	return 0;
}