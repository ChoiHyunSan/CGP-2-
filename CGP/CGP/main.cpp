////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;

	// Initialize and run the system object.
	result = SystemClass::GetInst()->Initialize();
	if(result)
	{
		SystemClass::GetInst()->Run();
	}

	// Shutdown and release the system object.
	SystemClass::GetInst()->Shutdown();

	return 0;
}