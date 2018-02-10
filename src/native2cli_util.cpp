#include<string>
using namespace std;

using namespace System;
using namespace System::IO;



void MarshalString ( String ^ s, std::string& os ) 
{
	// from MSDN
   using namespace Runtime::InteropServices;
   const char* chars = 
	   (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os.clear();
   os.assign(chars);
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}
FILE*	fopenString(System::String^ filename,const char*	mode){
	//----------------------
	//	Utility Fucntion , for Cli String , and bounding mode
	//----------------------
	static FILE*	fp = NULL;
	std::string		filenameStd;

	MarshalString(filename,filenameStd);
	
	return fopen(filenameStd.c_str(),mode);
}


void cliDataWriter_inUCHAR(System::IO::BinaryWriter^ bw,void* dataAdr,size_t dataSize)
{
	//----------------------------------------------
	//	Write-in data , according to known data size
	//		write header of rungs
	//		attension , should following 'unsigned char'
	//		Hsien , 2012.06.01
	//----------------------------------------------
	static size_t i;
	for(i=0;i<dataSize;i++)
		bw->Write(((unsigned char*)dataAdr)[i]);
		// writer tested ok, Hsine , 2012.06.01
}

void cliDataReader_inUCHAR(System::IO::BinaryReader^ br,void* dataCtn,size_t dataSize)
{
	//----------------------------------------------
	//	Read-out data , according to known data size
	//----------------------------------------------
	static size_t i;
	for(i=0;i<dataSize;i++)
		((unsigned char*)dataCtn)[i] = br->ReadByte();
		// reader tested ok, Hsien , 2012.06.01
}