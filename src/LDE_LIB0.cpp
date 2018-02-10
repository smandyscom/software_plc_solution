#include<LDE_LIB0.h>

typedef vector<VERTEX_SN>	SN_LIST;

static int	nextVerticeScan	(LD_ARRAY	&sourceArray
							 ,size_t	row
							 ,size_t	column
							 ,SN_LIST	&ls){

	/*static*/ int errorCode;
	//-------------------------------------------------
	// find next vertices along route , and store them
	//--------------------------------------------------------------
	// why use vector rather C-array :
	//  1.easy to allocate dynamically , build-in search, sort methods
	//	2.secure issue , C-array can't prevent access violation
	//		e.g, when next row either next column doesn't existed
	//
	//	if firt argument is not passed-by-reference , it would cause heap curruption
	//		due to destructor of ldElement would try to free vertice*
	//--------------------------------------------------------------
	if(row > (sourceArray.size()-1))	// overflow case included
		return STG_ERR_OVERINDEX_ROW;
	if(column > (sourceArray.at(row).size()-1))
		return STG_ERR_OVERINDEX_COLUMN;

	switch(sourceArray.at(row).at(column).getType()){		// right way to access , Hsien , 2012.05.09
		case BRANCH_UP_FORWARD:
		case BRANCH_UP_FORWARD_TSECTION_TYPE1:	// extension code
		case BRANCH_UP_FORWARD_TSECTION_TYPE2:	// extension code
			
			//if((errorCode = nextVerticeScan(sourceArray,row		,column+1	,ls)) != STG_SUCCESS)
			//	return errorCode;				// scan forward
			if((errorCode = nextVerticeScan(sourceArray,row+1	,column		,ls)) != STG_SUCCESS)
				return errorCode;				// scan upward
			if((errorCode = nextVerticeScan(sourceArray,row		,column+1	,ls)) != STG_SUCCESS)
				return errorCode;				// scan forward


			return STG_SUCCESS/*BRANCH_UP_FORWARD*/;							// return success , commented by Hsien, 2012.06.14
			break;
		case BRANCH_FORWARD:
		case BRANCH_FORWARD_TERMINAL:			// extension code
		case BRANCH_FORWARD_TSECTION:			// extension code
			if((errorCode = nextVerticeScan(sourceArray,row,column+1,ls)) != STG_SUCCESS)
				return errorCode;				// scan forward
			
			return STG_SUCCESS/*BRANCH_FORWARD*/;							// return success , commented by Hsien, 2012.06.14
			break;
		case BRANCH_DOWN:
		case BRANCH_DOWN_TERMINAL:				// extension code
		case BRANCH_DOWN_TSECTION:				// extension code , from upper column and forward
			if((errorCode = nextVerticeScan(sourceArray,row-1,column,ls)) != STG_SUCCESS)
				return errorCode;	//	scan downward
			return STG_SUCCESS/*BRANCH_DOWN*/;							// return success , commented by Hsien, 2012.06.14
			break;
		case VERTICE:
			ls.push_back(sourceArray.at(row).at(column).getVertice()->getSn());
			return STG_SUCCESS/*VERTICE*/;							// return success , commented by Hsien, 2012.06.14
		case EMPTY:
			return STG_ERR_ROUTEOPENED;				// route opened , graph error.
		default:
			return STG_ERR_UNKNOWN;							
			break;
	}
}

int			collectVertices	(LD_ARRAY &sourceArray,LD_DEVICE_LIST &buffer)
{
	LD_ARRAY::iterator	itrow;
	LD_ROW::iterator	itcolumn;
	vertice*			vPtr;

	//--------------------------------------
	//	grab all vertice existed in ldarray 
	//		casting-back to parent type before loading to device_list
	//		so that , need to do getData , to solve generated-string
	//--------------------------------------
	resetSn();	// interface function , reset history of serial number marking in heap

	for(itrow = sourceArray.begin(); itrow!=sourceArray.end();itrow++){
		for( itcolumn = itrow->begin(); itcolumn != itrow->end();itcolumn++){
			if(itcolumn->getType() == VERTICE){
				vPtr = itcolumn->getVertice();
				releaseSn(*vPtr);	// release serial number
				dispatchSn(*vPtr);// mark serial number

				vPtr->getData();				// added by Hsien , 2012.09.25
												// for data-generating
				buffer.push_back(*vPtr);		// copy , excepation happens , [8/27/2012 smandyscom]
			}
		}//inner for()
	}//outter for()	
		
	return STG_SUCCESS;
}

int			ld2EdgeStream	(LD_ARRAY &sourceArray,EDGE_STREAM &edgeStream)
{
	int					errorCode = 0;
	short*				errorReason = (short*)&errorCode;
	unsigned char*		errorRow	= &((unsigned char*)&errorCode)[2];
	unsigned char*		errorColumn = &((unsigned char*)&errorCode)[3];

	size_t				row,column;
	SN_LIST				ls;
	SN_LIST::iterator	itls;
	edge_st				buf_edge;
	
	//---------------------------------------------------------------------------
	//	row by row , column by column scanning
	//	when vertice meeted , start "next vertex scan" procedure
	//	to collect all vertice connected to this one , store their serial numbers 
	//	then append edge information in back of edge stream , which used to describe connectivity
	//	of this graph.
	//---------------------------------------------------------------------------
	for(row = 0; row < sourceArray.size();row++)
		for( column = 0; column < sourceArray[row].size();column++){
			if(sourceArray[row][column]/*.at(row).at(column)*/.getType() == VERTICE){
				//-----------------------------------------------------------------
				//	Scan next vertice when this one is vertice , Hsien , 2013.04.01
				//-----------------------------------------------------------------
				if(column != (sourceArray[row].size()-1)){
					//--------------------------------------
					// prevent accesing dgeStream violation
					// Hsien , 2012.05.28
					//--------------------------------------

					errorCode = nextVerticeScan(sourceArray,row,column+1,ls);
					if(errorCode == STG_ERR_OVERINDEX_ROW
						||	errorCode == STG_ERR_OVERINDEX_COLUMN
						||  errorCode == STG_ERR_ROUTEOPENED
						||	errorCode == STG_ERR_UNKNOWN){					// error is covered with	normal code
																			// Hsien , 2012.10.13
						//*errorReason	= (short)errorCode;
						//*errorRow		= (unsigned char)row;
						//*errorColumn	= (unsigned char)column;

						return errorCode;
					}
					//-----------------------
					//	if there's any connection for this vertice , 
					// build-up edge stream
					// merge into main trunk
					//	otherwise , do nothing
					//		bug killed by Hsien , 2012.06.01
					//-----------------------
					if(!ls.empty()){	
						buf_edge.from = (sourceArray[row][column]/*.at(row).at(column)*/.getVertice())->getSn();	
						for(itls=ls.begin();itls != ls.end() ; itls++){
							buf_edge.to = *itls;
							edgeStream.push_back(buf_edge);
						}
					}
				}
				ls.clear();			// end of procedure , clear buffer
			}//inner for
		}// outer for
	return STG_SUCCESS;
}
