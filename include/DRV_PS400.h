//---------------------------------------------------------
// FILENAME			: DRV_PS400.cpp
// ORIGIN AUTHOR	: Hsien 2013/6/20
// EDITOR			: Hsien
// NOTES			: PLC_RSSTA suited interface from ICPDAP's motion controll board PS400
//					
//----------------------
int ps400Start				(void);
int ps400GetDigitalInp		(void* data	,const size_t size);
int ps400SetDigitalOut		(void* data	,const size_t size);
int	ps400Close				(void);