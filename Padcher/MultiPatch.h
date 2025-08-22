#pragma once
// --- Підключення необхідних файлів ---
#include "PatcherLogic.h"      // Містить ApplySinglePatch, read/write_file_native тощо
#include "bps_patcher.h"       // Містить apply_bps_patch та create_bps_patch
#include "PadcherPack.h"    // Містить класи для роботи з проєктом (створіть цей файл)

namespace Padcher {

	// --- Підключення необхідних просторів імен ---
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic; // Для List<>
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Security::Cryptography; // Для хешів
	using namespace msclr::interop;

	public ref class MultiPatch : public System::Windows::Forms::Form
	{
	public:
		property String^ InitialRomPath;

		MultiPatch(void)
		{
			InitializeComponent(); 
			dataGridView1->Columns->Clear();

			DataGridViewCheckBoxColumn^ checkColumn = gcnew DataGridViewCheckBoxColumn();
			checkColumn->Name = "Enabled";
			checkColumn->HeaderText = L"✔️";
			checkColumn->Width = 40;
			dataGridView1->Columns->Add(checkColumn);

			dataGridView1->Columns->Add("Num", "#");
			dataGridView1->Columns["Num"]->Width = 40;
			dataGridView1->Columns["Num"]->ReadOnly = true;

			dataGridView1->Columns->Add("PatchName", "Patch Name");
			dataGridView1->Columns["PatchName"]->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
			dataGridView1->Columns["PatchName"]->ReadOnly = true;

			DataGridViewComboBoxColumn^ typeColumn = gcnew DataGridViewComboBoxColumn();
			typeColumn->Name = "PatchType";
			typeColumn->HeaderText = "Type";
			typeColumn->Items->Add("Main");
			typeColumn->Items->Add("Optional");
			typeColumn->DefaultCellStyle->NullValue = "Main";
			typeColumn->Width = 100;
			dataGridView1->Columns->Add(typeColumn);

			dataGridView1->Columns->Add("FullPath", "Full Path");
			dataGridView1->Columns["FullPath"]->Visible = false;
			dataGridView1->Columns->Add("Status", "Status");
			this->AllowDrop = true;
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &MultiPatch::MultiPatch_DragEnter);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &MultiPatch::MultiPatch_DragDrop);

