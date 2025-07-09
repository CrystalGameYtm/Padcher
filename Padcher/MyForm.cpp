#include "MyForm.h" 
using namespace System;
using namespace System::Windows::Forms;

// Атрибут STAThread є обов'язковим для програм Windows Forms
[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Patcher::MyForm form;
	Application::Run(% form);

	return 0;
}

