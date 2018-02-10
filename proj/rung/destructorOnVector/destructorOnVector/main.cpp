#include <vector>
#include <iostream>
using namespace std;

class object{
public:
	object();
	~object();
};

int main()
{

	object ob;
	vector<object>	obVector;

	for (int i=0;i<10;i++)
		obVector.push_back(ob);

	return 0;
}

object::object()
{
	cout<<"constructor\n";
}
object::~object()
{
	cout<<"destructor\n";	// destructor had been called several times when pushing back vector.
}