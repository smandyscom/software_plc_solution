#include<input_vertice.h>
#include<iostream>
using namespace std;

int main(){
	vertice v;
	normalOpen	vNo("NO0.1");
	normalClose	vNc("NC0.1");
	_short		vSt;

	//----
	//	test every funtion for both parent and child
	//----
	
	//---
	// operator=
	v = ((vertice)vNo);		// cast back to parent class , the possible way so far
	v = ((vertice)vNc);		// success

	v = vNo & vNc ;			// data merge

	((vertice)vNo) = "KI0.0";
	((vertice)vNo) = v.getData();		// fail , it write into stack area. can't hold it.
	((vertice)vNo) = v;

	((vertice)vNo).setData(v.getData());// fail
	((vertice*)&vNo)->setData(v.getData());	// success
	*((vertice*)&vNo) = "KI0.0";			// successfully tracking the physical  vNo

	*((vertice*)&vNo) = vNo & vNc	;		// data merge and store , ok!
	*((vertice*)&vNc) = vNo & vNc	;		// data merge and store , ok!
	*((vertice*)&vSt) = vNo & vNc	;		// data merge and store , ok!
											// write back into rest verice
	return 0;
}