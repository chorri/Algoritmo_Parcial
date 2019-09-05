#pragma once
#include "ClassLibrary.h"

namespace MageWars {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	private: System::Windows::Forms::Timer^  Time;

			 Graphics^ mainGraphic;

			 //Buffer Area
			 BufferedGraphics^buffer;
			 BufferedGraphicsContext^espacioBuffer;

			 // temp
			 Graphics^ graficador;
			 CPlayer^ Ash;
			 CEscenario^ mapa;
			 CEscenario^ mapa2;
			 CEscenario^ mapa3;
			 CEscenario^ mapa4;
			 CEscenario^ mapa5;

			 CManagerDeNivel^ LevelManager;
			 CMainGameManager^ GameManager;

	private: System::Windows::Forms::Button^  BotonInicio;
	private: System::Windows::Forms::Label^  texto_vidas;
	private: System::Windows::Forms::Button^  MoreLivesButton;
	private: System::Windows::Forms::Button^  LessLivesButton;
	private: System::Windows::Forms::Label^  DisplayVidas;
	private: System::Windows::Forms::Button^  EvenMoreLivesButton;
	private: System::Windows::Forms::Button^  EvenLessLivesButton;
	private: System::Windows::Forms::Label^  DisplayMinutos;


	private: System::Windows::Forms::Label^  texto_minutos;
	private: System::Windows::Forms::Button^  LessTimeButton;
	private: System::Windows::Forms::Button^  MoreTimeButton;
	private: System::Windows::Forms::Timer^  MenuTimer;
	private: System::Windows::Forms::Label^  PauseText;
	private: System::Windows::Forms::Label^  TimeText;





			 CArma^ armaUno;
			 CArma^ armaDos;

	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
			this->mainGraphic = this->CreateGraphics();

			this->espacioBuffer = BufferedGraphicsManager::Current;
			buffer = this->espacioBuffer->Allocate(this->mainGraphic, this->ClientRectangle);

			//temp
			short ancho = 33;
			short alto = 51;
			short x = this->ClientSize.Width / 2 - ancho / 2;
			short y = this->ClientSize.Height / 2 - alto / 2;
			Rectangle area = Rectangle(x, y, ancho, alto);
			this->Ash = gcnew CPlayer("Assets\\Player\\Idle.png", area, 14, 1, 4);

			this->mapa = gcnew CEscenario(Rectangle(0, 0, 800, 600), "Assets\\Room1.png");
			this->mapa2 = gcnew CEscenario(Rectangle(0, 0, 800, 600), "Assets\\Room2.png");
			this->mapa3 = gcnew CEscenario(Rectangle(0, 0, 800, 600), "Assets\\Room3.png");
			this->mapa4 = gcnew CEscenario(Rectangle(0, 0, 800, 600), "Assets\\Room4.png");
			this->mapa5 = gcnew CEscenario(Rectangle(0, 0, 800, 600), "Assets\\ThroneRoom.png");

			LevelManager = gcnew CManagerDeNivel();
			LevelManager->SetAreaDeVentana(this->ClientRectangle);
			LevelManager->SetJugador(Ash);

			LevelManager->AgregarNivel(1,mapa);
			LevelManager->AgregarNivel(2,mapa2);
			LevelManager->AgregarNivel(3,mapa3);
			LevelManager->AgregarNivel(4,mapa4);
			LevelManager->AgregarNivel(5,mapa5);
			//Area
			
			armaUno = gcnew CArma("Assets\\Player\\Orbe\\Orb.png", Rectangle(0, 0, 32, 32), 16, 1,24);
			armaUno->DefinirMinMaxIndice(0, 10, true);
			armaDos = gcnew CArma("Assets\\Player\\Orbe\\IceOrb.png", Rectangle(0, 0, 32, 32), 16, 1, 24);
			armaDos->DefinirMinMaxIndice(0, 10, true);
			armaDos->ChangeBulletLifeTime(5);
			armaDos->ChangeBulletSpeed(0);
			armaDos->ChangeImageRoute("Assets\\Player\\Proyectil\\Area.png");
			armaDos->ChangeSubX(5);
			armaDos->ChangeBulletDimensions(98,98);
			armaDos->bulletStartsDead = true;

			Ash->AgregarArma(armaUno);
			Ash->AgregarArma(armaDos);

			GameManager = gcnew CMainGameManager(Ash);
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			delete mainGraphic;

			//temp
			delete graficador;
			delete Ash;
			delete mapa;
			delete mapa2;
			delete mapa3;
			delete mapa4;
			delete mapa5;
			delete armaUno;
			
