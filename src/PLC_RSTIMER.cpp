//---------------------------------------------------------------------------------------------------------------------------------
// FILENAME			: PLC_RSTIMER.cpp
// ORIGIN AUTHOR	: Hsien, 2012.06.14
// EDITOR			: Hsien
// NOTES			: generic timer routine services
//						it's able to be applied on Regular/Non-stop , one-shot/modal timer
//						all numeric are represented in incremental , each incremental represent ms.
//						instruction:	1. link correponding state-control frames with linkTimerEn(),linkTimerRun(),linkTimerGoal()
//													these frames represented as control bits.
//										2. link	output bits frame with tickTimer() , it would tick timer and output according to 
//													settings of enable , running value , goalvalue bits.
//										3. then initialize timer by timerInit()
//										4. running routine services in such order:
//														a. enable frame
//														b. running value frame
//														c. goal value frame
//														d. output value frame
//	how are you
//-----------------------------------------------------------------------------------------------------------------------------------
#include<PLC_RSTIMER.H>

enum rstime_enum
{
	ENABLE,
	RUNNING,
	GOAL,
	OUTPUT,
	MAX_FRAME_NUM,
};

const static unsigned char bitMasks[] = {
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80,
};

static size_t	cycTime;						// unit in ms
static FRAME*	timerFrames[MAX_FRAME_NUM];

void	timerInit(const size_t _cycTime)
{
	cycTime = _cycTime;
}

int		linkTimerEn		(FRAME* enableBits)
{
	//---------------------------------
	//	link only , no further handling
	//---------------------------------
	timerFrames[ENABLE] = enableBits;

	return 1;
}

int		linkTimerRun	(FRAME*	runningValues)
{
	timerFrames[RUNNING] = runningValues;

	return 1;
}

int		linkTimerGoal	(FRAME* goalValues)
{
	timerFrames[GOAL] = goalValues;

	return 1;
}

int		tickTimer		(FRAME* outBit)
{
	//-----------------------------------------------
	//	Both for non-stop , regular timer
	//-----------------------------------------------
	static size_t deviceIndex , byteIndex , bitIndex;

	deviceIndex = 0;		// bug , device is not considering into iterating
	
	//----------------------------------
	//	iterating by byte , then by bits , commented by Hsien , 2013.01.14
	//----------------------------------
	for(byteIndex	= 0; byteIndex <timerFrames[ENABLE]->frmSize;byteIndex++){
		for(bitIndex	= 0;bitIndex < sizeof(bitMasks)/sizeof(unsigned char);bitIndex++){
			//------------------------------------
			//	accessing every single enable bits
			//------------------------------------
			if((timerFrames[ENABLE])->baseAdr[byteIndex] & bitMasks[bitIndex]){
				//-------------------------------------------
				//	if timer enabled , counting and comparing
				//-------------------------------------------
				*((unsigned short*)(frmQueryAdr(timerFrames[RUNNING],sizeof(unsigned short),deviceIndex)))
					+= (unsigned short)cycTime;							

				//--------------------------------------------------------------------
				//	check if running value over the goal value
				//		if yes , setting output bit as true , then reset running value
				//--------------------------------------------------------------------
				if(*((unsigned short*)(frmQueryAdr(timerFrames[RUNNING],sizeof(unsigned short),deviceIndex)))
					>= *((unsigned short*)(frmQueryAdr(timerFrames[GOAL],sizeof(unsigned short),deviceIndex)))){
						outBit->baseAdr[byteIndex] |= bitMasks[bitIndex];											// set output bit
						*((unsigned short*)(frmQueryAdr(timerFrames[RUNNING],sizeof(unsigned short),deviceIndex)))	// reset running value
							= 0 ;	
				}
				else
					outBit->baseAdr[byteIndex] &= ~bitMasks[bitIndex];				// reset output bit
																					// bug fixed by Hsien , 2012.06.28
			}
			else{
				//-------------------------------------------------------------------------------
				//	if timer disabled , clear target timer  
				//		, and output bits  , Hsien , 2013.01.14
				//-------------------------------------------------------------------------------
				*((unsigned short*)(frmQueryAdr(timerFrames[RUNNING],sizeof(unsigned short),deviceIndex)))
					= 0;
				outBit->baseAdr[byteIndex] &= ~bitMasks[bitIndex];				// reset output bit
																				// Hsien , 2013.01.14
			}

			++deviceIndex;	// counting device index

		}//inner for
	}//outer for

	return 1;
}
