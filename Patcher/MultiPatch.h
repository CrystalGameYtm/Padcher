#pragma once

#include "PatcherLogic.h" // Підключаємо нашу нову спільну логіку

namespace Patcher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO; // Для роботи з файлами
	using namespace msclr::interop; // Для marshal_as

	public ref class MultiPatch : public System::Windows::Forms::Form
	{
	public:
		property String^ InitialRomPath;

		MultiPatch(void)
		{
			InitializeComponent();
			// Викликаємо наш метод налаштування при завантаженні форми
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
	private: System::Windows::Forms::ProgressBar^ progressBar1;
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
			   this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
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
			   this->buttonAddPatches->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			   this->buttonAddPatches->Location = System::Drawing::Point(525, 295);
			   this->buttonAddPatches->Name = L"buttonAddPatches";
			   this->buttonAddPatches->Size = System::Drawing::Size(150, 26);
			   this->buttonAddPatches->TabIndex = 2;
			   this->buttonAddPatches->Text = L"Додати патчі...";
			   this->buttonAddPatches->UseVisualStyleBackColor = true;
			   this->buttonAddPatches->Click += gcnew System::EventHandler(this, &MultiPatch::buttonAddPatches_Click);
			   // 
			   // buttonApplyPatches
			   // 
			   this->buttonApplyPatches->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			   this->buttonApplyPatches->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->buttonApplyPatches->Location = System::Drawing::Point(525, 427);
			   this->buttonApplyPatches->Name = L"buttonApplyPatches";
			   this->buttonApplyPatches->Size = System::Drawing::Size(150, 42);
			   this->buttonApplyPatches->TabIndex = 4;
			   this->buttonApplyPatches->Text = L"Пропатчити";
			   this->buttonApplyPatches->UseVisualStyleBackColor = true;
			   this->buttonApplyPatches->Click += gcnew System::EventHandler(this, &MultiPatch::buttonApplyPatches_Click);
			   // 
			   // ignoreChecksumsCheckbox
			   // 
			   this->ignoreChecksumsCheckbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			   this->ignoreChecksumsCheckbox->AutoSize = true;
			   this->ignoreChecksumsCheckbox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			   this->ignoreChecksumsCheckbox->Location = System::Drawing::Point(12, 354);
			   this->ignoreChecksumsCheckbox->Name = L"ignoreChecksumsCheckbox";
			   this->ignoreChecksumsCheckbox->Size = System::Drawing::Size(167, 20);
			   this->ignoreChecksumsCheckbox->TabIndex = 3;
			   this->ignoreChecksumsCheckbox->Text = L"Ігнорувати перевірки";
			   this->ignoreChecksumsCheckbox->UseVisualStyleBackColor = true;
			   // 
			   // buttonSelectOutput
			   // 
			   this->buttonSelectOutput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->buttonSelectOutput->Location = System::Drawing::Point(477, 322);
			   this->buttonSelectOutput->Name = L"buttonSelectOutput";
			   this->buttonSelectOutput->Size = System::Drawing::Size(42, 26);
			   this->buttonSelectOutput->TabIndex = 1;
			   this->buttonSelectOutput->Text = L"...";
			   this->buttonSelectOutput->UseVisualStyleBackColor = true;
			   this->buttonSelectOutput->Click += gcnew System::EventHandler(this, &MultiPatch::buttonSelectOutput_Click);
			   // 
			   // outputPathTextBox
			   // 
			   this->outputPathTextBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->outputPathTextBox->Location = System::Drawing::Point(12, 322);
			   this->outputPathTextBox->Name = L"outputPathTextBox";
			   this->outputPathTextBox->Size = System::Drawing::Size(462, 26);
			   this->outputPathTextBox->TabIndex = 0;
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
			   this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->groupBox1->Location = System::Drawing::Point(12, 380);
			   this->groupBox1->Name = L"groupBox1";
			   this->groupBox1->Size = System::Drawing::Size(510, 100);
			   this->groupBox1->TabIndex = 18;
			   this->groupBox1->TabStop = false;
			   this->groupBox1->Text = L"Інформація про фінальний файл";
			   // 
			   // sha1Label
			   // 
			   this->sha1Label->AutoSize = true;
			   this->sha1Label->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->sha1Label->Location = System::Drawing::Point(70, 70);
			   this->sha1Label->Name = L"sha1Label";
			   this->sha1Label->Size = System::Drawing::Size(14, 15);
			   this->sha1Label->TabIndex = 5;
			   this->sha1Label->Text = L"-";
			   // 
			   // md5Label
			   // 
			   this->md5Label->AutoSize = true;
			   this->md5Label->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->md5Label->Location = System::Drawing::Point(70, 48);
			   this->md5Label->Name = L"md5Label";
			   this->md5Label->Size = System::Drawing::Size(14, 15);
			   this->md5Label->TabIndex = 4;
			   this->md5Label->Text = L"-";
			   // 
			   // crcLabel
			   // 
			   this->crcLabel->AutoSize = true;
			   this->crcLabel->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
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
			   this->label3->AutoSize = true;
			   this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->label3->Location = System::Drawing::Point(12, 299);
			   this->label3->Name = L"label3";
			   this->label3->Size = System::Drawing::Size(183, 20);
			   this->label3->TabIndex = 13;
			   this->label3->Text = L"Вихідний файл (новий)";
			   // 
			   // buttonClearList
			   // 
			   this->buttonClearList->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			   this->buttonClearList->Location = System::Drawing::Point(525, 322);
			   this->buttonClearList->Name = L"buttonClearList";
			   this->buttonClearList->Size = System::Drawing::Size(147, 26);
			   this->buttonClearList->TabIndex = 19;
			   this->buttonClearList->Text = L"Очистити список";
			   this->buttonClearList->UseVisualStyleBackColor = true;
			   this->buttonClearList->Click += gcnew System::EventHandler(this, &MultiPatch::buttonClearList_Click);
			   // 
			   // progressBar1
			   // 
			   this->progressBar1->Location = System::Drawing::Point(525, 392);
			   this->progressBar1->Name = L"progressBar1";
			   this->progressBar1->Size = System::Drawing::Size(147, 29);
			   this->progressBar1->TabIndex = 20;
			   // 
			   // MultiPatch
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->BackColor = System::Drawing::Color::Gainsboro;
			   this->ClientSize = System::Drawing::Size(684, 485);
			   this->Controls->Add(this->progressBar1);
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
			   this->Text = L"MultiPatch - Послідовне застосування патчів";
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			   this->groupBox1->ResumeLayout(false);
			   this->groupBox1->PerformLayout();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

	private:
		// --- ОБРОБНИКИ ПОДІЙ ---

		// Цей метод викликається при завантаженні форми
		System::Void MultiPatch_Load(System::Object^ sender, System::EventArgs^ e) {
			// Налаштовуємо колонки в таблиці
			dataGridView1->Columns->Clear();
			dataGridView1->Columns->Add("Num", "№");
			dataGridView1->Columns->Add("PatchPath", "Файл патчу");
			dataGridView1->Columns->Add("Status", "Статус");

			dataGridView1->Columns["Num"]->Width = 40;
			dataGridView1->Columns["PatchPath"]->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
			dataGridView1->Columns["Status"]->Width = 120;

			// Якщо з попередньої форми передали шлях до ROM, використовуємо його
			if (!String::IsNullOrEmpty(InitialRomPath) && File::Exists(InitialRomPath)) {
				// Автоматично генеруємо ім'я для вихідного файлу
				String^ dir = Path::GetDirectoryName(InitialRomPath);
				String^ name = Path::GetFileNameWithoutExtension(InitialRomPath);
				String^ ext = Path::GetExtension(InitialRomPath);
				outputPathTextBox->Text = Path::Combine(dir, name + "_multi-patched" + ext);
			}
		}

		// Кнопка "Додати патчі..."
		System::Void buttonAddPatches_Click(System::Object^ sender, System::EventArgs^ e) {
			OpenFileDialog^ ofd = gcnew OpenFileDialog();
			ofd->Multiselect = true;
			ofd->Title = "Виберіть файли патчів у потрібному порядку";
			ofd->Filter = "Усі патчі (*.ips;*.bps;*.asm)|*.ips;*.bps;*.asm|All files (*.*)|*.*";

			if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				for each (String ^ patchFile in ofd->FileNames) {
					int rowId = dataGridView1->Rows->Add();
					DataGridViewRow^ row = dataGridView1->Rows[rowId];
					row->Cells["Num"]->Value = rowId + 1;
					row->Cells["PatchPath"]->Value = patchFile;
					row->Cells["Status"]->Value = "Очікує";
				}
			}
		}

		// Кнопка "Очистити список"
		System::Void buttonClearList_Click(System::Object^ sender, System::EventArgs^ e) {
			dataGridView1->Rows->Clear();
		}

		// Кнопка "..." для вибору вихідного файлу
		System::Void buttonSelectOutput_Click(System::Object^ sender, System::EventArgs^ e) {
			SaveFileDialog^ sfd = gcnew SaveFileDialog();
			sfd->Filter = "All ROM Files|*.sfc;*.smc;*.gba;*.gb;*.gbc;*.nes;*.md|All files (*.*)|*.*";
			sfd->Title = "Зберегти пропатчений ROM";
			if (!String::IsNullOrEmpty(outputPathTextBox->Text)) {
				try {
					sfd->InitialDirectory = Path::GetDirectoryName(outputPathTextBox->Text);
					sfd->FileName = Path::GetFileName(outputPathTextBox->Text);
				}
				catch (Exception^) {}
			}
			if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				outputPathTextBox->Text = sfd->FileName;
			}
		}

		// ГОЛОВНА КНОПКА: "ПРОПАТЧИТИ"
		System::Void buttonApplyPatches_Click(System::Object^ sender, System::EventArgs^ e) {
			// --- 1. Перевірка вхідних даних ---
			if (String::IsNullOrEmpty(InitialRomPath) || !File::Exists(InitialRomPath)) {
				MessageBox::Show("Не знайдено початковий файл ROM-у. Будь ласка, закрийте це вікно і виберіть його знову.", "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			if (dataGridView1->Rows->Count == 0) {
				MessageBox::Show("Список патчів порожній. Будь ласка, додайте хоча б один патч.", "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			String^ outputPath = outputPathTextBox->Text;
			if (String::IsNullOrWhiteSpace(outputPath)) {
				MessageBox::Show("Будь ласка, вкажіть шлях для збереження фінального файлу.", "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			if (Path::GetFullPath(outputPath)->ToLower() == Path::GetFullPath(InitialRomPath)->ToLower()) {
				MessageBox::Show("Вихідний файл не може бути тим самим, що й початковий ROM. Виберіть інший шлях.", "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			// Блокуємо кнопки на час роботи
			this->Enabled = false;
			this->Update();

			// --- 2. Основний цикл патчингу ---
			try {
				// Читаємо початковий ROM у пам'ять
				byte_vector currentRomData = read_file_native(marshal_as<std::string>(InitialRomPath));

				// Проходимо по кожному патчу в таблиці
				for (int i = 0; i < dataGridView1->Rows->Count; ++i) {
					DataGridViewRow^ row = dataGridView1->Rows[i];
					String^ patchPath = row->Cells["PatchPath"]->Value->ToString();

					// Оновлюємо статус у таблиці
					row->Cells["Status"]->Value = "Застосування...";
					row->Cells["Status"]->Style->ForeColor = Color::Blue;
					this->Update(); // Примусово оновлюємо UI

					// Застосовуємо один патч
					currentRomData = ApplySinglePatch(currentRomData, patchPath, ignoreChecksumsCheckbox->Checked, Application::StartupPath);

					// Оновлюємо статус на "Готово"
					row->Cells["Status"]->Value = "Готово";
					row->Cells["Status"]->Style->ForeColor = Color::Green;
				}

				// --- 3. Збереження результату ---
				write_file_native(marshal_as<std::string>(outputPath), currentRomData);

				// Оновлюємо контрольні суми для нового файлу
				UpdateChecksums(outputPath);

				MessageBox::Show("Усі патчі успішно застосовано!", "Успіх", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
			catch (const std::exception& ex) {
				// Помилка з нативного C++ коду
				MarkErrorInGrid();
				MessageBox::Show("Сталася помилка C++:\n" + marshal_as<String^>(ex.what()), "Критична помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (Exception^ ex) {
				// Помилка з .NET коду
				MarkErrorInGrid();
				MessageBox::Show("Сталася помилка .NET:\n" + ex->Message, "Критична помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			finally {
				// Розблоковуємо кнопки в будь-якому випадку
				this->Enabled = true;
			}
		}

		// --- ДОПОМІЖНІ МЕТОДИ КЛАСУ ---

		// Позначає рядок, на якому сталася помилка
		void MarkErrorInGrid() {
			for each (DataGridViewRow ^ row in dataGridView1->Rows) {
				if (row->Cells["Status"]->Value->ToString() == "Застосування...") {
					row->Cells["Status"]->Value = "ПОМИЛКА";
					row->Cells["Status"]->Style->ForeColor = Color::Red;
					row->Cells["Status"]->Style->Font = gcnew System::Drawing::Font(this->Font, FontStyle::Bold);
					break;
				}
			}
		}

		// Оновлює інформацію про контрольні суми для вказаного файлу
		void UpdateChecksums(String^ filePath) {
			if (!File::Exists(filePath)) {
				crcLabel->Text = "немає"; md5Label->Text = "немає"; sha1Label->Text = "немає";
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
				crcLabel->Text = "помилка"; md5Label->Text = "помилка"; sha1Label->Text = "помилка";
			}
		}
	};
}