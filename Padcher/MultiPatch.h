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
			this->Load += gcnew System::EventHandler(this, &MultiPatch::MultiPatch_Load);
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
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label2;
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
			   this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			   this->button2 = (gcnew System::Windows::Forms::Button());
			   this->label2 = (gcnew System::Windows::Forms::Label());
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
			   this->dataGridView1->Size = System::Drawing::Size(660, 277);
			   this->dataGridView1->TabIndex = 0;
			   // 
			   // buttonAddPatches
			   // 
			   this->buttonAddPatches->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonAddPatches->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			   this->buttonAddPatches->Location = System::Drawing::Point(522, 295);
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
			   this->outputPathTextBox->Text = L"(This field is for legacy single-rom output)";
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
			   this->buttonClearList->Location = System::Drawing::Point(396, 295);
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
			   // comboBox1
			   // 
			   this->comboBox1->FormattingEnabled = true;
			   this->comboBox1->Location = System::Drawing::Point(117, 294);
			   this->comboBox1->Name = L"comboBox1";
			   this->comboBox1->Size = System::Drawing::Size(121, 21);
			   this->comboBox1->TabIndex = 23;
			   // 
			   // button2
			   // 
			   this->button2->Location = System::Drawing::Point(244, 295);
			   this->button2->Name = L"button2";
			   this->button2->Size = System::Drawing::Size(146, 23);
			   this->button2->TabIndex = 24;
			   this->button2->Text = L"Delete Patch in List";
			   this->button2->UseVisualStyleBackColor = true;
			   this->button2->Click += gcnew System::EventHandler(this, &MultiPatch::button2_Click);
			   // 
			   // label2
			   // 
			   this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			   this->label2->AutoSize = true;
			   this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->label2->Location = System::Drawing::Point(12, 295);
			   this->label2->Name = L"label2";
			   this->label2->Size = System::Drawing::Size(99, 20);
			   this->label2->TabIndex = 22;
			   this->label2->Text = L"Select Patch";
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
			   this->Controls->Add(this->button2);
			   this->Controls->Add(this->comboBox1);
			   this->Controls->Add(this->label2);
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
		for each (DataGridViewRow ^ row in dataGridView1->Rows) {
			if (row->Cells["Status"]->Value->ToString() == "Applying...") {
				row->Cells["Status"]->Value = "ERROR";
				row->Cells["Status"]->Style->ForeColor = Color::Red;
				row->Cells["Status"]->Style->Font = gcnew System::Drawing::Font(this->Font, FontStyle::Bold);
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
		ofd->Filter = "BPS Patches (*.bps)|*.bps|All Patches (*.ips;*.bps;*.asm)|*.ips;*.bps;*.asm|All files (*.*)|*.*";

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			for each (String ^ patchFile in ofd->FileNames) {
				int rowId = dataGridView1->Rows->Add();
				DataGridViewRow^ row = dataGridView1->Rows[rowId];
				row->Cells["Num"]->Value = rowId + 1;
				row->Cells["PatchPath"]->Value = patchFile;
				row->Cells["Status"]->Value = "Pending";
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

		   /// <summary>
		   /// Завантажує проект з файлу .padproj
		   /// </summary>
	private: System::Void buttonLoadProject_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Filter = "Padcher Pack|*.padpack";
		ofd->Title = "Open Padcher Pack";
		if (ofd->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

		try {
			currentProject = gcnew PadcherPack();

			System::Xml::XmlDocument^ doc = gcnew System::Xml::XmlDocument();
			doc->Load(ofd->FileName);

			// 1. Завантажуємо інформацію про базовий ROM
			System::Xml::XmlNode^ romNode = doc->SelectSingleNode("/PadcherPack/BaseRom");
			if (romNode == nullptr) throw gcnew Exception("BaseRom node not found in project file.");

			currentProject->BaseRomFileName = romNode->Attributes["FileName"]->Value;
			String^ projectDir = Path::GetDirectoryName(ofd->FileName);
			currentProject->FullBaseRomPath = Path::Combine(projectDir, currentProject->BaseRomFileName);
			InitialRomPath = currentProject->FullBaseRomPath;

			if (!File::Exists(InitialRomPath)) {
				MessageBox::Show("Base ROM '" + currentProject->BaseRomFileName + "' not found in the project folder.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			dataGridView1->Rows->Clear();
			System::Xml::XmlNodeList^ componentNodes = doc->SelectNodes("/PadcherPack/Components/Component");
			for each (System::Xml::XmlNode ^ node in componentNodes) {
				PatchComponent^ component = gcnew PatchComponent();
				component->FileName = node->Attributes["FileName"]->Value;
				component->Base64Data = node->InnerText;
				currentProject->Components->Add(component);

				// Додаємо в таблицю для відображення
				int rowId = dataGridView1->Rows->Add();
				DataGridViewRow^ row = dataGridView1->Rows[rowId];
				row->Cells["Num"]->Value = rowId + 1;
				row->Cells["PatchPath"]->Value = component->FileName;
				row->Cells["Status"]->Value = "Loaded";
			}

			projectPathTextBox->Text = ofd->FileName;
		}
		catch (Exception^ ex) {
			MessageBox::Show("Failed to load project:\n" + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			currentProject = nullptr;
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

		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = "Padcher Pack|*.padpack"; // Використовуємо нове розширення
		sfd->Title = "Save Padcher Pack";
		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

		try {
			// Створення XML-документа
			System::Xml::XmlDocument^ doc = gcnew System::Xml::XmlDocument();
			System::Xml::XmlElement^ root = doc->CreateElement("PadcherPack");
			doc->AppendChild(root);

			// 1. Зберігаємо інформацію про базовий ROM
			String^ baseRomFileName = Path::GetFileName(InitialRomPath);
			System::Xml::XmlElement^ romElement = doc->CreateElement("BaseRom");
			romElement->SetAttribute("FileName", baseRomFileName);
			root->AppendChild(romElement);

			// Копіюємо базовий ROM у папку з проєктом
			String^ projectDir = Path::GetDirectoryName(sfd->FileName);
			String^ newRomPath = Path::Combine(projectDir, baseRomFileName);
			if (Path::GetFullPath(InitialRomPath) != Path::GetFullPath(newRomPath)) {
				File::Copy(InitialRomPath, newRomPath, true);
			}

			// 2. Зберігаємо кожен патч
			System::Xml::XmlElement^ componentsElement = doc->CreateElement("Components");
			root->AppendChild(componentsElement);

			for each (DataGridViewRow ^ row in dataGridView1->Rows) {
				String^ patchFullPath = row->Cells["PatchPath"]->Value->ToString();

				if (File::Exists(patchFullPath)) {
					String^ patchFileName = Path::GetFileName(patchFullPath);
					array<Byte>^ patchBytes = File::ReadAllBytes(patchFullPath);
					String^ base64Data = Convert::ToBase64String(patchBytes);

					System::Xml::XmlElement^ componentElement = doc->CreateElement("Component");
					componentElement->SetAttribute("FileName", patchFileName);
					componentElement->InnerText = base64Data; // Дані зберігаємо всередині тегу
					componentsElement->AppendChild(componentElement);
				}
				else if (currentProject != nullptr) { // Якщо патч вже завантажено з іншого проєкту
					for each (auto existingComp in currentProject->Components) {
						if (existingComp->FileName == patchFullPath) {
							System::Xml::XmlElement^ componentElement = doc->CreateElement("Component");
							componentElement->SetAttribute("FileName", existingComp->FileName);
							componentElement->InnerText = existingComp->Base64Data;
							componentsElement->AppendChild(componentElement);
							break;
						}
					}
				}
			}

			// 3. Зберігаємо XML-файл на диск
			doc->Save(sfd->FileName);

			projectPathTextBox->Text = sfd->FileName;
			MessageBox::Show("Padcher Pack saved successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (Exception^ ex) {
			MessageBox::Show("Failed to save project:\n" + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

		   /// <summary>
		   /// "Компілює" завантажений проєкт в один фінальний .BPS патч.
		   /// </summary>
	private: System::Void buttonApplyPatches_Click(System::Object^ sender, System::EventArgs^ e) {
		if (currentProject == nullptr) {
			MessageBox::Show("Please load a project first before exporting.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		if (String::IsNullOrEmpty(currentProject->FullBaseRomPath) || !File::Exists(currentProject->FullBaseRomPath)) {
			MessageBox::Show("Base ROM not found. Please check the project file and folder.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = "BPS Patch|*.bps";
		sfd->Title = "Export Final BPS Patch";
		sfd->FileName = Path::GetFileNameWithoutExtension(currentProject->BaseRomFileName) + "_final_patch.bps";
		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK) {
			return;
		}

		this->Enabled = false;
		this->Update();

		try {
			byte_vector originalRomData = read_file_native(marshal_as<std::string>(currentProject->FullBaseRomPath));
			byte_vector currentRomData = originalRomData;

			for (int i = 0; i < currentProject->Components->Count; ++i) {
				PatchComponent^ component = currentProject->Components[i];
				DataGridViewRow^ row = dataGridView1->Rows[i];

				row->Cells["Status"]->Value = "Applying...";
				this->Update();

				array<Byte>^ patchBytes = Convert::FromBase64String(component->Base64Data);
				currentRomData = ApplySinglePatchFromData(currentRomData, patchBytes, component->FileName, ignoreChecksumsCheckbox->Checked);

				row->Cells["Status"]->Value = "Done";
			}

			String^ metadata = "Created with Padcher";
			byte_vector finalBpsData = create_bps_patch(originalRomData, currentRomData, true, marshal_as<std::string>(metadata));
			write_file_native(marshal_as<std::string>(sfd->FileName), finalBpsData);

			MessageBox::Show("Final BPS patch exported successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		catch (const std::exception& ex) {
			MarkErrorInGrid();
			MessageBox::Show("A C++ error occurred:\n" + marshal_as<String^>(ex.what()), "Critical Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (Exception^ ex) {
			MarkErrorInGrid();
			MessageBox::Show("A .NET error occurred:\n" + ex->Message, "Critical Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		finally {
			this->Enabled = true;
		}
	}

		   /// <summary>
		   /// Обробник для невикористовуваної кнопки (заглушка)
		   /// </summary>
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		MessageBox::Show("This feature (Delete Patch) is not yet implemented.", "Info", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
	};
}