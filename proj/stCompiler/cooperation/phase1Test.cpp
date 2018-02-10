#include<stdio.h>
#include<limits.h>
#include<time.h>

#define DB_NAME "D:\\workdir\\vs2005projects\\PLC  framework\\resource\\PLC.DB"

#include<PLC_IRC.h>
#include<PLC_STCASM.h>

#define RUN_VM 1

#if RUN_VM
#include<PLC_PFC.h>
#include<PLC_PFCVM.h>
#include<PLC_FCLCT.H>

#define PFC_VIRTUAL_MACHINE 1

extern int initialize(void);
extern void*	vmExecute(void*	base);
#endif

int main()
{
	FILE* fpin,*fpout , *logOfInput , *logOfOutput , *log;

	fpin	= tmpfile();
	fpout	= tmpfile();

	logOfOutput = fopen("compiler_output.log","a+"); // appendence mode
	logOfInput  = fopen("compiler_intput.log","a+");
	log = fopen("compiler.log","a+");

	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);

	fprintf(logOfOutput,"\n%s\n",asctime(timeinfo));
	fprintf(logOfInput,"\n%s\n",asctime(timeinfo));
	fprintf(log,"\n%s\n<input>\n",asctime(timeinfo));

	//---------------------------------
	//	output ST statements to log file
	//---------------------------------
	char c;
	while(!feof(stdin)){
		fread(&c,sizeof(char),1,stdin);
		fwrite(&c,sizeof(char),1,fpin);
		fwrite(&c,sizeof(char),1,logOfInput);
		fwrite(&c,sizeof(char),1,log);
	}
	rewind(fpin);

	fprintf(log,"<output>\n");
	if(!stCompiler_init(DB_NAME)){
		fprintf(log,"DB initialization failed\n");
		return 0;
	}
	if(!stCompiler(fpin,fpout)){
		fprintf(log,"compiling error\n");
		return 0;
	}

	init_irc_fprintf();
	//-------------------------------------------------------------------
	//	recover IRC information from format of file into internal format
	//-------------------------------------------------------------------
	IRC_UNIT irc_instruction[UCHAR_MAX];
	memset(irc_instruction,0,sizeof(irc_instruction));
	rewind(fpout);
	fread(irc_instruction,sizeof(IRC_UNIT),UCHAR_MAX,fpout);

	//-------------------------------------------------------------------
	//	output IRC code stream to log file
	//-------------------------------------------------------------------
	int i=-1;
	do{
		++i;
//		irc_fprintf(logOfOutput,irc_instruction[i]);
		irc_fprintf(log,irc_instruction[i]);
	}while(irc_instruction[i].type != _PROGCTRL
		|| (irc_instruction[i].progCtrlData.ctrlCmd != _PROG_END));	// condition corrected , Hsien , 2012.04.24

	fclose(fpin);
	fclose(fpout);
	fclose(logOfOutput);
	fclose(logOfInput);
	fclose(log);

#if RUN_VM

	if(!initialize())
		return 0;
	PFC_UNIT pfc_instruction[UCHAR_MAX];
	memset(pfc_instruction,0,sizeof(pfc_instruction));

	i=-1;
	do{
		++i;
		irc2pfc((void*)&pfc_instruction[i],irc_instruction[i]);
	}while(irc_instruction[i].type != _PROGCTRL
		|| (irc_instruction[i].progCtrlData.ctrlCmd != _PROG_END));	// condition corrected , Hsien , 2012.04.24

	short* y0	= (short*)FCLCT::QueryAddress(2,sizeof(short),0);	// debugging use
	short* x000	= (short*)FCLCT::QueryAddress(1,sizeof(short),0);

	vmExecute(pfc_instruction);

#endif

	return 0;
}