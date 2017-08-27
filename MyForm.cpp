#using <mscorlib.dll>
#include "MyForm.h"
#include "windows.h"
#include <tchar.h>

#using <System.dll>
using namespace System;
#using <System.Windows.Forms.dll>
using namespace System::Windows::Forms;


// This is the entry point for this application
// int _tmain(void)
// Changed to suppress console

// STAThreadAttribute needed for OpenFileDialog
[System::STAThreadAttribute]
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	VisInjector::MyForm form;
	//form.ShowDialog();
	Application::Run(%form);

	return 0;
}