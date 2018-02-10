

	//--------------------------------
	//	The Command to Set/Reset Value
	//--------------------------------
#define	VIR_VALUE_SET	0x00
#define	VIR_VALUE_RESET	0x01

#define	VIR_ONE_SHOT_PRESS_DOWN	VIR_VALUE_SET
#define	VIR_ONE_SHOT_PRESS_UP	VIR_VALUE_RESET

//---------------
//	The Formattion in C++/CLI language
//---------------
#define	VIR_BUTTON_DEVICE_NAME_FORMAT	"{0:G}{1:D2}.{2:D2}"
#define	VIR_IMAGE_NAME_IN_WORD			"{0:G}{1:D}"			// Hsien 2013/5/29