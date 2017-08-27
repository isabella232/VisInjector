#pragma once
#include "Utils.h"
#include <msclr\marshal_cppstd.h>
#include <Vector>
#include <algorithm>
#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")


namespace VisInjector {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		std::vector<DWORD>* PIDList;
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			std::vector<DWORD>* PIDList = new std::vector<DWORD>;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			delete PIDList;
		}
	private: System::Windows::Forms::ListBox^  ProcessList;
	private: System::Windows::Forms::Button^  ProcessListUpdate;

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  ChooseDLL;
	private: System::Windows::Forms::TextBox^  DLLName;
	private: System::Windows::Forms::Button^  Inject;


	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ProcessList = (gcnew System::Windows::Forms::ListBox());
			this->ProcessListUpdate = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->ChooseDLL = (gcnew System::Windows::Forms::Button());
			this->DLLName = (gcnew System::Windows::Forms::TextBox());
			this->Inject = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// ProcessList
			// 
			this->ProcessList->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->ProcessList->FormattingEnabled = true;
			this->ProcessList->Location = System::Drawing::Point(12, 38);
			this->ProcessList->Name = L"ProcessList";
			this->ProcessList->Size = System::Drawing::Size(240, 145);
			this->ProcessList->TabIndex = 0;
			// 
			// ProcessListUpdate
			// 
			this->ProcessListUpdate->Location = System::Drawing::Point(12, 189);
			this->ProcessListUpdate->Name = L"ProcessListUpdate";
			this->ProcessListUpdate->Size = System::Drawing::Size(240, 34);
			this->ProcessListUpdate->TabIndex = 1;
			this->ProcessListUpdate->Text = L"Refresh";
			this->ProcessListUpdate->UseVisualStyleBackColor = true;
			this->ProcessListUpdate->Click += gcnew System::EventHandler(this, &MyForm::ProcessListUpdate_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openFileDialog1_FileOk);
			// 
			// ChooseDLL
			// 
			this->ChooseDLL->Location = System::Drawing::Point(177, 11);
			this->ChooseDLL->Name = L"ChooseDLL";
			this->ChooseDLL->Size = System::Drawing::Size(75, 20);
			this->ChooseDLL->TabIndex = 3;
			this->ChooseDLL->Text = L"Choose DLL";
			this->ChooseDLL->UseVisualStyleBackColor = true;
			this->ChooseDLL->Click += gcnew System::EventHandler(this, &MyForm::ChooseDLL_Click);
			// 
			// DLLName
			// 
			this->DLLName->Location = System::Drawing::Point(12, 12);
			this->DLLName->Name = L"DLLName";
			this->DLLName->ReadOnly = true;
			this->DLLName->Size = System::Drawing::Size(159, 20);
			this->DLLName->TabIndex = 4;
			// 
			// Inject
			// 
			this->Inject->Location = System::Drawing::Point(12, 229);
			this->Inject->Name = L"Inject";
			this->Inject->Size = System::Drawing::Size(240, 34);
			this->Inject->TabIndex = 5;
			this->Inject->Text = L"Inject";
			this->Inject->UseVisualStyleBackColor = true;
			this->Inject->Click += gcnew System::EventHandler(this, &MyForm::Inject_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(264, 269);
			this->Controls->Add(this->Inject);
			this->Controls->Add(this->DLLName);
			this->Controls->Add(this->ChooseDLL);
			this->Controls->Add(this->ProcessListUpdate);
			this->Controls->Add(this->ProcessList);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"MyForm";
			this->Text = L"VisInjector";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
		this->ProcessListUpdate_Click(sender, e); //Update process list when form is loaded
	}

	private: System::Void ProcessListUpdate_Click(System::Object^  sender, System::EventArgs^  e) {
		ProcessList->Items->Clear(); //Clear out old items
		delete PIDList; //Clear old PIDs
		PIDList = new std::vector<DWORD>;

		WTS_PROCESS_INFO* pWPIs = NULL;
		DWORD dwProcCount = 0;
		if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
		{
			//Sort process list to make it easier to navigate
			std::sort(pWPIs, pWPIs+dwProcCount, compareProcessList);
			for (DWORD i = 0; i < dwProcCount; i++)
			{
				ProcessList->Items->Add(gcnew String(pWPIs[i].pProcessName));
				PIDList->push_back(pWPIs[i].ProcessId);
				//pWPIs[i].ProcessId = process ID
				//pWPIs[i].SessionId = session ID
				//pWPIs[i].pUserSid = user SID that started the process
			}
		}

		//Free memory
		if (pWPIs)
			WTSFreeMemory(pWPIs);
		
	}

	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
	}

	private: System::Void ChooseDLL_Click(System::Object^  sender, System::EventArgs^  e) {
		openFileDialog1->InitialDirectory = "c:\\";
		openFileDialog1->Filter = "DLL files (*.dll)|*.dll|All files (*.*)|*.*";
		openFileDialog1->FilterIndex = 2;
		openFileDialog1->RestoreDirectory = true;

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			DLLName->Text = openFileDialog1->FileName; //Set textbox value to selected filename 
			DLLName->SelectAll(); //Scroll to end of textbox to more easily see filename
		}
	}

	private: System::Void Inject_Click(System::Object^  sender, System::EventArgs^  e) {
		String^ proccessName = ProcessList->SelectedItem->ToString();
		String^ dllPath = openFileDialog1->FileName;

		if (proccessName->Length > 0 && dllPath->Length > 0) //Check to make sure values are set properly
		{
			/*
			//System::String to WCHAR* conversion for future reference
			pin_ptr<const wchar_t> wchstr = PtrToStringChars(proccessName);
			WCHAR* convertedProccessName = const_cast<wchar_t*>(wchstr);

			wchstr = PtrToStringChars(dllPath);
			WCHAR* converteddllPath = const_cast<wchar_t*>(wchstr);
			*/
			//Get PID
			DWORD PID = PIDList->at(ProcessList->SelectedIndex);

			//Convert from System::String to std::basic_string
			using namespace Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(proccessName)).ToPointer();
			std::string convertedProccessName = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
			chars = (const char*)(Marshal::StringToHGlobalAnsi(dllPath)).ToPointer();
			std::string converteddllPath = chars;

			//Perform injection
			if (createRemoteThreadInject(convertedProccessName.c_str(), converteddllPath.c_str(), PID))
			{
				MessageBox::Show("Injected successfully.");
			}
			else
			{
				MessageBox::Show("Injection failed.");
			}
		}
	}

};
}