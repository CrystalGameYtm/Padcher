#pragma once
#include "MultiPatch.h" // ϳ�������� MultiPatch ��� ������ � ������� �������
#include "bps_patcher.h"  // ���� �������� ��� BPS
#include "flips.hpp"      // �������� ��� IPS
#include "asm_patcher.h" // ���� �������� ��� ASM ������
#include <msclr/marshal_cppstd.h>
#include <fstream> // ��� ������� ����� ����������

namespace Patcher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Security::Cryptography;
	using namespace System::Text;

	// �������� ������� ��� ������� ����� � �������� ������
	byte_vector read_file_native(const std::string& filepath) {
		std::ifstream file(filepath, std::ios::binary | std::ios::ate);
		if (!file) {
			throw std::runtime_error("�� ������� ������� ����: " + filepath);
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		byte_vector buffer(size);
		if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
			throw std::runtime_error("�� ������� ��������� ����: " + filepath);
		}
		return buffer;
	}

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			// �������� ��������
			ClearChecksumLabels();
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textBox1;
	protected:
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::TextBox^ textBox3;
	private: System::Windows::Forms::Button^ opensave;
	private: System::Windows::Forms::Button^ openpatch;
	private: System::Windows::Forms::Button^ openrom;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::CheckBox^ ignoreChecksumsCheckbox;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ sha1Label;
	private: System::Windows::Forms::Label^ md5Label;
	private: System::Windows::Forms::Label^ crcLabel;
	private: System::Windows::Forms::Label^ labelRomSha1;
	private: System::Windows::Forms::Label^ labelRomMd5;
	private: System::Windows::Forms::Label^ labelRomCrc32;

	private: System::Windows::Forms::CheckBox^ checkmultipatch;
	private: System::Windows::Forms::Button^ button2;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->opensave = (gcnew System::Windows::Forms::Button());
			this->openpatch = (gcnew System::Windows::Forms::Button());
			this->openrom = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->ignoreChecksumsCheckbox = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->sha1Label = (gcnew System::Windows::Forms::Label());
			this->md5Label = (gcnew System::Windows::Forms::Label());
			this->crcLabel = (gcnew System::Windows::Forms::Label());
			this->labelRomSha1 = (gcnew System::Windows::Forms::Label());
			this->labelRomMd5 = (gcnew System::Windows::Forms::Label());
			this->labelRomCrc32 = (gcnew System::Windows::Forms::Label());
			this->checkmultipatch = (gcnew System::Windows::Forms::CheckBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->textBox1->Location = System::Drawing::Point(12, 41);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(439, 26);
			this->textBox1->TabIndex = 0;
			// 
			// textBox2
			// 
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->textBox2->Location = System::Drawing::Point(12, 105);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(439, 26);
			this->textBox2->TabIndex = 2;
			// 
			// textBox3
			// 
			this->textBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->textBox3->Location = System::Drawing::Point(12, 169);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(439, 26);
			this->textBox3->TabIndex = 4;
			// 
			// opensave
			// 
			this->opensave->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->opensave->Location = System::Drawing::Point(457, 169);
			this->opensave->Name = L"opensave";
			this->opensave->Size = System::Drawing::Size(42, 26);
			this->opensave->TabIndex = 5;
			this->opensave->Text = L"...";
			this->opensave->UseVisualStyleBackColor = true;
			this->opensave->Click += gcnew System::EventHandler(this, &MyForm::opensave_Click);
			// 
			// openpatch
			// 
			this->openpatch->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->openpatch->Location = System::Drawing::Point(457, 105);
			this->openpatch->Name = L"openpatch";
			this->openpatch->Size = System::Drawing::Size(42, 27);
			this->openpatch->TabIndex = 3;
			this->openpatch->Text = L"...";
			this->openpatch->UseVisualStyleBackColor = true;
			this->openpatch->Click += gcnew System::EventHandler(this, &MyForm::openpatch_Click);
			// 
			// openrom
			// 
			this->openrom->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->openrom->Location = System::Drawing::Point(457, 41);
			this->openrom->Name = L"openrom";
			this->openrom->Size = System::Drawing::Size(42, 27);
			this->openrom->TabIndex = 1;
			this->openrom->Text = L"...";
			this->openrom->UseVisualStyleBackColor = true;
			this->openrom->Click += gcnew System::EventHandler(this, &MyForm::openrom_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(12, 82);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(99, 20);
			this->label1->TabIndex = 13;
			this->label1->Text = L"���� �����";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(12, 18);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(105, 20);
			this->label2->TabIndex = 10;
			this->label2->Text = L"���� ROM-�";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(12, 146);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(183, 20);
			this->label3->TabIndex = 12;
			this->label3->Text = L"�������� ���� (�����)";
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(349, 370);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(150, 42);
			this->button1->TabIndex = 8;
			this->button1->Text = L"����������";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// ignoreChecksumsCheckbox
			// 
			this->ignoreChecksumsCheckbox->AutoSize = true;
			this->ignoreChecksumsCheckbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			this->ignoreChecksumsCheckbox->Location = System::Drawing::Point(16, 210);
			this->ignoreChecksumsCheckbox->Name = L"ignoreChecksumsCheckbox";
			this->ignoreChecksumsCheckbox->Size = System::Drawing::Size(167, 20);
			this->ignoreChecksumsCheckbox->TabIndex = 6;
			this->ignoreChecksumsCheckbox->Text = L"���������� ��������";
			this->ignoreChecksumsCheckbox->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->sha1Label);
			this->groupBox1->Controls->Add(this->md5Label);
			this->groupBox1->Controls->Add(this->crcLabel);
			this->groupBox1->Controls->Add(this->labelRomSha1);
			this->groupBox1->Controls->Add(this->labelRomMd5);
			this->groupBox1->Controls->Add(this->labelRomCrc32);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->groupBox1->Location = System::Drawing::Point(12, 250);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(487, 100);
			this->groupBox1->TabIndex = 14;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"���������� ��� ����";
			// 
			// sha1Label
			// 
			this->sha1Label->AutoSize = true;
			this->sha1Label->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->sha1Label->Location = System::Drawing::Point(70, 70);
			this->sha1Label->Name = L"sha1Label";
			this->sha1Label->Size = System::Drawing::Size(91, 15);
			this->sha1Label->TabIndex = 5;
			this->sha1Label->Text = L"�� ���������";
			// 
			// md5Label
			// 
			this->md5Label->AutoSize = true;
			this->md5Label->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->md5Label->Location = System::Drawing::Point(70, 48);
			this->md5Label->Name = L"md5Label";
			this->md5Label->Size = System::Drawing::Size(91, 15);
			this->md5Label->TabIndex = 4;
			this->md5Label->Text = L"�� ���������";
			// 
			// crcLabel
			// 
			this->crcLabel->AutoSize = true;
			this->crcLabel->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->crcLabel->Location = System::Drawing::Point(70, 26);
			this->crcLabel->Name = L"crcLabel";
			this->crcLabel->Size = System::Drawing::Size(91, 15);
			this->crcLabel->TabIndex = 3;
			this->crcLabel->Text = L"�� ���������";
			// 
			// labelRomSha1
			// 
			this->labelRomSha1->AutoSize = true;
			this->labelRomSha1->Location = System::Drawing::Point(15, 70);
			this->labelRomSha1->Name = L"labelRomSha1";
			this->labelRomSha1->Size = System::Drawing::Size(49, 16);
			this->labelRomSha1->TabIndex = 2;
			this->labelRomSha1->Text = L"SHA-1:";
			// 
			// labelRomMd5
			// 
			this->labelRomMd5->AutoSize = true;
			this->labelRomMd5->Location = System::Drawing::Point(15, 48);
			this->labelRomMd5->Name = L"labelRomMd5";
			this->labelRomMd5->Size = System::Drawing::Size(38, 16);
			this->labelRomMd5->TabIndex = 1;
			this->labelRomMd5->Text = L"MD5:";
			// 
			// labelRomCrc32
			// 
			this->labelRomCrc32->AutoSize = true;
			this->labelRomCrc32->Location = System::Drawing::Point(15, 26);
			this->labelRomCrc32->Name = L"labelRomCrc32";
			this->labelRomCrc32->Size = System::Drawing::Size(52, 16);
			this->labelRomCrc32->TabIndex = 0;
			this->labelRomCrc32->Text = L"CRC32:";
			// 
			// checkmultipatch
			// 
			this->checkmultipatch->AutoSize = true;
			this->checkmultipatch->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->checkmultipatch->Location = System::Drawing::Point(397, 81);
			this->checkmultipatch->Name = L"checkmultipatch";
			this->checkmultipatch->Size = System::Drawing::Size(102, 24);
			this->checkmultipatch->TabIndex = 15;
			this->checkmultipatch->Text = L"MultiPatch";
			this->checkmultipatch->UseVisualStyleBackColor = true;
			this->checkmultipatch->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkmultipatch_CheckedChanged);
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->button2->Location = System::Drawing::Point(193, 371);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(150, 42);
			this->button2->TabIndex = 16;
			this->button2->Text = L"Open MultiPatch";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Gainsboro;
			this->ClientSize = System::Drawing::Size(511, 424);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->checkmultipatch);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->ignoreChecksumsCheckbox);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->openrom);
			this->Controls->Add(this->openpatch);
			this->Controls->Add(this->opensave);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"Padcher";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		// ������� ���������� ���� �� asar.exe
		String^ asarPath = Path::Combine(Application::StartupPath, "asar.exe");

		// ����������, �� ���� ���� �� ��� ������
		if (!File::Exists(asarPath)) {
			// ���� ����� ����, �������� ������������
			MessageBox::Show(
				"���� \"asar.exe\" �� �������� � ����� � ���������.\n\n"
				"������� �������� .asm ����� ���� ����������, ���� �� �� �������� asar.exe ����� �� ��� ��������.",
				"�����: ������� ���������",
				MessageBoxButtons::OK,
				MessageBoxIcon::Warning
			);

			// �����������: ����� �������� ������, ���� �� ������ ������� �����������
			// ������ �������� ��� asar. �� ������ UI ���� ���������.
			// button1->Enabled = false; 
			// labelStatus->Text = "������� (Asar �����������)";
		}
	}
	private: void ClearChecksumLabels() {
		crcLabel->Text = "���� �� �������";
		md5Label->Text = "���� �� �������";
		sha1Label->Text = "���� �� �������";
	}

	private: String^ ComputeHash(Stream^ stream, HashAlgorithm^ hasher) {
		array<Byte>^ hash = hasher->ComputeHash(stream);
		StringBuilder^ sb = gcnew StringBuilder(hash->Length * 2);
		for each(Byte b in hash) {
			sb->Append(b.ToString("x2"));
		}
		return sb->ToString()->ToUpper();
	}

	private: String^ ComputeCrc32(Stream^ stream) {
		array<Byte>^ buffer = gcnew array<Byte>(stream->Length);
		stream->Read(buffer, 0, (int)stream->Length);

		uint32_t crc = 0xFFFFFFFF;
		for each(Byte b in buffer) {
			crc ^= b;
			for (int j = 0; j < 8; j++) {
				uint32_t mask = -(crc & 1);
				crc = (crc >> 1) ^ (0xEDB88320 & mask);
			}
		}
		return (~crc).ToString("X8");
	}

	private: void UpdateChecksums(String^ filePath) {
		if (String::IsNullOrEmpty(filePath) || !File::Exists(filePath)) {
			ClearChecksumLabels();
			return;
		}

		try {
			FileStream^ fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::Read, FileShare::Read);

			// CRC32
			fs->Position = 0;
			crcLabel->Text = ComputeCrc32(fs);

			// MD5
			fs->Position = 0;
			md5Label->Text = ComputeHash(fs, MD5::Create());

			// SHA-1
			fs->Position = 0;
			sha1Label->Text = ComputeHash(fs, SHA1::Create());

			fs->Close();
		}
		catch (Exception^ ex) {
			crcLabel->Text = "�������";
			md5Label->Text = "�������";
			sha1Label->Text = "�������";
			MessageBox::Show("�� ������� ��������� ��������� ����:\n" + ex->Message, "�������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
	}

	private: System::Void openrom_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "All ROM Files|*.sfc;*.smc;*.gba;*.gb;*.gbc;*.nes;*.md|All files (*.*)|*.*";
		openFileDialog->Title = "������� ���� ROM-�";
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			textBox1->Text = openFileDialog->FileName;
			UpdateChecksums(openFileDialog->FileName);
		}
	}

	private: System::Void openpatch_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "All Supported Patches (*.ips, *.bps, *.asm)|*.ips;*.bps;*.asm|All files (*.*)|*.*";
		openFileDialog->Title = "������� ���� �����";
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			textBox2->Text = openFileDialog->FileName;
			String^ romPath = textBox1->Text;
			if (!String::IsNullOrEmpty(romPath) && File::Exists(romPath)) {
				String^ patchDir = Path::GetDirectoryName(openFileDialog->FileName);
				String^ patchNameWithoutExt = Path::GetFileNameWithoutExtension(openFileDialog->FileName);
				String^ romExtension = Path::GetExtension(romPath);
				textBox3->Text = Path::Combine(patchDir, patchNameWithoutExt + romExtension);
			}
		}
	}

	private: System::Void opensave_Click(System::Object^ sender, System::EventArgs^ e) {
		SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
		saveFileDialog->Filter = "All ROM Files|*.sfc;*.smc;*.gba;*.gb;*.gbc;*.nes;*.md|All files (*.*)|*.*";
		saveFileDialog->Title = "�������� ����������� ROM";
		if (!String::IsNullOrEmpty(textBox3->Text)) {
			try {
				saveFileDialog->InitialDirectory = Path::GetDirectoryName(textBox3->Text);
				saveFileDialog->FileName = Path::GetFileName(textBox3->Text);
			}
			catch (Exception^) {}
		}
		if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			textBox3->Text = saveFileDialog->FileName;
		}
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		ClearChecksumLabels();
		groupBox1->Text = "���������� ��� ���� (�������...)";
		this->Update();

		String^ romPath = textBox1->Text;
		String^ patchPath = textBox2->Text;
		String^ outputPath = textBox3->Text;

		if (String::IsNullOrWhiteSpace(romPath) || String::IsNullOrWhiteSpace(patchPath) || String::IsNullOrWhiteSpace(outputPath)) {
			MessageBox::Show("���� �����, �������� �� ����.", "�������", MessageBoxButtons::OK, MessageBoxIcon::Error);
			groupBox1->Text = "���������� ��� ����";
			return;
		}
		try {
			String^ extension = Path::GetExtension(patchPath)->ToLower();

			if (extension == ".asm") {
				File::Copy(romPath, outputPath, true);
				String^ asarPath = Path::Combine(Application::StartupPath, "asar.exe");

				if (!File::Exists(asarPath)) {
					throw gcnew System::IO::FileNotFoundException("�� �������� ���� asar.exe!");
				}

				// ������� �������� ������. ����� ��������!
				String^ commandLine = String::Format("\"{0}\" \"{1}\" \"{2}\"", asarPath, patchPath, outputPath);

				// ϳ�������� �� ������� �������
				STARTUPINFOW si = { sizeof(si) };
				PROCESS_INFORMATION pi = { 0 };

				// ���������� managed string � LPWSTR (����� �������, Unicode)
				wchar_t* lpCommandLine = (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(commandLine).ToPointer();

				// ��������� ������
				BOOL success = CreateProcessW(
					NULL,           // lpApplicationName - �� �������������, �������� ��� � �������� ������
					lpCommandLine,  // lpCommandLine - ����� ������� ��� �������
					NULL,           // lpProcessAttributes
					NULL,           // lpThreadAttributes
					FALSE,          // bInheritHandles
					0,              // dwCreationFlags
					NULL,           // lpEnvironment
					NULL,           // lpCurrentDirectory
					&si,            // lpStartupInfo
					&pi             // lpProcessInformation
				);

				// ��������� ���'���, ������� ��� ������
				System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr)lpCommandLine);

				if (!success) {
					throw gcnew Exception("�� ������� ��������� ������ asar.exe. ������� WinAPI: " + System::Runtime::InteropServices::Marshal::GetLastWin32Error());
				}

				// ������, ���� ������ Asar �����������
				WaitForSingleObject(pi.hProcess, INFINITE);

				// �������� ��� ���������� �������
				DWORD exitCode;
				GetExitCodeProcess(pi.hProcess, &exitCode);

				// ��������� ����������� �������
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				// ���������� ��� ����������
				if (exitCode != 0) {
					throw gcnew Exception("Asar �������� ��� �������. ��������� ��������� ���� ������ � ���������� ����� ��� �������� ����������.");
				}
			}

			else if (extension == ".ips" || extension == ".bps") {
				// --- ����� ��� IPS/BPS (��� ������ ���) ---
				std::string nativeRomPath = msclr::interop::marshal_as<std::string>(romPath);
				std::string nativePatchPath = msclr::interop::marshal_as<std::string>(patchPath);

				byte_vector source_vec = read_file_native(nativeRomPath);
				byte_vector patch_vec = read_file_native(nativePatchPath);
				byte_vector target_vec;

				if (extension == ".ips") {
					// ������������� flips ��� IPS
					std::string error = flips::apply_ips(patch_vec, source_vec, target_vec);
					if (!error.empty()) throw std::runtime_error(error);
				}
				else { // .bps
					// ������������� bps_patcher ��� BPS
					target_vec = apply_bps_patch(source_vec, patch_vec, !this->ignoreChecksumsCheckbox->Checked);
				}

				// �������� ��������� � ����
				array<Byte>^ targetManaged = gcnew array<Byte>(target_vec.size());
				System::Runtime::InteropServices::Marshal::Copy((IntPtr)target_vec.data(), targetManaged, 0, (int)target_vec.size());
				File::WriteAllBytes(outputPath, targetManaged);
			}
			else {
				throw gcnew NotSupportedException("�������������� ������ �����: " + extension);
			}

			groupBox1->Text = "���������� ��� ����� ����";
			UpdateChecksums(outputPath);
			MessageBox::Show("���� \"" + Path::GetFileName(patchPath) + "\" ������ �����������!", "����", MessageBoxButtons::OK, MessageBoxIcon::Information);

		}
		catch (const std::exception& ex) {
			String^ errorMessage = gcnew String(ex.what());
			MessageBox::Show("������� ������� C++:\n" + errorMessage, "�������", MessageBoxButtons::OK, MessageBoxIcon::Error);
			groupBox1->Text = "���������� ��� ���� (�������)";
		}
		catch (Exception^ ex) {
			MessageBox::Show("������� ������� .NET:\n" + ex->Message, "�������", MessageBoxButtons::OK, MessageBoxIcon::Error);
			groupBox1->Text = "���������� ��� ���� (�������)";
		}
	}
	private: System::Void checkmultipatch_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		bool isMultiPatchMode = this->checkmultipatch->Checked;

		// �������� ������ ��� MultiPatch � ������ ��� ���������� ��������
		this->button2->Enabled = isMultiPatchMode;  // ������ "Open MultiPatch"

		// �������� ������ ��� ���������� �������� � ������ ��� MultiPatch
		this->button1->Enabled = !isMultiPatchMode; // ������ "����������"

		// ����� ����� �������/�������� ��������, �� �� ������ � ����� MultiPatch
		this->textBox2->Enabled = !isMultiPatchMode;
		this->openpatch->Enabled = !isMultiPatchMode;
		this->label1->Enabled = !isMultiPatchMode;
		this->ignoreChecksumsCheckbox->Enabled = !isMultiPatchMode;
		this->textBox3->Enabled = !isMultiPatchMode;
		this->opensave->Enabled = !isMultiPatchMode;
		this->label3->Enabled = !isMultiPatchMode;
	};
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		
		String^ romPath = this->textBox1->Text;

		if (String::IsNullOrWhiteSpace(romPath)) {
			MessageBox::Show("���� �����, �������� ������� ���� ROM-�.", "�������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return; // �������� � ������, ���� ���� �������.
		}

			MultiPatch^ multiPatchForm = gcnew MultiPatch();
			multiPatchForm->InitialRomPath = romPath;
			this->Hide();
			multiPatchForm->ShowDialog();
			this->Show();
	

		};
	};
	}