			delete LevelManager;
			delete GameManager;
		}
	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Time = (gcnew System::Windows::Forms::Timer(this->components));
			this->BotonInicio = (gcnew System::Windows::Forms::Button());
			this->texto_vidas = (gcnew System::Windows::Forms::Label());
			this->MoreLivesButton = (gcnew System::Windows::Forms::Button());
			this->LessLivesButton = (gcnew System::Windows::Forms::Button());
			this->DisplayVidas = (gcnew System::Windows::Forms::Label());
			this->EvenMoreLivesButton = (gcnew System::Windows::Forms::Button());
			this->EvenLessLivesButton = (gcnew System::Windows::Forms::Button());
			this->DisplayMinutos = (gcnew System::Windows::Forms::Label());
			this->texto_minutos = (gcnew System::Windows::Forms::Label());
			this->LessTimeButton = (gcnew System::Windows::Forms::Button());
			this->MoreTimeButton = (gcnew System::Windows::Forms::Button());
			this->MenuTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->PauseText = (gcnew System::Windows::Forms::Label());
			this->TimeText = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// Time
			// 
			this->Time->Interval = 60;
			this->Time->Tick += gcnew System::EventHandler(this, &MyForm::Update);
			// 
			// BotonInicio
			// 
			this->BotonInicio->Location = System::Drawing::Point(316, 84);
			this->BotonInicio->Name = L"BotonInicio";
			this->BotonInicio->Size = System::Drawing::Size(117, 46);
			this->BotonInicio->TabIndex = 0;
			this->BotonInicio->Text = L"INICIAR JUEGO";
			this->BotonInicio->UseVisualStyleBackColor = true;
			this->BotonInicio->Click += gcnew System::EventHandler(this, &MyForm::StartGame);
			// 
			// texto_vidas
			// 
			this->texto_vidas->AutoSize = true;
			this->texto_vidas->Location = System::Drawing::Point(362, 143);
			this->texto_vidas->Name = L"texto_vidas";
			this->texto_vidas->Size = System::Drawing::Size(33, 13);
			this->texto_vidas->TabIndex = 7;
			this->texto_vidas->Text = L"Vidas";
			// 
			// MoreLivesButton
			// 
			this->MoreLivesButton->Location = System::Drawing::Point(411, 154);
			this->MoreLivesButton->Name = L"MoreLivesButton";
			this->MoreLivesButton->Size = System::Drawing::Size(22, 23);
			this->MoreLivesButton->TabIndex = 3;
			this->MoreLivesButton->Text = L">";
			this->MoreLivesButton->UseVisualStyleBackColor = true;
			this->MoreLivesButton->Click += gcnew System::EventHandler(this, &MyForm::MoreLives);
			// 
			// LessLivesButton
			// 
			this->LessLivesButton->Location = System::Drawing::Point(316, 154);
			this->LessLivesButton->Name = L"LessLivesButton";
			this->LessLivesButton->Size = System::Drawing::Size(22, 23);
			this->LessLivesButton->TabIndex = 2;
			this->LessLivesButton->Text = L"<";
			this->LessLivesButton->UseVisualStyleBackColor = true;
			this->LessLivesButton->Click += gcnew System::EventHandler(this, &MyForm::LessLives);
			// 
			// DisplayVidas
			// 
			this->DisplayVidas->Location = System::Drawing::Point(344, 156);
			this->DisplayVidas->Name = L"DisplayVidas";
			this->DisplayVidas->Size = System::Drawing::Size(65, 18);
			this->DisplayVidas->TabIndex = 8;
			this->DisplayVidas->Text = L"||||||||||||";
			this->DisplayVidas->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// EvenMoreLivesButton
			// 
			this->EvenMoreLivesButton->Location = System::Drawing::Point(439, 154);
			this->EvenMoreLivesButton->Name = L"EvenMoreLivesButton";
			this->EvenMoreLivesButton->Size = System::Drawing::Size(30, 23);
			this->EvenMoreLivesButton->TabIndex = 4;
			this->EvenMoreLivesButton->Text = L">>";
			this->EvenMoreLivesButton->UseVisualStyleBackColor = true;
			this->EvenMoreLivesButton->Click += gcnew System::EventHandler(this, &MyForm::EvenMoreLives);
			// 
			// EvenLessLivesButton
			// 
			this->EvenLessLivesButton->Location = System::Drawing::Point(280, 154);
			this->EvenLessLivesButton->Name = L"EvenLessLivesButton";
			this->EvenLessLivesButton->Size = System::Drawing::Size(30, 23);
			this->EvenLessLivesButton->TabIndex = 1;
			this->EvenLessLivesButton->Text = L"<<";
			this->EvenLessLivesButton->UseVisualStyleBackColor = true;
			this->EvenLessLivesButton->Click += gcnew System::EventHandler(this, &MyForm::EvenLessLives);
			// 
			// DisplayMinutos
			// 
			this->DisplayMinutos->Location = System::Drawing::Point(344, 215);
			this->DisplayMinutos->Name = L"DisplayMinutos";
			this->DisplayMinutos->Size = System::Drawing::Size(65, 18);
			this->DisplayMinutos->TabIndex = 10;
			this->DisplayMinutos->Text = L"||||||||||||";
			this->DisplayMinutos->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// texto_minutos
			// 
			this->texto_minutos->AutoSize = true;
			this->texto_minutos->Location = System::Drawing::Point(331, 197);
			this->texto_minutos->Name = L"texto_minutos";
			this->texto_minutos->Size = System::Drawing::Size(91, 13);
			this->texto_minutos->TabIndex = 9;
			this->texto_minutos->Text = L"Minutos de Juego";
			// 
			// LessTimeButton
			// 
			this->LessTimeButton->Location = System::Drawing::Point(316, 215);
			this->LessTimeButton->Name = L"LessTimeButton";
			this->LessTimeButton->Size = System::Drawing::Size(22, 23);
			this->LessTimeButton->TabIndex = 5;
			this->LessTimeButton->Text = L"<";
			this->LessTimeButton->UseVisualStyleBackColor = true;
			this->LessTimeButton->Click += gcnew System::EventHandler(this, &MyForm::LessTime);
			// 
			// MoreTimeButton
			// 
			this->MoreTimeButton->Location = System::Drawing::Point(411, 215);
			this->MoreTimeButton->Name = L"MoreTimeButton";
			this->MoreTimeButton->Size = System::Drawing::Size(22, 23);
			this->MoreTimeButton->TabIndex = 6;
			this->MoreTimeButton->Text = L">";
			this->MoreTimeButton->UseVisualStyleBackColor = true;
			this->MoreTimeButton->Click += gcnew System::EventHandler(this, &MyForm::MoreTime);
			// 
			// MenuTimer
			// 
			this->MenuTimer->Enabled = true;
			this->MenuTimer->Tick += gcnew System::EventHandler(this, &MyForm::MenuTimerUpdate);
			// 
			// PauseText
			// 
			this->PauseText->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->PauseText->BackColor = System::Drawing::Color::SteelBlue;
			this->PauseText->Enabled = false;
			this->PauseText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20));
			this->PauseText->ForeColor = System::Drawing::SystemColors::Desktop;
			this->PauseText->Location = System::Drawing::Point(12, 16);
			this->PauseText->Name = L"PauseText";
			this->PauseText->Size = System::Drawing::Size(107, 48);
			this->PauseText->TabIndex = 11;
			this->PauseText->Text = L"PAUSE";
			this->PauseText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->PauseText->Visible = false;
			// 
			// TimeText
			// 
			this->TimeText->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->TimeText->BackColor = System::Drawing::Color::CornflowerBlue;
			this->TimeText->Enabled = false;
			this->TimeText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16));
			this->TimeText->ForeColor = System::Drawing::SystemColors::Desktop;
			this->TimeText->Location = System::Drawing::Point(12, 16);
			this->TimeText->Name = L"TimeText";
			this->TimeText->Size = System::Drawing::Size(107, 48);
			this->TimeText->TabIndex = 12;
			this->TimeText->Text = L"####";
			this->TimeText->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->TimeText->Visible = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 562);
			this->Controls->Add(this->TimeText);
			this->Controls->Add(this->PauseText);
			this->Controls->Add(this->LessTimeButton);
			this->Controls->Add(this->MoreTimeButton);
			this->Controls->Add(this->DisplayMinutos);
			this->Controls->Add(this->texto_minutos);
			this->Controls->Add(this->EvenLessLivesButton);
			this->Controls->Add(this->EvenMoreLivesButton);
			this->Controls->Add(this->DisplayVidas);
			this->Controls->Add(this->LessLivesButton);
			this->Controls->Add(this->MoreLivesButton);
			this->Controls->Add(this->texto_vidas);
			this->Controls->Add(this->BotonInicio);
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"Mage Wars";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::DrawWindow);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::PlayerKeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::PlayerKeyUp);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MouseClickDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MoverElRaton);
			this->Resize += gcnew System::EventHandler(this, &MyForm::ResizeWindow);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {

	GameManager->AgregarBoton(this->BotonInicio);
	GameManager->AgregarBoton(this->MoreLivesButton);
	GameManager->AgregarBoton(this->EvenMoreLivesButton);
	GameManager->AgregarBoton(this->LessLivesButton);
	GameManager->AgregarBoton(this->EvenLessLivesButton);
	GameManager->AgregarBoton(this->MoreTimeButton);
	GameManager->AgregarBoton(this->LessTimeButton);

	GameManager->AgregarTexto(this->DisplayMinutos);
	GameManager->AgregarTexto(this->DisplayVidas);
	GameManager->AgregarTexto(this->texto_minutos);
	GameManager->AgregarTexto(this->texto_vidas);

	GameManager->SetMainTimer(this->Time);
	GameManager->SetPauseText(this->PauseText);
	GameManager->SetTimeText(this->TimeText);

	MessageBox::Show("Assets from CastleOutlaws\n\tby PinkPotato");
}

