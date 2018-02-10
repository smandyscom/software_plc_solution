#include<string>
#include<string.h>
using namespace std;

int main()
{
	char buf[32];
	string s,s2;
//	int i=0;

	strcpy(buf,"FIRST STRING");
	strcat(buf,"\n");
	strcat(buf,"SECOND STRING");

	s = buf;
	s2.assign(s,0,s.find("\n"));
	strcpy(buf,s2.c_str());
	s2 = s.substr(s.find("\n"));
	strcpy(buf,s2.c_str());
		// done, string is able to split/merge by "\n"

	string s3(buf,32);

	return 0;
}