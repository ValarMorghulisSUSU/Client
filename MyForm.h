#pragma once

#include <winsock.h>
#include <stdio.h>

#define WSA_UDP_NETEVENT	(WM_USER+1)
#define	WSA_TCP_NETEVENT	(WM_USER+2)

#define SERVER_PORT_TCP     0x2021
#define CLIENT_PORT_TCP     0x2031

#define SERVER_PORT_UDP     0x2001
#define CLIENT_PORT_UDP     0x2011

SOCKET         UDPSocket, TCPSocket;
sockaddr_in	   CallAddress;
sockaddr_in	   OurAddress;

int			   f = 0;
namespace Practive5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^ listBox1;
	private: System::Windows::Forms::ListBox^ listBox2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::TextBox^ textBox1;
	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(225, 12);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(206, 173);
			this->listBox1->TabIndex = 0;
			// 
			// listBox2
			// 
			this->listBox2->FormattingEnabled = true;
			this->listBox2->Location = System::Drawing::Point(13, 12);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(206, 173);
			this->listBox2->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(13, 192);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(206, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Подключиться";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(13, 247);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(206, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Отключиться";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(310, 218);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(121, 23);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Отправить";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(13, 221);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(291, 20);
			this->textBox1->TabIndex = 5;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(461, 296);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listBox2);
			this->Controls->Add(this->listBox1);
			this->Name = L"MyForm";
			this->Text = L" CLIENT";
			this->Activated += gcnew System::EventHandler(this, &MyForm::MyForm_Activated);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Activated(System::Object^ sender, System::EventArgs^ e) {
		WSADATA	    WSAData;
		int		    rc;
		char			Name[101], * IpAddr, Buf[1000];
		PHOSTENT       phe;

		if (f == 1) return;
		f = 1;
		rc = WSAStartup(MAKEWORD(2, 0), &WSAData);
		if (rc != 0) {
			listBox1->Items->Add("Ошибка инициализации WSAStartup");
			return;
		}

		//UDP
		UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (UDPSocket == INVALID_SOCKET) {
			listBox1->Items->Add("Протокол UDP установлен.");
		}

		memset(&OurAddress, 0, sizeof(OurAddress));
		OurAddress.sin_family = AF_INET;
		OurAddress.sin_port = CLIENT_PORT_UDP;

		rc = bind(UDPSocket, (LPSOCKADDR)&OurAddress, sizeof(sockaddr_in));
		if (rc == SOCKET_ERROR) {
			listBox1->Items->Add("Адресная ошибка");
			return;
		} 

		rc = WSAAsyncSelect(UDPSocket, (HWND)(this->Handle.ToInt32()), WSA_UDP_NETEVENT, FD_READ);
		if (rc != 0) {
			listBox1->Items->Add("Ошибка WSAAsyncSelect");
			return;
		} 

		//TCP
		TCPSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (TCPSocket == INVALID_SOCKET) {
			listBox1->Items->Add("Протокол TCP установлен.");
		}

		memset(&OurAddress, 0, sizeof(OurAddress));
		OurAddress.sin_family = AF_INET;
		OurAddress.sin_port = CLIENT_PORT_TCP;

		rc = bind(TCPSocket, (LPSOCKADDR)&OurAddress, sizeof(sockaddr_in));
		if (rc == SOCKET_ERROR) {
			listBox1->Items->Add("Адресная ошибка");
			return;
		} 

		rc = WSAAsyncSelect(TCPSocket, (HWND)(this->Handle.ToInt32()), WSA_TCP_NETEVENT, FD_ACCEPT);
		if (rc != 0) {
			listBox1->Items->Add("Ошибка WSAAsyncSelect");
			return;
		} 

		gethostname(Name, 101);
		strcpy(Buf, "Имя компьютера ");
		strcat(Buf, Name);
		String^ s = gcnew String(Buf);
		listBox1->Items->Add(s);

		phe = gethostbyname(Name);
		if (phe != NULL) {
			memcpy((void*)&(OurAddress.sin_addr), phe->h_addr, phe->h_length);
			IpAddr = inet_ntoa(OurAddress.sin_addr);
			strcpy(Buf, "IP-Адрес ");
			strcat(Buf, IpAddr);
			String^ s2 = gcnew String(Buf);
			listBox1->Items->Add(s2);
		} 

		listBox1->Items->Add(L"Сервер запущен");
	}
		   protected: virtual void WndProc(Message% m) override {
			   int      rc, l = sizeof(CallAddress);
			   wchar_t  Buf[500];

			   if (m.Msg == WSA_UDP_NETEVENT) {
				   if (m.LParam.ToInt32() == FD_READ) {
					   rc = recvfrom((SOCKET)m.WParam.ToInt32(), (char*)Buf, sizeof(Buf) - 1, 0, (PSOCKADDR)&CallAddress, &l);
					   if (rc == SOCKET_ERROR) {
						   rc = WSAGetLastError();
						   listBox1->Items->Add(String::Format("Ошибка recv {0}", rc));
						   return;
					   } // if
					   if (rc >= 1) {
						   String^ s = gcnew String(Buf);
						   listBox2->Items->Add(s);
					   } // if

					   CallAddress.sin_port = SERVER_PORT_TCP;
					   rc = connect(TCPSocket, (PSOCKADDR)&CallAddress, sizeof(CallAddress));

					   listBox1->Items->Add("Канал создан");

					   rc = WSAAsyncSelect(TCPSocket, (HWND)(this->Handle.ToInt32()), WSA_TCP_NETEVENT, FD_READ);
					   if (rc != 0) {
						   listBox1->Items->Add("Ошибка WSAAsyncSelect");
						   return;
					   }
				   }
			   }
			   if (m.Msg == WSA_TCP_NETEVENT) {
				   if (m.LParam.ToInt32() == FD_READ) {
					   rc = recv((SOCKET)m.WParam.ToInt32(), (char*)Buf, sizeof(Buf) - 1, 0);
					   if (rc == SOCKET_ERROR) {
						   rc = WSAGetLastError();
						   listBox1->Items->Add(String::Format("Ошибка recv {0}", rc));
						   return;
					   }
					   if (rc >= 1) {
						   String^ s = gcnew String(Buf);
						   listBox1->Items->Add(s);
					   }
				   }
				   else {
					   listBox1->Items->Add("Канал разорван");
				   }
			   }

			   Form::WndProc(m);
		   }
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	OurAddress.sin_addr.S_un.S_un_b.s_b4 = 255;
	OurAddress.sin_port = SERVER_PORT_UDP;
	int rc = sendto(UDPSocket, "1", 4, 0, (LPSOCKADDR)&OurAddress, sizeof(OurAddress));
	if (rc == SOCKET_ERROR) {
		rc = WSAGetLastError();
		listBox1->Items->Add(String::Format("Ошибска recv {0}", rc));
		return;
	}
	listBox2->Items->Add("1");

}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
	int      rc, l, i;
	char     Buf[500];
	wchar_t  Buf2[1000];
	PHOSTENT phe;

	l = textBox1->Text->Length;
	if (l > 0) {
		for (i = 0; i < l; i++) {
			Buf2[i] = textBox1->Text->default[i];
			Buf2[i + 1] = 0;
		}

		rc = send(TCPSocket, (char*)Buf2, 2 * l + 2, 0);
		if (rc == SOCKET_ERROR) {
			rc = WSAGetLastError();
			listBox1->Items->Add(String::Format("Ошибка recv {0}", rc));
			return;
		}
		String^ s = gcnew String(Buf2);
		listBox2->Items->Add(s);
	} 
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	closesocket(TCPSocket);
}
};
}