private: System::Void DrawWindow(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	//Primero dibujamos en el buffer
	//this->background->Dibujar(this->buffer->Graphics);

	//Luego ese buffer lo renderizamos
	//this->buffer->Render();
}
private: System::Void ResizeWindow(System::Object^  sender, System::EventArgs^  e) {
	}


private: System::Void Update(System::Object^  sender, System::EventArgs^  e) {

		//MessageBox::Show("TEMP");
		this->buffer->Graphics->Clear(Color::White);

		this->LevelManager->GetNivelActual()->GetEscenario()->DibujarImagen(this->buffer->Graphics);
		GameManager->MainUpdate(this->buffer->Graphics);
		
		this->buffer->Render();

		
	}
private: System::Void PlayerKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		switch (e->KeyCode)
		{
		case Keys::Up:
			this->GameManager->mainPlayer->CambiarDireccion(false, -1);
			break;
		case Keys::Down:
			this->GameManager->mainPlayer->CambiarDireccion(false, 1);
			break;
		case Keys::Left:
			this->GameManager->mainPlayer->CambiarDireccion(true, -1);
			break;
		case Keys::Right:
			this->GameManager->mainPlayer->CambiarDireccion(true, 1);
			break;
		
		case Keys::Space:
			GameManager->mainPlayer->Disparar(10, 16);
			break;

		case Keys::Escape:
			GameManager->GameOver(graficador);
			break;			
		case Keys::P:
			GameManager->AlternateMainTimer();
			break;

		case Keys::NumPad1:
			this->GameManager->mainPlayer->ChangeCurrentWeapon(0);
			break;
		case Keys::NumPad2:
			this->GameManager->mainPlayer->ChangeCurrentWeapon(1);
			break;

		case Keys::Q:
			this->GameManager->mainPlayer->ChangeCurrentWeapon(0);
			break;
		case Keys::W:
			this->GameManager->mainPlayer->ChangeCurrentWeapon(1);
			break;
		default:
			break;
		}
	}
