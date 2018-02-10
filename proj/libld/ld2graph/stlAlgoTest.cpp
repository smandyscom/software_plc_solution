#include<LD_ELEMENTS.h>
#include<algorithm>
using namespace std;

bool isMatch(vertice* v1)
{
	return (v1->getSn() == 3);
}

bool isMatch2(vertice* v1,short& sn)
{
	return (v1->getSn() == sn);
}
int main()
{
	vector<vertice*> vv;
	vector<vertice*>::iterator it;

	for(int i=0;i<10;i++)
		vv.push_back(new vertice());

	for(int i=0;i<10;i++)
		vv[i]->setSn();

	it = find_if(vv.begin(),vv.end(),isMatch);

	short sn = 3;
	it = find_first_of(vv.begin(),vv.end(),&sn,&sn,isMatch2);

	return 0;
}