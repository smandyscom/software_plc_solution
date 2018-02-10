#include<HS_MECH.h>		

#define SIMU_STATUS_RUN_ONCE		0
#define SIMU_STATUS_RUN_BREAK		1
#define SIMU_STATUS_SIGNAL_WAIT		2		// in
#define	SIMU_STATUS_INFO_WRITE_OUT	3		// the pending state when simu try to write info out

#define SIMU_SIGNAL_STEP	0x01	
#define SIMU_SIGNAL_START	0x02
#define SIMU_SIGNAL_STOP	0x03
#define SIMU_SIGNAL_NULL	0x04

typedef struct simu_command_data_st			SIMU_DATA_COMMAND;
typedef struct simu_information_data_st		SIMU_DATA_INFORMATION;
typedef struct simu_import_signal_st		SIMU_IMPORT_SIGNAL;
typedef struct simu_export_signal_st		SIMU_EXPORT_SIGNAL;

struct simu_command_data_st
{
	int		simuSignal;				//	signal
	//int		simuPid;
	int		breakInsTable[16];		//	indicate which instruction should be stopped in automative mode
									//	limited number of break points
};

struct simu_information_data_st
{
	int		simuState;				// indicate state of simulator
	int		simuDebugInfo;			// indicate the data content read from instruction
	//int		simuPid;
};


struct simu_import_signal_st
{
	HS_SIGNAL			hsSignal;			// inheritant from HS_MECH , for offering handshake mechanism
	SIMU_DATA_COMMAND	command;
};

struct simu_export_signal_st
{
	HS_SIGNAL				hsSignal;			// inheritant from HS_MECH , for offering handshake mechanism
	SIMU_DATA_INFORMATION	information;
};

#define SIMU_PROC_PID	1					// the fixed pid