			currentProject = nullptr;
		}

	protected:
		~MultiPatch()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		// --- Змінні для керування проєктом ---
		PadcherPack^ currentProject;

	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::Button^ buttonAddPatches;
	private: System::Windows::Forms::Button^ buttonApplyPatches;
	private: System::Windows::Forms::CheckBox^ ignoreChecksumsCheckbox;
	private: System::Windows::Forms::Button^ buttonSelectOutput;
	private: System::Windows::Forms::TextBox^ outputPathTextBox;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Label^ sha1Label;
	private: System::Windows::Forms::Label^ md5Label;
	private: System::Windows::Forms::Label^ crcLabel;
	private: System::Windows::Forms::Label^ labelRomSha1;
	private: System::Windows::Forms::Label^ labelRomMd5;
	private: System::Windows::Forms::Label^ labelRomCrc32;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ buttonClearList;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ buttonLoadProject;
	private: System::Windows::Forms::TextBox^ projectPathTextBox;



	private: System::Windows::Forms::Button^ buttonSaveProject;


	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			   this->buttonAddPatches = (gcnew System::Windows::Forms::Button());
			   this->buttonApplyPatches = (gcnew System::Windows::Forms::Button());
			   this->ignoreChecksumsCheckbox = (gcnew System::Windows::Forms::CheckBox());
			   this->buttonSelectOutput = (gcnew System::Windows::Forms::Button());
			   this->outputPathTextBox = (gcnew System::Windows::Forms::TextBox());
			   this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			   this->sha1Label = (gcnew System::Windows::Forms::Label());
			   this->md5Label = (gcnew System::Windows::Forms::Label());
			   this->crcLabel = (gcnew System::Windows::Forms::Label());
			   this->labelRomSha1 = (gcnew System::Windows::Forms::Label());
			   this->labelRomMd5 = (gcnew System::Windows::Forms::Label());
			   this->labelRomCrc32 = (gcnew System::Windows::Forms::Label());
			   this->label3 = (gcnew System::Windows::Forms::Label());
			   this->buttonClearList = (gcnew System::Windows::Forms::Button());
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->buttonLoadProject = (gcnew System::Windows::Forms::Button());
			   this->projectPathTextBox = (gcnew System::Windows::Forms::TextBox());
			   this->buttonSaveProject = (gcnew System::Windows::Forms::Button());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			   this->groupBox1->SuspendLayout();
			   this->SuspendLayout();
			   // 
			   // dataGridView1
			   // 
			   this->dataGridView1->AllowUserToAddRows = false;
			   this->dataGridView1->AllowUserToDeleteRows = false;
			   this->dataGridView1->AllowUserToResizeRows = false;
			   this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			   this->dataGridView1->Location = System::Drawing::Point(12, 12);
			   this->dataGridView1->MultiSelect = false;
			   this->dataGridView1->Name = L"dataGridView1";
			   this->dataGridView1->ReadOnly = true;
			   this->dataGridView1->RowHeadersVisible = false;
			   this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			   this->dataGridView1->Size = System::Drawing::Size(660, 295);
			   this->dataGridView1->TabIndex = 0;
			   // 
			   // buttonAddPatches
			   // 
			   this->buttonAddPatches->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonAddPatches->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			   this->buttonAddPatches->Location = System::Drawing::Point(522, 313);
			   this->buttonAddPatches->Name = L"buttonAddPatches";
			   this->buttonAddPatches->Size = System::Drawing::Size(150, 23);
			   this->buttonAddPatches->TabIndex = 2;
			   this->buttonAddPatches->Text = L"Add Patches...";
			   this->buttonAddPatches->UseVisualStyleBackColor = true;
			   this->buttonAddPatches->Click += gcnew System::EventHandler(this, &MultiPatch::buttonAddPatches_Click);
			   // 
			   // buttonApplyPatches
			   // 
			   this->buttonApplyPatches->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonApplyPatches->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
			   this->buttonApplyPatches->Location = System::Drawing::Point(525, 497);
			   this->buttonApplyPatches->Name = L"buttonApplyPatches";
			   this->buttonApplyPatches->Size = System::Drawing::Size(150, 42);
			   this->buttonApplyPatches->TabIndex = 4;
			   this->buttonApplyPatches->Text = L"Export Project to BPS...";
			   this->buttonApplyPatches->UseVisualStyleBackColor = true;
			   this->buttonApplyPatches->Click += gcnew System::EventHandler(this, &MultiPatch::buttonApplyPatches_Click);
			   // 
			   // ignoreChecksumsCheckbox
			   // 
			   this->ignoreChecksumsCheckbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			   this->ignoreChecksumsCheckbox->AutoSize = true;
			   this->ignoreChecksumsCheckbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			   this->ignoreChecksumsCheckbox->Location = System::Drawing::Point(12, 424);
			   this->ignoreChecksumsCheckbox->Name = L"ignoreChecksumsCheckbox";
			   this->ignoreChecksumsCheckbox->Size = System::Drawing::Size(137, 20);
			   this->ignoreChecksumsCheckbox->TabIndex = 3;
			   this->ignoreChecksumsCheckbox->Text = L"Ignore Checksums";
			   this->ignoreChecksumsCheckbox->UseVisualStyleBackColor = true;
			   // 
			   // buttonSelectOutput
			   // 
			   this->buttonSelectOutput->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonSelectOutput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold));
			   this->buttonSelectOutput->Location = System::Drawing::Point(630, 394);
			   this->buttonSelectOutput->Name = L"buttonSelectOutput";
			   this->buttonSelectOutput->Size = System::Drawing::Size(42, 26);
			   this->buttonSelectOutput->TabIndex = 1;
			   this->buttonSelectOutput->Text = L"...";
			   this->buttonSelectOutput->UseVisualStyleBackColor = true;
			   this->buttonSelectOutput->Click += gcnew System::EventHandler(this, &MultiPatch::buttonSelectOutput_Click);
			   // 
			   // outputPathTextBox
			   // 
			   this->outputPathTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->outputPathTextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->outputPathTextBox->Location = System::Drawing::Point(16, 392);
			   this->outputPathTextBox->Name = L"outputPathTextBox";
			   this->outputPathTextBox->ReadOnly = true;
			   this->outputPathTextBox->Size = System::Drawing::Size(612, 26);
			   this->outputPathTextBox->TabIndex = 0;
			   this->outputPathTextBox->Text = L"(This field is for single build rom output)";
			   // 
			   // groupBox1
			   // 
			   this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->groupBox1->Controls->Add(this->sha1Label);
			   this->groupBox1->Controls->Add(this->md5Label);
			   this->groupBox1->Controls->Add(this->crcLabel);
			   this->groupBox1->Controls->Add(this->labelRomSha1);
			   this->groupBox1->Controls->Add(this->labelRomMd5);
			   this->groupBox1->Controls->Add(this->labelRomCrc32);
			   this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			   this->groupBox1->Location = System::Drawing::Point(12, 450);
			   this->groupBox1->Name = L"groupBox1";
			   this->groupBox1->Size = System::Drawing::Size(510, 100);
			   this->groupBox1->TabIndex = 18;
			   this->groupBox1->TabStop = false;
			   this->groupBox1->Text = L"File Information (for selected Output File)";
			   // 
			   // sha1Label
			   // 
			   this->sha1Label->AutoSize = true;
			   this->sha1Label->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F));
			   this->sha1Label->Location = System::Drawing::Point(70, 70);
			   this->sha1Label->Name = L"sha1Label";
			   this->sha1Label->Size = System::Drawing::Size(14, 15);
			   this->sha1Label->TabIndex = 5;
			   this->sha1Label->Text = L"-";
			   // 
			   // md5Label
			   // 
			   this->md5Label->AutoSize = true;
			   this->md5Label->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F));
			   this->md5Label->Location = System::Drawing::Point(70, 48);
			   this->md5Label->Name = L"md5Label";
			   this->md5Label->Size = System::Drawing::Size(14, 15);
			   this->md5Label->TabIndex = 4;
			   this->md5Label->Text = L"-";
			   // 
			   // crcLabel
			   // 
			   this->crcLabel->AutoSize = true;
			   this->crcLabel->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F));
			   this->crcLabel->Location = System::Drawing::Point(70, 26);
			   this->crcLabel->Name = L"crcLabel";
			   this->crcLabel->Size = System::Drawing::Size(14, 15);
			   this->crcLabel->TabIndex = 3;
			   this->crcLabel->Text = L"-";
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
			   // label3
			   // 
			   this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			   this->label3->AutoSize = true;
			   this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->label3->Location = System::Drawing::Point(12, 369);
			   this->label3->Name = L"label3";
			   this->label3->Size = System::Drawing::Size(87, 20);
			   this->label3->TabIndex = 13;
			   this->label3->Text = L"Output File";
			   // 
			   // buttonClearList
			   // 
			   this->buttonClearList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonClearList->Location = System::Drawing::Point(396, 313);
			   this->buttonClearList->Name = L"buttonClearList";
			   this->buttonClearList->Size = System::Drawing::Size(119, 23);
			   this->buttonClearList->TabIndex = 19;
			   this->buttonClearList->Text = L"Clear List";
			   this->buttonClearList->UseVisualStyleBackColor = true;
			   this->buttonClearList->Click += gcnew System::EventHandler(this, &MultiPatch::buttonClearList_Click);
			   // 
			   // label1
			   // 
			   this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			   this->label1->AutoSize = true;
			   this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->label1->Location = System::Drawing::Point(12, 317);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(114, 20);
			   this->label1->TabIndex = 22;
			   this->label1->Text = L"PatchPack File";
			   // 
			   // buttonLoadProject
			   // 
			   this->buttonLoadProject->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonLoadProject->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
			   this->buttonLoadProject->Location = System::Drawing::Point(522, 342);
			   this->buttonLoadProject->Name = L"buttonLoadProject";
			   this->buttonLoadProject->Size = System::Drawing::Size(100, 26);
			   this->buttonLoadProject->TabIndex = 21;
			   this->buttonLoadProject->Text = L"Load Project...";
			   this->buttonLoadProject->UseVisualStyleBackColor = true;
			   this->buttonLoadProject->Click += gcnew System::EventHandler(this, &MultiPatch::buttonLoadProject_Click);
			   // 
			   // projectPathTextBox
			   // 
			   this->projectPathTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				   | System::Windows::Forms::AnchorStyles::Right));
			   this->projectPathTextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->projectPathTextBox->Location = System::Drawing::Point(16, 340);
			   this->projectPathTextBox->Name = L"projectPathTextBox";
			   this->projectPathTextBox->ReadOnly = true;
			   this->projectPathTextBox->Size = System::Drawing::Size(500, 26);
			   this->projectPathTextBox->TabIndex = 20;
			   // 
			   // buttonSaveProject
			   // 
			   this->buttonSaveProject->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonSaveProject->Location = System::Drawing::Point(628, 342);
			   this->buttonSaveProject->Name = L"buttonSaveProject";
			   this->buttonSaveProject->Size = System::Drawing::Size(44, 26);
			   this->buttonSaveProject->TabIndex = 25;
			   this->buttonSaveProject->Text = L"Save";
			   this->buttonSaveProject->UseVisualStyleBackColor = true;
			   this->buttonSaveProject->Click += gcnew System::EventHandler(this, &MultiPatch::buttonSaveProject_Click);
			   // 
			   // MultiPatch
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->BackColor = System::Drawing::Color::Gainsboro;
			   this->ClientSize = System::Drawing::Size(684, 555);
			   this->Controls->Add(this->buttonSaveProject);
			   this->Controls->Add(this->label1);
			   this->Controls->Add(this->buttonLoadProject);
			   this->Controls->Add(this->projectPathTextBox);
			   this->Controls->Add(this->buttonClearList);
			   this->Controls->Add(this->label3);
			   this->Controls->Add(this->ignoreChecksumsCheckbox);
			   this->Controls->Add(this->buttonSelectOutput);
			   this->Controls->Add(this->outputPathTextBox);
			   this->Controls->Add(this->groupBox1);
			   this->Controls->Add(this->buttonApplyPatches);
			   this->Controls->Add(this->buttonAddPatches);
			   this->Controls->Add(this->dataGridView1);
			   this->MinimumSize = System::Drawing::Size(700, 520);
			   this->Name = L"MultiPatch";
			   this->Text = L"Padcher - MultiPatch";
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			   this->groupBox1->ResumeLayout(false);
			   this->groupBox1->PerformLayout();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

		   // --- ДОПОМІЖНІ ФУНКЦІЇ ---

		   /// <summary>
		   /// Застосовує один патч, дані якого передано як масив байтів.
		   /// </summary>
	private: byte_vector ApplySinglePatchFromData(byte_vector currentRomData, array<Byte>^ patchData, String^ patchFileName, bool ignoreChecksums) {
		if (patchFileName->EndsWith(".bps", StringComparison::OrdinalIgnoreCase)) {
			// Конвертуємо .NET масив байт у std::vector<uint8_t>
			byte_vector patch_vec(patchData->Length);
			System::Runtime::InteropServices::Marshal::Copy(patchData, 0, (IntPtr)patch_vec.data(), patchData->Length);

			return apply_bps_patch(currentRomData, patch_vec, !ignoreChecksums);
		}
		// Тут можна додати логіку для IPS, ASM тощо, якщо потрібно
		else {
			throw gcnew NotSupportedException("Patch type for '" + patchFileName + "' is not supported for in-memory patching yet.");
		}
	}

		   /// <summary>
		   /// Позначає помилку в таблиці.
		   /// </summary>
	private: void MarkErrorInGrid() {
		for each (DataGridViewRow ^ newRow in dataGridView1->Rows) {
			if (newRow->Cells["Status"]->Value->ToString() == "Applying...") {
				newRow->Cells["Status"]->Value = "ERROR";
				newRow->Cells["Status"]->Style->ForeColor = Color::Red;
				newRow->Cells["Status"]->Style->Font = gcnew System::Drawing::Font(this->Font, FontStyle::Bold);
				break;
			}
		}
	}

		   /// <summary>
		   /// Оновлює інформацію про контрольні суми для файлу.
		   /// </summary>
	private: void UpdateChecksums(String^ filePath) {
		if (!File::Exists(filePath)) {
			crcLabel->Text = "n/a"; md5Label->Text = "n/a"; sha1Label->Text = "n/a";
			return;
		}
		try {
			FileStream^ fs = gcnew FileStream(filePath, FileMode::Open, FileAccess::Read, FileShare::Read);
			crcLabel->Text = ComputeCrc32(fs);
			fs->Position = 0;
			md5Label->Text = ComputeHash(fs, MD5::Create());
			fs->Position = 0;
			sha1Label->Text = ComputeHash(fs, SHA1::Create());
			fs->Close();
		}
		catch (Exception^) {
			crcLabel->Text = "error"; md5Label->Text = "error"; sha1Label->Text = "error";
		}
	}

		   // --- ОБРОБНИКИ ПОДІЙ ---

	private: System::Void MultiPatch_Load(System::Object^ sender, System::EventArgs^ e) {
		 MessageBox::Show("MultiPatch_Load");
		dataGridView1->Columns->Clear();
		dataGridView1->Columns->Add("Num", "#");
		dataGridView1->Columns->Add("PatchPath", "Patch File");
		dataGridView1->Columns->Add("Status", "Status");

		dataGridView1->Columns["Num"]->Width = 40;
		dataGridView1->Columns["PatchPath"]->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
		dataGridView1->Columns["Status"]->Width = 120;

		// Скидаємо проєкт при завантаженні
		currentProject = nullptr;
	}

		   /// <summary>
		   /// Додає патчі з файлової системи до списку. Це початковий етап створення проєкту.
		   /// </summary>
	private: System::Void buttonAddPatches_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Multiselect = true;
		ofd->Title = "Select patch files in the desired order";
		ofd->Filter = "All Patches (*.ips;*.bps;*.asm)|*.ips;*.bps;*.asm|All files (*.*)|*.*";

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			for each(String ^ patchFile in ofd->FileNames) {

				// ==> ОСЬ ТУТ ВИПРАВЛЕННЯ <==
				// Просто починаємо з оголошення нової змінної
				int newRowIndex = dataGridView1->Rows->Add();
				DataGridViewRow^ newRow = dataGridView1->Rows[newRowIndex];

				newRow->Cells["Enabled"]->Value = true;

				newRow->Cells["Num"]->Value = dataGridView1->Rows->Count;
				newRow->Cells["PatchName"]->Value = Path::GetFileName(patchFile);
				newRow->Cells["PatchType"]->Value = "Main";
				newRow->Cells["FullPath"]->Value = patchFile;
			}
		}
	}

		   /// <summary>
		   /// Очищує список патчів у таблиці.
		   /// </summary>
	private: System::Void buttonClearList_Click(System::Object^ sender, System::EventArgs^ e) {
		dataGridView1->Rows->Clear();
	}

		   /// <summary>
		   /// Кнопка для застарілого функціоналу вибору файлу для збереження пропатченого ROM.
		   /// </summary>
	private: System::Void buttonSelectOutput_Click(System::Object^ sender, System::EventArgs^ e) {
		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = "All ROM Files|*.sfc;*.smc;*.gba;*.gb;*.gbc;*.nes;*.md|All files (*.*)|*.*";
		sfd->Title = "Save Patched ROM";
		if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			outputPathTextBox->Text = sfd->FileName;
		}
	}
		   private: void LoadProject(String^ projectFilePath) {
				   try {
					   // 1. Створення нового об'єкту проєкту та завантаження XML
					   currentProject = gcnew PadcherPack(); // Переконайтесь, що PadcherPack.h підключено
					   System::Xml::XmlDocument^ doc = gcnew System::Xml::XmlDocument();
					   doc->Load(projectFilePath);

					   // 2. Завантаження інформації про базовий ROM
					   System::Xml::XmlNode^ romNode = doc->SelectSingleNode("/PadcherPack/BaseRom");

					   // ==> ОСЬ ТУТ ВИПРАВЛЕННЯ <==
					   if (romNode == nullptr) throw gcnew System::Exception("BaseRom node not found in project file.");

					   currentProject->BaseRomFileName = romNode->Attributes["FileName"]->Value;
					   String^ projectDir = Path::GetDirectoryName(projectFilePath);
					   currentProject->FullBaseRomPath = Path::Combine(projectDir, currentProject->BaseRomFileName);
					   InitialRomPath = currentProject->FullBaseRomPath; // InitialRomPath - це член класу MultiPatch

					   if (!File::Exists(InitialRomPath)) {
						   MessageBox::Show("Base ROM '" + currentProject->BaseRomFileName + "' not found in the project folder. Please make sure it's in the same directory as the .padpack file.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
						   return;
					   }

					   // 3. Завантаження компонентів (патчів)
					   dataGridView1->Rows->Clear();
					   System::Xml::XmlNodeList^ componentNodes = doc->SelectNodes("/PadcherPack/Components/Component");
					   for each(System::Xml::XmlNode ^ node in componentNodes) {
						   PatchComponent^ component = gcnew PatchComponent();
						   component->FileName = node->Attributes["FileName"]->Value;
						   component->Type = node->Attributes["Type"]->Value;

						   int rowId = dataGridView1->Rows->Add();
						   DataGridViewRow^ row = dataGridView1->Rows[rowId];
						   row->Cells["Num"]->Value = rowId + 1;
						   row->Cells["Status"]->Value = "Loaded";

						   if (component->Type == "embedded") {
							   component->Base64Data = node->InnerText;
							   row->Cells["PatchPath"]->Value = component->FileName + " (embedded)";
						   }
						   else { // external
							   component->RelativePath = node->Attributes["RelativePath"]->Value;
							   row->Cells["PatchPath"]->Value = Path::Combine(projectDir, component->RelativePath);
						   }
						   currentProject->Components->Add(component);
					   }

					   projectPathTextBox->Text = projectFilePath;
					   MessageBox::Show("Project loaded successfully.", "Info", MessageBoxButtons::OK, MessageBoxIcon::Information);
				   }
				   catch (System::Exception^ ex) { // <== І ТУТ ТЕЖ System::Exception
					   MessageBox::Show("Failed to load project:\n" + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					   currentProject = nullptr;
				   }
			   }
	private: System::Void buttonLoadProject_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Filter = "Padcher Pack|*.padpack";
		ofd->Title = "Open Padcher Pack";

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			// Все, що робить цей метод - це відкриває діалог
			// і передає вибраний файл у наш новий метод LoadProject.
			LoadProject(ofd->FileName);
		}
	}
		   /// <summary>
		   /// Зберігає поточний стан (базовий ROM та список патчів) у файл проєкту .padproj
		   /// </summary>
	private: System::Void buttonSaveProject_Click(System::Object^ sender, System::EventArgs^ e) {
		if (String::IsNullOrEmpty(InitialRomPath) || !File::Exists(InitialRomPath)) {
			MessageBox::Show("Please select a base ROM before saving a project.", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}
		if (dataGridView1->Rows->Count == 0) {
			MessageBox::Show("Patch list is empty. Add patches before saving.", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}

		// 2. Діалог збереження файлу
		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = "Padcher Pack|*.padpack";
		sfd->Title = "Save Padcher Pack";
		sfd->FileName = "MyModPack.padpack";
		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK) {
			return; // Користувач скасував
		}

		try {
			// 3. Створення XML-структури
			System::Xml::XmlDocument^ doc = gcnew System::Xml::XmlDocument();
			System::Xml::XmlElement^ root = doc->CreateElement("PadcherPack");
			doc->AppendChild(root);

			// 4. Збереження інформації про базовий ROM і копіювання його
			String^ baseRomFileName = Path::GetFileName(InitialRomPath);
			System::Xml::XmlElement^ romElement = doc->CreateElement("BaseRom");
			romElement->SetAttribute("FileName", baseRomFileName);
			root->AppendChild(romElement);

			String^ projectDir = Path::GetDirectoryName(sfd->FileName);
			String^ newRomPath = Path::Combine(projectDir, baseRomFileName);
			if (Path::GetFullPath(InitialRomPath)->ToLower() != Path::GetFullPath(newRomPath)->ToLower()) {
				File::Copy(InitialRomPath, newRomPath, true); // Копіюємо ROM в папку проєкту
			}

			// 5. Підготовка до збереження патчів
			System::Xml::XmlElement^ componentsElement = doc->CreateElement("Components");
			root->AppendChild(componentsElement);

			String^ asmDirName = Path::GetFileNameWithoutExtension(sfd->FileName) + "_asm_files";
			String^ asmFullPath = Path::Combine(projectDir, asmDirName);

			// 6. Ітерація по патчах з таблиці та їх збереження
			for each (DataGridViewRow ^ row in dataGridView1->Rows) {
				String^ patchOriginalPath = row->Cells["PatchPath"]->Value->ToString();

				// Якщо у рядку вже не шлях, а "embedded", значить, він вже з проєкту, пропускаємо
				if (!File::Exists(patchOriginalPath)) continue;

				String^ patchFileName = Path::GetFileName(patchOriginalPath);
				System::Xml::XmlElement^ componentElement = doc->CreateElement("Component");
				componentElement->SetAttribute("FileName", patchFileName);

				// --- Розрізнення типів ---
				if (patchFileName->EndsWith(".asm", StringComparison::OrdinalIgnoreCase)) {
					// Зовнішній патч (.asm)
					componentElement->SetAttribute("Type", "external");

					if (!Directory::Exists(asmFullPath)) {
						Directory::CreateDirectory(asmFullPath);
					}
					String^ newAsmPath = Path::Combine(asmFullPath, patchFileName);
					File::Copy(patchOriginalPath, newAsmPath, true);

					componentElement->SetAttribute("RelativePath", Path::Combine(asmDirName, patchFileName));
				}
				else {
					componentElement->SetAttribute("Type", "embedded");
					array<Byte>^ patchBytes = File::ReadAllBytes(patchOriginalPath);
					componentElement->InnerText = Convert::ToBase64String(patchBytes);
				}
				componentsElement->AppendChild(componentElement);
			}

			// 7. Збереження XML-файлу
			doc->Save(sfd->FileName);

			projectPathTextBox->Text = sfd->FileName;
			MessageBox::Show("Padcher Pack saved successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (Exception^ ex) {
			MessageBox::Show("Failed to save project:\n" + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		
	}

	private: System::Void buttonApplyPatches_Click(System::Object^ sender, System::EventArgs^ e) {
		if (String::IsNullOrEmpty(InitialRomPath) || !File::Exists(InitialRomPath)) {
			MessageBox::Show("Initial ROM file not found. Please close this window and select it again.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (dataGridView1->Rows->Count == 0) {
			MessageBox::Show("The patch list is empty. Please add at least one patch.", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			return;
		}

		// 2. Визначаємо режим роботи: Проєктний чи Прямий Мультипатчинг
		bool isProjectMode = (currentProject != nullptr);

		String^ outputFilePath;
		if (isProjectMode) {
			// РЕЖИМ ПРОЄКТУ: Експортуємо фінальний BPS
			SaveFileDialog^ sfd = gcnew SaveFileDialog();
			sfd->Filter = "BPS Patch|*.bps";
			sfd->Title = "Export Final BPS Patch";
			sfd->FileName = Path::GetFileNameWithoutExtension(currentProject->BaseRomFileName) + "_final_patch.bps";
			if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;
			outputFilePath = sfd->FileName;
		}
		else {
			// РЕЖИМ МУЛЬТИПАТЧИНГУ: Зберігаємо фінальний ROM
			if (String::IsNullOrWhiteSpace(outputPathTextBox->Text)) {
				MessageBox::Show("Please specify an output file path for the patched ROM.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			outputFilePath = outputPathTextBox->Text;
			if (Path::GetFullPath(outputFilePath)->ToLower() == Path::GetFullPath(InitialRomPath)->ToLower()) {
				MessageBox::Show("The output file cannot be the same as the source ROM.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
		}

		// Блокуємо UI на час роботи
		this->Enabled = false;
		this->Update();

		try {
			// 3. Завантажуємо оригінальний ROM
			byte_vector originalRomData = read_file_native(marshal_as<std::string>(InitialRomPath));
			byte_vector currentRomData = originalRomData;

			// 4. Послідовно застосовуємо патчі
			for (int i = 0; i < dataGridView1->Rows->Count; ++i) {
				DataGridViewRow^ newRow = dataGridView1->Rows[i];

				// !!! ГОЛОВНА ПЕРЕВІРКА: чи увімкнений патч?
				bool isEnabled = Convert::ToBoolean(newRow->Cells["Enabled"]->Value);
				if (!isEnabled) {
					newRow->Cells["Status"]->Value = "Skipped"; // Позначаємо, що патч пропущено
					newRow->Cells["Status"]->Style->ForeColor = Color::Gray;
					continue; // Переходимо до наступного патча
				}

				newRow->Cells["Status"]->Value = "Applying...";
				newRow->Cells["Status"]->Style->ForeColor = Color::Blue;
				this->Update();

				if (isProjectMode) {
					// Беремо дані з проєкту
					PatchComponent^ component = currentProject->Components[i];
					if (component->Type == "embedded") {
						array<Byte>^ patchBytes = Convert::FromBase64String(component->Base64Data);
						currentRomData = ApplySinglePatchFromData(currentRomData, patchBytes, component->FileName, ignoreChecksumsCheckbox->Checked);
					}
					else { // external .asm
						String^ projectDir = Path::GetDirectoryName(projectPathTextBox->Text);
						String^ patchFullPath = Path::Combine(projectDir, component->RelativePath);
						currentRomData = ApplySinglePatch(currentRomData, patchFullPath, ignoreChecksumsCheckbox->Checked, Application::StartupPath);
					}
				}
				else {
					// Беремо шлях з нашої нової прихованої колонки "FullPath"
					String^ patchPath = newRow->Cells["FullPath"]->Value->ToString();
					currentRomData = ApplySinglePatch(currentRomData, patchPath, ignoreChecksumsCheckbox->Checked, Application::StartupPath);
				}

				newRow->Cells["Status"]->Value = "Done";
				newRow->Cells["Status"]->Style->ForeColor = Color::Green;
			}

			

			// 5. Зберігаємо результат
			if (isProjectMode) {
				// Створюємо та зберігаємо фінальний BPS
				String^ metadata = "Created with Padcher";
				byte_vector finalBpsData = create_bps_patch(originalRomData, currentRomData, true, marshal_as<std::string>(metadata));
				write_file_native(marshal_as<std::string>(outputFilePath), finalBpsData);
				MessageBox::Show("Final BPS patch exported successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
			else {
				// Зберігаємо фінальний ROM
				write_file_native(marshal_as<std::string>(outputFilePath), currentRomData);
				UpdateChecksums(outputFilePath); // Оновлюємо хеші на формі
				MessageBox::Show("All patches applied successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
		}
		catch (const std::exception& ex) {
			MarkErrorInGrid();
			MessageBox::Show("A C++ error occurred during patching:\n" + marshal_as<String^>(ex.what()), "Critical Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (Exception^ ex) {
			MarkErrorInGrid();
			MessageBox::Show("A .NET error occurred during patching:\n" + ex->Message, "Critical Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		finally {
			this->Enabled = true; // Повертаємо доступ до UI
		}
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		MessageBox::Show("This feature (Delete Patch) is not yet implemented.", "Info", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
		   private: System::Void MultiPatch_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e) {
			   if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
				   e->Effect = DragDropEffects::Copy;
			   }
			   else {
				   e->Effect = DragDropEffects::None;
			   }
		   }
		   private: System::Void MultiPatch_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e) {
			   array<String^>^ files = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);

			   // Створюємо список тільки для патчів
			   List<String^>^ patchFiles = gcnew List<String^>();

			   for each(String ^ file in files) {
				   String^ extension = Path::GetExtension(file)->ToLower();

				   // ВИПАДОК 1: Перетягнули файл проєкту .padpack
				   if (extension == ".padpack") {
					   // Якщо перетягнули кілька файлів, завантажуємо тільки перший проєкт
					   LoadProject(file);
					   return; // Виходимо, оскільки проєкт завантажено
				   }
				   // ВИПАДОК 2: Перетягнули патч
				   else if (extension == ".ips" || extension == ".bps" || extension == ".asm") {
					   patchFiles->Add(file);
				   }
				   // Інші типи файлів (наприклад, ROM'и) просто ігноруємо на цій формі
			   }

			   // Якщо були перетягнуті патчі, додаємо їх у таблицю
			   if (patchFiles->Count > 0) {
				   if (currentProject != nullptr) {
					   currentProject = nullptr;
					   projectPathTextBox->Text = "(Project cleared)";
				   }

				   // ==> ОНОВЛЕНИЙ ЦИКЛ <==
				   for each (String ^ patchFile in patchFiles) {
					   int rowId = dataGridView1->Rows->Add();
					   DataGridViewRow^ row = dataGridView1->Rows[rowId];

					   row->Cells["Enabled"]->Value = true;
					   row->Cells["Num"]->Value = dataGridView1->Rows->Count;
					   row->Cells["PatchName"]->Value = Path::GetFileName(patchFile);
					   row->Cells["PatchType"]->Value = "Main";
					   row->Cells["FullPath"]->Value = patchFile;

				   }
			   }
		   }
	};
}