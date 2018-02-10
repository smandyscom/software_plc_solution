//---------------
//
//---------------

#define	VIR_INP_IMG_SIZE	16
#define	VIR_OUT_IMG_SIZE	16

int		virImageScanIn	(void* data,const size_t	dataSize);
int		virImageScanOut	(void* data,const size_t	dataSize);

int		virImageClean();
int		virStartScript();

extern char	virInpImage[VIR_INP_IMG_SIZE];
extern char	virOutImage[VIR_OUT_IMG_SIZE];
