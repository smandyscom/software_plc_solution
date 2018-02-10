#include<LDE_METADATA.H>
#include<stdlib.h>
#include<string.h>
using namespace metaData;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
/*int main()
{
	char*	str=0,*str2 = 0;
	size_t	tmpSize;
	int		iCode = 7;
	DEV_METADATA dmds,dmds_duplicated;

	str = (char*)realloc(str,0);
	str = (char*)realloc(str,0);

//	strlen(str);

	str = (char*)malloc(1);
	str = (char*)realloc(str,strlen("tfghjkl;")+1);
	str = strcpy(str,"tfghjkl;");
	//----------
	//	life cycle of dmds
	//----------
	initialize(&dmds);
	initialize(&dmds_duplicated);

	if(!writeIn(&dmds,iCode,str))
		return 0;

	free(str);

	str = (char*)readOut(&dmds_duplicated,iCode);
	writeIn(&dmds_duplicated,iCode,str);
	writeIn(&dmds,iCode,str);
	str = (char*)readOut(&dmds,iCode);

	duplicate(&dmds_duplicated,dmds);

	finalize(&dmds);
	finalize(&dmds_duplicated);

	return 0;
}*/

//int main()
//{
//	DEV_METADATA	tmp;
//	ROW_METADATA	rowmds,rowmds_backUp;
//	RUNG_METADATA	rmds_deep,rmds_shallow;
//
//	initialize(&rowmds,DEEP);
//	initialize(&rowmds_backUp,DEEP);
//	initialize(&tmp);
//	writeIn(&tmp,8,"THIS IS TMP DATA1");
////	rowmds.imds[7] = new DEV_METADATA;
////	initialize(rowmds.imds[7]);
//	duplicate(rowmds.imds[7],tmp);
//	duplicate(&rowmds_backUp,rowmds); // fine , no memory leakage
//
//	finalize(&rowmds,DEEP);
//
//	initialize(&rowmds,DEEP);
//	duplicate(rowmds.imds[9],tmp);
//
//	initialize(&rmds_deep);
//	initialize(&rmds_shallow);
//	createTable(&rmds_deep,1,DEEP);
//	createTable(&rmds_shallow,1,SHALLOW);
//
//	writeIn(&rmds_shallow,0,&rowmds_backUp);
//	duplicate(rmds_deep.rows[0],rowmds_backUp);
//
//	return 0;
//}

#define NUM 10

//int main()
//{
//	/* following section is used to check if any finalizer and duplicator */
//	// is possible to cause memory leakage
//	DEV_METADATA*	leaf = device::initialize(),*leaf2;
//	device::finalize(leaf);
//	_CrtDumpMemoryLeaks();
//
//	leaf	= device::initialize();
//	leaf2	= device::duplicate(leaf);
//	device::finalize(leaf);
//	device::finalize(leaf2);
//	_CrtDumpMemoryLeaks();					// above things tested ok , no memory leakage
//
//	leaf = device::initialize();
//	CLST_METADATA*	layer1 = 
//		cluster::initialize(
//		(CP_CONSTRUCTOR)device::duplicate
//		,(DESTRUCTOR)device::finalize,NUM);
//	//layer1->headData = device::initialize();
//	CLST_METADATA*	layer2;
//
//	device::writeIn(leaf,9,"THIS IS LEAF");	
//	for(int i=0;i<NUM;i++)
//		layer1->dataCluster[i] = (void*)device::duplicate(leaf);		// un-initilized leaf would cause error
//	device::finalize(leaf);
////	cluster::finalize(layer1);		// finalizer test ok , no memory leakage
////	_CrtDumpMemoryLeaks();
//
//	/*layer2 = */cluster::finalize(cluster::duplicate(layer1));
//	//cluster::finalize(layer2);
//	cluster::finalize(layer1);
//	_CrtDumpMemoryLeaks();		// ok , all funcs tested , no leakage happend
//
//	//device::duplicate(leaf);
//
//	return 1;
//}
//
int main()
{
	/* multi-layer cluster test */
	DEV_METADATA	*leaf = device::initialize();
	CLST_METADATA	*root,*layer_middle,*tree_duplicate;

	root = cluster::initialize((CP_CONSTRUCTOR)cluster::duplicate,(DESTRUCTOR)cluster::finalize,NUM);
	layer_middle = cluster::initialize((CP_CONSTRUCTOR)device::duplicate,(DESTRUCTOR)device::finalize,NUM);

	device::writeIn(leaf,67,"THIS IS LEAF");
	for(int i=0;i<NUM;i++)
		layer_middle->dataCluster[i] = device::duplicate(leaf);
	device::finalize(leaf);

	for(int i=0;i<NUM;i++)
		root->dataCluster[i] = cluster::duplicate(layer_middle);
	cluster::finalize(layer_middle);

	tree_duplicate = cluster::duplicate(root);

	cluster::finalize(root);
	cluster::finalize(tree_duplicate);
	_CrtDumpMemmoryLeaks();		
	return 1;		// no leakage happened
}