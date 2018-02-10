//---------------------------------------------------------------------------------------------
//	FILENAME: ircv.cpp
//	Author: Hsien , 2012.06.11
//	Editor:
//	note: untility tool to verbose irc instructions in human-readable format.
//			1. in debug mode , you can put filename on properties->debugging->command arguments
//---------------------------------------------------------------------------------------------
#include<PLC_IRC.h>
#include<stdio.h>
#include<limits.h>

#define FILENAME 1

int main( int argc, char * argv[])
{
	FILE* fp;
	size_t	  ircInstructionCounts;
	IRC_UNIT* ircInstruction;
	int i=0,ircSize;

	//char filename[] = "C:\\Users\\smandyscom\\Desktop\\test.irc";		//debug usage

	if(argv[1])
//		fp= fopen(argv[1],"r");
		fp= fopen(argv[1],"rb+");

	else{
		fprintf(stderr,"error: no file input");
		return 0;
	}
	if(!fp){
		fprintf(stderr,"error: file not found");
		return 0;
	}
	fclose(fp);

	ircFprintfInit();
	//-------------------------------------------------------------------
	//	recover IRC information from format of file into internal format
	//-------------------------------------------------------------------
	ircInstructionCounts 
		= ircFopen(argv[1],&ircInstruction);
	//-------------------------------------------------------------------
	//	output IRC code stream to log file
	//-------------------------------------------------------------------
	for(i=0;i<ircInstructionCounts;i++)
		ircFprintf(stdout,ircInstruction[i]);

	return 0;
}