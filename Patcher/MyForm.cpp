#include "MyForm.h" 
using namespace System;
using namespace System::Windows::Forms;

// ������� STAThread � ����'������� ��� ������� Windows Forms
[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Patcher::MyForm form;
	Application::Run(% form);

	return 0;
}

