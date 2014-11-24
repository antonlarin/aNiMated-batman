#include "MainWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	DTestTask::MainWindow form;
	Application::Run(%form);
	return 0;
}