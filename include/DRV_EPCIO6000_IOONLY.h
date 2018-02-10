//--------------------------
//
//--------------------------

#define CARD_INDEX			1
#define RIO_CLOCK_DIVIDER	20	// Sampling Rate (40MHz) / ( 2 * divider )


int getDigitalInp		(void* data	,const size_t size);
int setDigitalOut		(void* data	,const size_t size);
int getLio				(void* data , const size_t size);
int	setLio				(void* data , const size_t size);	// Hsien 2013/4/29

int closeMachine		(void);
int startMachine		(void);
int	resetMachine		(void);	// Hsien 2013/5/2
