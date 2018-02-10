#include<stdio.h>
#include<string.h>
#include<PLC_PVM.h>
//#define	FORMAT_COMMENT	"#%s['\n']"
//#define FORMAT_STR		"\[%d\]%s%d"
//#define FORMAT_SEG	"FILE=\"%s\""

//extern int procLoadByLine(const char*	_sourceLineStr
//				   ,int*		_level
//				   ,char*		_filenameStr
//				   ,char*		_pidStr);
//extern int	turnsIntoData();

//int main()
//{
//	FILE*	fp = fopen("test.txt","r");
//	char	strFirst[128];
//	char	str[64];
//	char	dontCare[64];
//	char	filename[64];
//	char	comment	[64];
//
//	int		level;
//	int		pid;
//	int		matchCount;
//
//
//	fgets(strFirst,32767,fp);
//	//fscanf(fp,"%s['\n']",strFirst);
//	matchCount = sscanf(strFirst,FORMAT_STR,&level/*,&dontCare*/,filename/*,dontCare*/,&pid);
//
//	fgets(strFirst,32767,fp);
//	matchCount = sscanf(strFirst,FORMAT_COMMENT,&comment);
//
//
//	return 1;
//}

//int main()
//{
//	FILE*	fp = fopen("test.txt","r");
//	char	lineBuffer[256];
//
//	int		__level;
//	char	__filename[128];
//	char	__pidStr[16];
//	int		returnValue;
//
//	while(fgets(lineBuffer,128,fp) != NULL){
//		returnValue = procLoadByLine(lineBuffer
//			,&__level
//			,__filename
//			,__pidStr);
//	}
//
//	return 0;
//}

extern int	startSponWithoutRs(void*);

int main()
{
	PVM	pvmInstance;
	FILE*	tabFp = fopen("textDir.txt","r");
	int		returnValue;

	pvmInitialize(&pvmInstance,15,startSponWithoutRs,NULL);

	//returnValue = pvmLoadFromTab(&pvmInstance,tabFp);

	returnValue = pvmLoadFromTab/*2*/(&pvmInstance,tabFp);

	return 0;
}