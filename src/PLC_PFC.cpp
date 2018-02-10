//---------------------------------------------------------------------------------------------------
// FILENAME			: PLC_PFC.h
// ORIGIN AUTHOR	: Hsien, 2012.03.29
// EDITOR			: Hsien
// NOTES			: PFC OPERAND UTILITIES:
//			basic method to manipulate PFC OPERAND , whether stored in main memory/instructions	
//		all value were represented in full-bits(32bits/64bits) , regardless its type in 
//		compiling time
//---------------------------------------------------------------------------------------------------
//		these things is inevitable binding with virtual machine , will combine with virtual-machine's document
//		commented by Hsien , 2012.04.19
//	how are you
//---------------------------------------------------------------------------------------------------
#include<PLC_PFC.h>

namespace BY_VALUE
{
	//--------------------------------------------------
	//	type regardless , bool/char/short/int are represented in 32 bits
	//	by 2's complement rule , adding for all of them could complished by binary adding
	//--------------------------------------------------
	SGN32	get	(SGN32*	data){
		return *data;
	}

	void	set	(SGN32* data,SGN32 value)
	{
		*data = value;	
	}
}

namespace BY_REFERENCE
{
	//---------------------------------------------------
	//	Drain data along address
	//---------------------------------------------------
	SGN32	getBool		(void*	data)
	{
		if((*(SGN08*)GET_ADDR(data))	& GET_BMSK(data))
			return true;
		else
			return false;
		//	return value , problem fixed. does it cost performance?
		//	, Hsien 2012.03.30
		//return (*(SGN08*)GET_ADDR(data))	& GET_BMSK(data) ; 
	}
	SGN32	getByte		(void*	data)
	{
		//----------------------------------------------------------
		// SGN08 -> SGN32(return value)
		//	conversion (type-cast) code were generated by compiler automatically
		//	Hsien , 2012.03.30
		//----------------------------------------------------------
		return	(*(SGN08*)GET_ADDR(data));
	}
	SGN32	getWord		(void*	data)
	{
		return (*(SGN16*)GET_ADDR(data));
	}
	SGN32	getDword	(void*	data)
	{
		return (*(SGN32*)GET_ADDR(data));
	}

	REAL32	getFloat	(void*	data)
	{
		return (*(REAL32*)GET_ADDR(data));
	}

	//---------------------------------------------------
	//	Drain data along address
	//---------------------------------------------------
	void	setBool		(void*	data,BOOLEAN value){
		if(value)
			(*(SGN08*)GET_ADDR(data))	|=	GET_BMSK(data);
		else
			(*(SGN08*)GET_ADDR(data))	&=	~(GET_BMSK(data));
	}

	void	setByte		(void*	data,SGN08	value){
		*(SGN08*)GET_ADDR(data) = value;
	}
	void	setWord		(void*	data,SGN16	value){
		*(SGN16*)GET_ADDR(data) = value;
	}
	void	setDword	(void*	data,SGN32	value){
		*(SGN32*)GET_ADDR(data) = value;
	}

	void	setFloat	(void*	data,REAL32	value){
		*(REAL32*)GET_ADDR(data) = value;
	}
}


