#ifndef _LD_ITEMCODES_H
#define _LD_ITEMCODES_H
namespace plain
{
	public enum class inpItemCodes
	{
	//------------------------
	//	adding : input_devices
	//------------------------
	BF_FORWARD,		// branch forward , seems redundant
	BF_TERMINAL,
	BF_TSECTION,
	BUF_T1,	// branch up-forward
	BUF_T2,
	BD_TERMINAL,		// branch downward
	BD_TSECTION,
	ST,		// short , added by Hsien , 2012.06.04
		//-----------------------
		//	for constant inp area
		//-----------------------
	ST_HEAD,
	ST_TAIL,
	ST_TAIL_TSECTION,		// to dedicate last inp column's outlook
	E_VERTICAL,
	E_NOTHING,
	E_EXTENSION,
		
	MAX_IMAGE_NUM,
	//------------------------
	//	input_devices
	//------------------------
	NO,		// normal open
	NC,		// normal close	
	//--------------
	//	delete/reset
	//--------------
	DELETE,
//	NO_ACTION,
	};

	public enum class outItemCodes
	{
	//--------------
	//	delete/reset
	//--------------
	DELETE,
	E_LINE,
	//------------------------
	//	adding : input_devices
	//------------------------
	COIL,				// for bit control
	SET,				// for multibit integer control
	};


}// namespace

#endif