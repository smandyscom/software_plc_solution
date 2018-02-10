#include<LD_ELEMENTS.h>
public ref class inputControl : public System::Windows::Forms::UserControl
	{
	public:
		inputControl(void)
		{
			this->le = new ldElement();
		}
	protected:
		ldElement* le;
};

