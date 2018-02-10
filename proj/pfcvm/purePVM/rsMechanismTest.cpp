#include<PLC_FRAME.h>

int rs1(FRAME* frm)
{
	for(size_t i=0;i<frm->frmSize;i++)
		frm->baseAdr[i] = 128;				// set all value = 128

	return 1;
}

int rs2(FRAME* frm)
{
	for(size_t i=0;i<frm->frmSize;i++)
		frm->baseAdr[i] += i;

	return 1;
}

int main()
{
	int res = 1;
	FRAME*	frm = frmAllocate(1,4*(sizeof(int)));

	frmAddRs(frm,rs1);
	frmRunRs(frm);		// worked fine , Hsien , 2012.06.12
						// using debug->windows->memory to monitor whole memory's status

	frmAddRs(frm,rs2);
	
	for(int i=0;i<5;i++)
		res = frmAddRs(frm,rs2);
	frmRunRs(frm);
	frmRemoveRs(frm);
	frmRunRs(frm);

			// ok, all mechanism worked well , 2012.06.12
	return 0;
}