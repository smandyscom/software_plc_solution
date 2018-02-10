#include<STC_ASM.h>

#define FILENAME_INPUT	1
#define FILENAME_OUTPUT	2

int main(int argc, char * argv[])
{
	FILE* fp_i,*fp_o;
//	char filename_i[] = "C:\\Users\\smandyscom\\Desktop\\st files\\interlock_example1.st";
//	char filename_o[] = "C:\\Users\\smandyscom\\Desktop\\test.irc";

	if(argv[FILENAME_INPUT] && argv[FILENAME_OUTPUT]){
		fp_i = fopen(argv[FILENAME_INPUT],"r");
		fp_o = fopen(argv[FILENAME_OUTPUT],"w");
	}
	else{
		fprintf(stderr,"error: no file input");
		return 0;
	}
	if((!fp_i) || (!fp_o)){
		fprintf(stderr,"error: file not found");
		return 0;
	}

	stCompilerInit("D:\\workdir\\vs2005projects\\PLC  framework\\resource\\PLC.DB");
	if(!stCompiler(fp_i,fp_o)){
		fprintf(stdout,"compiling failed.");
	}
	else
		fprintf(stdout,"compiling ok.");

	fclose(fp_o);
	fclose(fp_i);

	return 0;
}