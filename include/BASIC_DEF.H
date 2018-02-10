//***************************************************************************************//
//***************************************************************************************//
//*                                                                    copyright @      *//
//*        Automation and Production Lab (APL)           Prof. Dr.-Ing. Wei-Tai Lei     *//
//*        Power Mechanical Engineering , National Tsing Hua University  (PME, NTHU)    *//
//*                                                                                     *//
//***************************************************************************************//
//***************************************************************************************//

//*****************************************************************************************
// FILENAME: BASIC_DEF.H
// EDITOR: Meng-Pei Sung
// DATE: 12th SETP 2005
// 
//*****************************************************************************************

#ifndef _BASIC_DEF_H
#define _BASIC_DEF_H


#define UNS08	    unsigned char
#define UNS16	    unsigned short
#define UNS32	    unsigned int
#define UNS64		unsigned long long		//	for windows system , Hsien 2012.03.08

#define SGN08	    char
#define SGN16	    short
#define SGN32	    int
#define SGN64		long long				// correct definiton for windows system , Hsien 2012.03.08

#define REAL32	       float
#define REAL64	       double

#define BOOLEAN        bool

#define STR            char*
#define BUFFER         char*

#ifndef NULL
#define	NULL           0x00000000L
#endif 

//*****************************************************************************************
// basic constant
//*****************************************************************************************
#define FALSE	       0
#define TRUE	       1

#define OFF	           0
#define ON	           1

#define OKS	           1
#define RETURN_OK      OKS

#define SYSERROR       -1
#define DECERROR       -2
#define CDNERROR       -3
#define CDPERROR       -4
#define GEOERROR       -5
#define PCLERROR       -6
#define PLCERROR       -7
#define VARERROR       -10


#define FLOAT_MAX         3.402823466e+38F        /* max value */

#define PI	           3.14159265358979323846
#define PI_HALF        PI / 2.0
#define ONEHALF_PI     1.5 * PI
#define TWO_PI	       2.0 * PI
#define PI_05	       0.5 * PI
#define PI_10	       PI
#define PI_15	       1.5 * PI
#define PI_20	       2.0 * PI

#define DEG2PI	       PI / 180.0
#define PI2DEG         1.0 / DEG2PI

#define INCH_TO_MM     25.4
#define MM_TO_INCH     1.0/INCH_TO_MM


#define	UNDEFINED_CMD		-1
#define	UNDEFINED_FB		-2
#define	UNEXPECTED_PROCESS	-3

#endif