private: System::Void PlayerKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		switch (e->KeyCode)
		{
		case Keys::Up:
			this->GameManager->mainPlayer->CambiarDireccion(false, 0);
			//this->mapa->navegarMapa(0, -1);
			break;
		case Keys::Down:
			this->GameManager->mainPlayer->CambiarDireccion(false, 0);
			//this->mapa->navegarMapa(0, 1);
			break;
		case Keys::Left:
			this->GameManager->mainPlayer->CambiarDireccion(true, 0);
			//this->mapa->navegarMapa(1, 0);
			break;
		case Keys::Right:
			this->GameManager->mainPlayer->CambiarDireccion(true, 0);
			//this->mapa->navegarMapa(-1, 0);
			break;
		default:
			break;
		}
	}

private: System::Void MoverElRaton(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		
	for (short i = 0; i < GameManager->mainPlayer->armas->Count; i++)
	{
		GameManager->mainPlayer->armas[i]->CambiarObjetivo(e->X,e->Y);
	}

}

private: System::Void MouseClickDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	//Magic Number para definir las animaciones de las balas
	GameManager->mainPlayer->Disparar(10,16);
}

private: System::Void StartGame(System::Object^  sender, System::EventArgs^  e) {
	GameManager->StartGame();
	GameManager->AlternateMainTimer();
	GameManager->mainPlayer->ResetArea();
	//Inicializar valores

	//Iniciar juego

}
private: System::Void MoreLives(System::Object^  sender, System::EventArgs^  e) {
	GameManager->ChangeLives(1);
}
private: System::Void LessLives(System::Object^  sender, System::EventArgs^  e) {
	GameManager->ChangeLives(-1);
}
private: System::Void EvenMoreLives(System::Object^  sender, System::EventArgs^  e) {
	GameManager->ChangeLives(5);
}
private: System::Void EvenLessLives(System::Object^  sender, System::EventArgs^  e) {
	GameManager->ChangeLives(-5);
}
private: System::Void LessTime(System::Object^  sender, System::EventArgs^  e) {
	GameManager->ChangeTime(-1);
}
private: System::Void MoreTime(System::Object^  sender, System::EventArgs^  e) {
	GameManager->ChangeTime(1);
}
private: System::Void MenuTimerUpdate(System::Object^  sender, System::EventArgs^  e) {
	this->DisplayVidas->Text = GameManager->vidasIniciales.ToString();
	this->DisplayMinutos->Text = GameManager->minutosPorPartida.ToString();
}
};
}