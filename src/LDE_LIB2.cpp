//---------------------------------------------------------
// FILENAME				: LDE_LIB2.h
// ORIGIN AYTHOR		: Hsien , 2012.09.26
// EDITOR				: Hsien
// NOTE					: 
//----------------------------------------------------------
#include<LDE_LIB2.h>

static int	skeletonAssign	(OD_ARRAY &coils,string &expr,string &stmt)
{
	//-------------------
	//	appendence mode
	//-------------------
	OD_ARRAY::iterator	it;

	for(it = coils.begin(); it!= coils.end() ; it++){
		stmt.append(it->getGenStr());
		stmt.append("=");				// generating a=b=c=
	}

	stmt.append(expr);					// attach expression
	stmt.append(";\n");					// result as a=b=c=expr;

	return STG_SUCCESS;
}

static int	skeletonIf		(OD_ARRAY &sets	,string &expr		,string &stmt)
{
	//-------------------
	//	appendence mode
	//-------------------
	OD_ARRAY::iterator	it;

	stmt.append("if(");
	stmt.append(expr);
	stmt.append("){\n");		// next line

	for(it = sets.begin(); it!= sets.end()	; it++){
		stmt.append(it->getGenStr());
		stmt.append("\n");
	}

	stmt.append("}\n");
	stmt.append("end");		// added by Hsien  ,2013.02.04
							// new grammar onlined.

	return STG_SUCCESS;
}
static int	skeletonSwitcher(const unsigned char kind		
							 ,OD_ARRAY &subBuffer
							 ,string &expr
							 ,string &os)
{
	switch(kind){
		case SINGLE:	skeletonAssign(subBuffer,expr,os);	break;		//	appendence mode
		case IF_STMT:	skeletonIf(subBuffer,expr,os);		break;		//	appendence mode
		case EMPTY_STMT:	os.append(";\n");				break;		//	appendence mode
		default:
			return STG_ERR_UNKNOWN;			// unknown kind
			break;
	}//switch

	return STG_SUCCESS;
}


int	exprGenerator	(LD_ARRAY &sourceArray,string	&os)
{
	//----------------------------------------------------------------------
	//	1. collect vertices
	//	2. then checking if vertices are valid.
	//	3. figure out how these vertices linked
	//	4. do adjecent matrix analyzing , and reducing to a single statement
	//----------------------------------------------------------------------
	LD_DEVICE_LIST	inpDev_list	;	 
	EDGE_STREAM		es;
	adjMatrix		am;
	int				errCode;

	LD_DEVICE_LIST::iterator itVi,itVj,it;

	if((errCode = collectVertices	(sourceArray,inpDev_list)) != STG_SUCCESS)
		return errCode;											//	derive vertices from source array , store em as a list
	if((errCode = ld2EdgeStream	(sourceArray,es)) != STG_SUCCESS)	//	analysing connection of vertice-to-vertice , store em as a edge list
		return errCode;	// connection error
	am.build(es);											//	edge stream -> building adjecent matrix

	//--------------------------------------------------------------
	//	adjecent matrix + devices list -> seires-parallel reduction
	//--------------------------------------------------------------
	while(am.queryState() != FULLY_REDUCED){
		am.stepReduce();
		//----------------------------------
		//	search for target reduction pair
		//----------------------------------
		for(it=inpDev_list.begin();it != inpDev_list.end() ; it++){
			if(it->getSn() == am.queryVi())
				itVi = it; 
			if(it->getSn() == am.queryVj())
				itVj = it;
		}//for()

		switch(am.queryState()){
			case SS_REDUCED : *itVi = *itVi & *itVj; break;
			case SP_REDUCED : *itVi = *itVi | *itVj; break;
			case FULLY_REDUCED:						 break;
			case NO_MORE_REDUCTION:		// unexpected adj matrix , do error handling
			default:
				return STG_ERR_UNKNOWN;
				break;
		}//switch()
	}//while()

	if(am.queryVi() != 0)
		return STG_ERR_ROUTEOPENED;				// if the last one reduced vertice is not the head vertice(ST_HEAD)
								//	, means there's open connection in rung(input error)
								// Hsien , 2012.06.05
	os.clear();
	os = itVi->getData();

	return STG_SUCCESS;
}

int stmtGenerator	(OD_ARRAY	&allOds,string &expr,string &os)
{
	//---------------
	//	scanning allods , if conitigous ods are the same type
	//		outputing them in specific skeleton , in expression->output sentence form
	//	use look-backward instead of look-ahead , for avoiding vector<> accesing violation
	//		Hsien , 2012.09.25
	//---------------
	OD_ARRAY::iterator	it;				// iterator , manipulating usage
	OD_ARRAY			subBuffer;		// subbuffer to load part of ods from allOds array.
	unsigned char		previousKind;	// previous kind to current element
	int					errCode;

	it			= allOds.begin();
	previousKind= it->getKind();	// initial skeleton kind

	for(it	= allOds.begin();it != allOds.end();it++){
		if(/*violating accessing raised , look-ahead is not suit for iterator*/
				(it/*+1*/)->getKind() != previousKind){
			//--------------------------------------------------------
			//		condition : next kind is not equal to current kind
			//--------------------------------------------------------
			if(skeletonSwitcher(previousKind,subBuffer,expr,os) == STG_ERR_UNKNOWN)
				return STG_ERR_UNKNOWN;							// output , appendence mode
			subBuffer.clear();									// after outputing , clear buffer
			previousKind = it->getKind();	
		}//if()
			subBuffer.push_back(*it);
	}//for

	//------------------
	//	do last output for rest of output devices
	//	condition : the end of ods array 
	//------------------
	if((errCode = skeletonSwitcher(previousKind,subBuffer,expr,os)) != STG_SUCCESS)
		return errCode;												//	appendence mode

	return STG_SUCCESS;
}

