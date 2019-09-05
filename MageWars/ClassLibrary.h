#pragma once
using namespace System;//Rectangle
using namespace System::ComponentModel;
using namespace System::Drawing;//Bitmap
using namespace System::Drawing::Drawing2D;
using namespace System::Collections::Generic;// List :D
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::Design;



//Los diferentes estados de las diferentes clases que usan maquinas de estados


enum EstadosBala
{
	Moving, Impact
};

enum EstadosArma
{
	Idle, Shooting
};


enum EstadosEnemigo 
{
	EIdle, EMoving, EChasing, EShoot, EDying
};

enum PlayerStates
{
	Play, Damage
};

ref class CManagerMatematico
{
private:
	static CManagerMatematico^ instance;
public:

	CManagerMatematico() {
		instance = this;
	}

	float GetDistancia(float x1, float y1, float x2, float y2) {
		float distancia = Math::Sqrt(Math::Pow(x2 - x1, 2) + Math::Pow(y2 - y1, 2));
		return distancia;
	}

	PointF GetVectorDireccion(float x1, float y1, float x2, float y2) {
		PointF vectorDireccion;


		//Vector Dirección
		float xDireccion = x2 - x1;
		float yDireccion = y2 - y1;

		//Distancia empleada para normalizar el Vector Dirección
		float distancia = Math::Sqrt(Math::Pow(xDireccion, 2) + Math::Pow(yDireccion, 2));

		//Se Normaliza el Vector Dirección para que sus valores se encuentren entre 0 y 1
		xDireccion = xDireccion / distancia;
		yDireccion = yDireccion / distancia;

		vectorDireccion.X = xDireccion;
		vectorDireccion.Y = yDireccion;


		return vectorDireccion;
	}

	static CManagerMatematico^ GetInstance() {
		return instance;
	}
};

ref class CBase
{
protected:
	Bitmap^ imagen;
	Rectangle area;

public:
	CBase(String^ ruta, Rectangle _area) {
		imagen = gcnew Bitmap(ruta);
		area = _area;
	}
	CBase(Bitmap^ _imagen, Rectangle _area) {
		imagen = _imagen;
		area = _area;
	}
	~CBase() {
		delete imagen;
	}

	Bitmap^ GetImagen() {
		return imagen;
	}
	Rectangle GetArea() {
		return area;
	}
	void SetArea(Rectangle area) {
		this->area = area;
	}

	virtual void DibujarImagen(Graphics^ graficador) {
		graficador->DrawImage(imagen,area);
	}
private:
};

ref class CEntidad : public CBase
{
protected:
	//Usamos esto debido a que algunos de nuestros spritesheets son una tira
	short minIndice = 0;
	short maxIndice = 0;
public:

	//Loop significa que luego de que termina de reproducir las imagenes regresa al inicio
	bool loop = true;

	short indiceX, indiceY;
	short subImagenesX, subImagenesY;
	
	CEntidad(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY)
		: CBase(ruta, _area), subImagenesX(_subImagenesX), subImagenesY(_subImagenesY), indiceX(0), indiceY(0) {
		maxIndice = this->subImagenesX - 1;
	}
	CEntidad(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY)
		: CBase(_imagen, _area), subImagenesX(_subImagenesX), subImagenesY(_subImagenesY), indiceX(0), indiceY(0) {
		maxIndice = this->subImagenesX - 1;
	}

	void DefinirMinMaxIndice(short min, short max, bool canLoop) {
		minIndice = min;
		maxIndice = max;
		loop = canLoop;
	}

	virtual void Animar(Graphics^ graficador) {
		DibujarImagen(graficador);
		if (this->indiceX >= maxIndice) {
			if (loop) {
				this->indiceX = minIndice;
			}
		}
		else {
			this->indiceX++;
		}
	}

	void DibujarImagen(Graphics^ graficador) override {
		//Lapiz para poder ver los limites del sprite
		Pen^ pen = gcnew Pen(Color::Red);
		graficador->DrawRectangle(pen, area);

		Rectangle recorte = Recortar();
		graficador->DrawImage(this->imagen, this->area, recorte, GraphicsUnit::Pixel);


		//Borrar el lapiz
		delete pen;
	}


protected:
	virtual Rectangle Recortar() {

		short ancho = this->imagen->Width / this->subImagenesX;
		short alto = this->imagen->Height / this->subImagenesY;
		short x = this->indiceX * ancho;
		short y = this->indiceY * alto;
		return Rectangle(x, y, ancho, alto);
	}
};


ref class CEscenario : public CEntidad {
	
	//Cuando hice este codigo solo DIOS y yo entendiamos....
	//Ahora solo DIOS
public:

	const short anchoRecorte = 815;
	const short altoRecorte = 640;
	const short dx = 1;
	const short dy = 1;
	/*
		anchoF=100
		anchoR=20
		x = [0, anchoF-anchoRecorte] = [0, 80] = 81
		nroSIX = intervaloX / 3
		y = [0, altoF -altoRecorte ]
	*/
	CEscenario(Rectangle area, String^ ruta)
		: CEntidad(ruta, area,
		(area.Width - anchoRecorte) / dx,
			(area.Height - altoRecorte) / dy) { }

	void navegarMapa(short movX, short movY) {
		//movX == 0 si no se mueve en x, -1 si va a la izquierda, +1 si va a la derecha
		//movY == 0 si no se mueve en y, -1 si va hacia arriba, +1 si va hacia abajo
		this->indiceX += this->dx * movX;
		this->indiceY += this->dy * movY;
	}


protected:
	virtual Rectangle Recortar() override {
		return Rectangle(this->indiceX, this->indiceY, this->anchoRecorte, this->altoRecorte);
	}
};

ref class CNivel
{
private:
	List<Rectangle>^ rectEntidades;
	CEscenario^ escenario;
public:
	CNivel() {}
	CNivel(short nivelDeDificultad, CEscenario^ _escenario) {
		rectEntidades = gcnew List<Rectangle>();
		escenario = _escenario;
		//delete imagen;
	}
	~CNivel() {
		delete rectEntidades;
		delete escenario;
	}

	CEscenario^ GetEscenario() {
		return escenario;
	}

	List<Rectangle>^ GetPuntos() {
		return rectEntidades;
	}

	Rectangle GetPuntoEnemigo(short index) {
		return rectEntidades[index];
	}
};

ref class CManagerDeNivel
{
private:
	static CManagerDeNivel^ instance;

	Rectangle areaDeVentana;

	CEntidad^ jugador;
	List<CNivel^>^ niveles;
	short nivelMaximo = 5;
	short nivelActual = 0;
public:

	CManagerDeNivel() {
		niveles = gcnew List<CNivel^>();

		instance = this;
	}
	~CManagerDeNivel() {
		for (short i = 0; i < niveles->Count; i++)
		{
			delete niveles[i];
		}
		delete niveles;
	}

	void SetJugador(CEntidad^ entidadJugador) {
		jugador = entidadJugador;
	}
	short GetNivelMaximo() {
		return nivelMaximo;
	}
	short GetNumeroNivel() {
		return nivelActual;
	}

	void SubirNivel() {
		nivelActual++;
	}

	CEntidad^ GetJugador() {
		return jugador;
	}

	void SetAreaDeVentana(Rectangle newArea) {
		areaDeVentana = newArea;
	}
	Rectangle GetAreaDeVentana() {
		return areaDeVentana;
	}

	void AgregarNivel(short nivelDeDificultad, CEscenario^ escenario) {
		CNivel^ nivelTemporal = gcnew CNivel(nivelDeDificultad, escenario);
		niveles->Add(nivelTemporal);
	}

	void CambiarDeNivel(int index) {
		nivelActual = index;
	}

	CNivel^ GetNivelActual() {
		return niveles[nivelActual];
	}

	static CManagerDeNivel^ GetInstance() {
		return instance;
	}
};

ref class CMovil : public CEntidad {
public:
	//Variables
	float dX, dY;
	bool canMove = true;
	float speed = 4;
	double fvida = 0;
	short vida = 0;

	Point offset = Point(-20,50);

	//Metodos
	CMovil(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed)
		: CEntidad(ruta, _area,_subImagenesX,_subImagenesY), speed(_speed) {}
	CMovil(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed)
		: CEntidad(_imagen, _area,_subImagenesX,_subImagenesY), speed(_speed) {}


	void SetVida(double nuevaVida) {
		fvida = nuevaVida;
		vida = nuevaVida;
	}

	void ChangeVida(double ammount) {
		fvida += ammount;
		vida = Math::Ceiling(fvida);
	}

	Rectangle AreaObjetivo() {
		Rectangle objArea = area;
		objArea.X += dX * speed;
		objArea.Y += dY * speed;

		return objArea;
	}

	void MostrarVida(Graphics^ graficador) {
		SolidBrush^ lapiz = gcnew SolidBrush(Color::Black);
		Font^ fuente = gcnew Font("Arial", 18);
		StringFormat^ formato = gcnew StringFormat();
		formato->Alignment = StringAlignment::Center;
		formato->LineAlignment = StringAlignment::Center;
		formato->Trimming = StringTrimming::None;

		Point punto = Point(area.Location.X + offset.X, area.Location.Y + offset.Y);
		graficador->DrawString(vida.ToString(),fuente,lapiz,Rectangle(punto,area.Size),formato);

		delete fuente;
		delete formato;
		delete lapiz;
	}

	void Animar(Graphics^ graficador) override {
		DibujarImagen(graficador);
		MostrarVida(graficador);
		if (this->indiceX >= maxIndice) {
			if (loop) {
				this->indiceX = minIndice;
			}
		}
		else {
			this->indiceX++;
		}
	}

	void Mover(Graphics^ graficador) {
		if (area.X + dX > 0 && area.X + dX + area.Width < graficador->VisibleClipBounds.Right) {
			area.X += dX * speed;
		}
		if (area.Y + dY > 0 && area.Y + dY + area.Height < graficador->VisibleClipBounds.Bottom) {
			area.Y += dY * speed;
		}
	}

	/* Luego agregaremos la rotacion de los proyectiles
	void LookAt(short posX, short posY, Bitmap^ img)
	{
		short centerX = area.X + area.Width / 2;
		short centerY = area.Y + area.Height / 2;

		float angulo = Math::Atan2(centerY - posY, centerX - posX) * 180 / Math::PI;
		angulo += 90;
		angulo = (angulo < 0) ? angulo + 360 : angulo;

		Graphics^ temp = Graphics::FromImage(img);
		//temp->TranslateTransform((float)img->Width / 2, (float)img->Height / 2);
		
		temp->Transform->RotateAt(angulo,Point(posX,posY));

		//temp->TranslateTransform(-(float)img->Width / 2, -(float)img->Height / 2);
		temp->DrawImage(img,Point(centerX,centerY));
		temp->InterpolationMode = InterpolationMode::HighQualityBicubic;


		img->SetResolution(img->HorizontalResolution, img->VerticalResolution);
		
		delete temp;
	}
	*/


	//El metodo solo se llama por el jugador entonces sabemos que solo podra cambiar X o Y
	//para definir que valor se cambia se usa un booleano
	void CambiarDireccion(bool cambiaX, float valor) {
		if (cambiaX) {
			dX = valor;
		}
		else {
			dY = valor;
		}
	}
};

ref class CBala : public CMovil {
public:

	CBala(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed, float lifeTime, double damage,List<CBala^>^ referencia)
		: CMovil(ruta, _area, _subImagenesX, _subImagenesY, _speed){
		listaReferencia = referencia;
		daño = damage;
	}
	CBala(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed, float lifeTime, double damage,List<CBala^>^ referencia)
		: CMovil(_imagen, _area, _subImagenesX, _subImagenesY,_speed) {
		listaReferencia = referencia;
		daño = damage;
	}

	EstadosBala estadoActual = EstadosBala::Moving;
	float currentTime = 0;
	float lifeTime = 25;

	double daño;

	//Referencia de la lista donde está creado
	List<CBala^>^ listaReferencia;

	bool quickFixAreaDuration = false;

	//Los metodos updates seran cargados en MyForm gracias al timer
	//La bala tiene dos estados que definen lo que harán
	void Update(Graphics^ graficador){//,List<CEntidad^>^ entidades) {
		switch (estadoActual)
		{
		case  EstadosBala::Moving:
			//CheckCollison(entidades);

			if (currentTime >= lifeTime || area.X <= 0 || area.Y <= 0 ||
				area.X >= CManagerDeNivel::GetInstance()->GetAreaDeVentana().Width - area.Width ||
				area.Y >= CManagerDeNivel::GetInstance()->GetAreaDeVentana().Height - area.Height) {
				canMove = false;
			}
			else {
				currentTime += 1;
			}

			if (canMove) {
				Animar(graficador);
				Mover(graficador);
			}
			else {
				indiceX = 5;
				DefinirMinMaxIndice(5,9,false);

				estadoActual = EstadosBala::Impact;
			}
			break;
		case EstadosBala::Impact:
			if (quickFixAreaDuration) {
				DefinirMinMaxIndice(0,5, false);
			}
			Animar(graficador);
			break;
		default:
			break;
		}
	}

	
	bool CheckCollison(Rectangle entidad) {
//Usamos area en lugar de areaObjetivo para que se vea mejor
		if (area.IntersectsWith(entidad)) {
			canMove = false;
			return true;
		}
		else {
			return false;
		}
	}

	void Animar(Graphics^ graficador) override {
		DibujarImagen(graficador);
		if (this->indiceX >= maxIndice) {
			if (loop) {
				this->indiceX = minIndice;
			}
			else {
				//Bala se destruye
				listaReferencia->Remove(this);
				delete this;
			}
		}
		else {
			this->indiceX++;
		}
	}
};


ref class CArma : public CEntidad {
public:

	CArma(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY, float _maxRadio)
		: CEntidad(ruta, _area, _subImagenesX, _subImagenesY) {
		maxRadio = _maxRadio;
	}
	CArma(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY, float _maxRadio)
		: CEntidad(_imagen, _area, _subImagenesX, _subImagenesY) {
		maxRadio = _maxRadio;
	}

	EstadosArma estadoActual = EstadosArma::Idle;


	float maxRadio;
	float xObjetivo = 0;
	float yObjetivo = 0;

	float vectorDireccionX;
	float vectorDireccionY;

	String^ bulletImageRoute = "Assets\\Player\\Proyectil\\Projectile.png";
	short bulletLifeTime = 5;
	short bulletSpeed = 25;
	short subX = 9;
	bool bulletStartsDead = false;
	short bulletWidth = 31;
	short bulletHeight = 34;

	float MultiplicadorDeArma = 1;

	//Evita el spam
	bool canShoot = true;

	void ChangeBulletDimensions(short w, short h) {
		bulletWidth = w;
		bulletHeight = h;
	}

	void ChangeSubX(short _subImagenesX) {
		subX = _subImagenesX;
	}

	void ChangeImageRoute(String^ ruta) {
		bulletImageRoute = ruta;
	}

	void ChangeBulletLifeTime(short time) {
		bulletLifeTime = time;
	}

	void ChangeBulletSpeed(short _speed) {
		bulletSpeed = _speed;
	}

	void SetDamageMultiplier(short newDmg) {
		MultiplicadorDeArma = newDmg;
	}

	void Update(Graphics^ graficador, CEntidad^ EntidadPrincipal) {
		switch (estadoActual)
		{
		case EstadosArma::Idle:
			Animar(graficador);
			Orbitar(EntidadPrincipal);
			break;
		case EstadosArma::Shooting:
			Animar(graficador);
			Orbitar(EntidadPrincipal);
			//Magic Number para definir los index de la animación idle
			if (this->indiceX >= maxIndice) {
				DefinirMinMaxIndice(0, 10, true);
				estadoActual = EstadosArma::Idle;
				indiceX = minIndice;
				canShoot = true;
			}
			break;
		default:
			break;
		}
	}

	//Cambia el objetivo hacia el cual orbitar
	void CambiarObjetivo(float newX, float newY) {
		xObjetivo = newX;
		yObjetivo = newY;
	}

	void Animar(Graphics^ graficador) override {
		DibujarImagen(graficador);
		if (this->indiceX >= maxIndice) {
			if (loop) {
				this->indiceX = minIndice;
			}
		}
		else {
			this->indiceX++;
		}
	}

	//Cambia el estado del arma e invoca balas
	void Disparar(short indiceInicial, short indiceFinal, List<CBala^>^ referenciaBala) {
		if (canShoot) {
			//Configurar Arma
			DefinirMinMaxIndice(indiceInicial, indiceFinal, false);
			estadoActual = EstadosArma::Shooting;
			indiceX = indiceInicial;
			canShoot = false;
			//Creación de la bala
			CBala^ bala = gcnew CBala(bulletImageRoute, Rectangle(area.X + area.Width / 2, area.Y + area.Height / 2, bulletWidth, bulletHeight), subX, 1, bulletSpeed, bulletLifeTime,1*MultiplicadorDeArma,referenciaBala);
			Rectangle balaArea = bala->GetArea();
			if (bulletStartsDead) {
				bala->estadoActual = EstadosBala::Impact;
				bala->quickFixAreaDuration = true;
				bala->daño = 0.22;
			}
			//Calcular posición para que no aparezca raro
			bala->SetArea(Rectangle(balaArea.X - balaArea.Width/2,balaArea.Y - balaArea.Height/2,balaArea.Width,balaArea.Height));
			//Definir animacion
			bala->DefinirMinMaxIndice(0, 4, true);
			//Definir dirección
			bala->dX = vectorDireccionX;
			bala->dY = vectorDireccionY;
			referenciaBala->Add(bala);
		}
	}

private:
	//Este metodo calcula el vector dirección con respecto al "xObjetivo" y el "yObjetivo"
	//Luego de obtener el vector dirección lo multiplica por el "maxRadio" para definir el nuevo "X" y "Y" del area
	void Orbitar(CEntidad^ EntidadCentral) {
		//Valores finales para "X" y "Y"
		float xDestino = 0;
		float yDestino = 0;

		//Se el centro del objeto para trabajar
		float xCentro = EntidadCentral->GetArea().X + EntidadCentral->GetArea().Width / 2;
		float yCentro = EntidadCentral->GetArea().Y + EntidadCentral->GetArea().Height / 2;

		//Vector Dirección
		vectorDireccionX = xObjetivo - xCentro;
		vectorDireccionY = yObjetivo - yCentro;

		//Distancia empleada para normalizar el Vector Dirección
		float distancia = Math::Sqrt(Math::Pow(vectorDireccionX, 2) + Math::Pow(vectorDireccionY, 2));

		//Se Normaliza el Vector Dirección para que sus valores se encuentren entre 0 y 1
		vectorDireccionX = vectorDireccionX / distancia;
		vectorDireccionY = vectorDireccionY / distancia;

		//Se asigna los valores para las coordenadas de la ubicación final
		//debido a que la dirección ya se ha normalizado el valor máximo que puede tomar es el del "maxRadio"
		xDestino = xCentro + (vectorDireccionX * maxRadio);
		yDestino = yCentro + (vectorDireccionY * maxRadio);

		//Debido a que la posición se encuentra en la esquina dependiendo del destino se 'suma' o 'resta' el 'ancho' o 'alto' del objeto
		xDestino = (xDestino > 0) ? (xDestino - area.Width / 2) : (xDestino + area.Width / 2);
		yDestino = (yDestino > 0) ? (yDestino - area.Height / 2) : (yDestino + area.Height / 2);

		this->SetArea(Rectangle(xDestino, yDestino, this->GetArea().Width, this->GetArea().Height));
	}
};

ref class CPlayer : public CMovil {
public:
	CPlayer(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed)
		: CMovil(ruta, _area, _subImagenesX, _subImagenesY, _speed) {
		armas = gcnew List<CArma^>();
		balas = gcnew List<CBala^>();
		originalWidth = area.Width;
		originalHeight = area.Height;
	}
	CPlayer(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed)
		: CMovil(_imagen, _area, _subImagenesX, _subImagenesY, _speed) {
		armas = gcnew List<CArma^>();
		balas = gcnew List<CBala^>();
		originalWidth = area.Width;
		originalHeight = area.Height;
	}

	~CPlayer() {
		for (int i = 0; i < armas->Count; i++)
		{
			delete armas[i];
		}
		for (int i = 0; i < balas->Count; i++)
		{
			delete balas[i];
		}
	}

	List<CBala^>^ balas;
	List<CArma^>^ armas;

	PlayerStates estadoActual;

	float originalWidth;
	float originalHeight;
	
	float invincibilityFrames = 10;
	float currentFrames;
	float runSpeed = 4;

	short currentWeapon = 0;

	int maximasMedidas = 2;

	void Update(Graphics^ graficador) {
		for (short i = 0; i < balas->Count; i++)
		{
			balas[i]->Update(graficador);
		}
		//for (short i = 0; i < armas->Count; i++)
		//{
			armas[currentWeapon]->Update(graficador,(CEntidad^)this);
		//}
		Animar(graficador);
		Mover(graficador);

		Pen^ pen = gcnew Pen(Color::LightSteelBlue);
		switch (estadoActual)
		{
		case PlayerStates::Play:

			break;
		case PlayerStates::Damage:
			//Lapiz para poder ver los limites del sprite
			graficador->DrawRectangle(pen, area);

			if (currentFrames >= invincibilityFrames) {
				speed -= runSpeed;
				currentFrames = 0;
				estadoActual = PlayerStates::Play;
			}
			else {
				currentFrames += 0.6;
			}
			break;
		default:
			break;
		}

		delete pen;
	}

	void TakeDamage(short playerAmmount) {
		if (estadoActual != PlayerStates::Damage) {
			ChangeVida(playerAmmount);
			speed += runSpeed;
			estadoActual = PlayerStates::Damage;
		}
	}

	void ChangeDamageMultiplier() {
		for (short i = 0; i < armas->Count; i++)
		{
			armas[i]->SetDamageMultiplier(CManagerDeNivel::GetInstance()->GetNumeroNivel()+1);
		}
	}
	
	void ChangeCurrentWeapon(short weapon) {
		currentWeapon = weapon;
	}

	void AgregarArma(CArma^ arma) {
		armas->Add(arma);
	}

	void Disparar(short indiceInicial, short indiceFinal) {
		//for (int i = 0; i < armas->Count; i++)
		//{
			
			armas[currentWeapon]->Disparar(indiceInicial, indiceFinal,balas);
		//}
	}

	void ResetArea() {
		area.Width = originalWidth;
		area.Height = originalHeight;
	}

	void Crecer(float cantidad/*De 0 a 1*/) {
		area.Width += area.Width*cantidad;
		area.Height += area.Height*cantidad;
		if (area.Width > originalWidth*maximasMedidas) {
			area.Width = originalWidth * maximasMedidas;
		}
		if (area.Height > originalHeight*maximasMedidas) {
			area.Height = originalHeight*maximasMedidas;
		}
	}
};

ref class CEnemigoMenor : public CMovil {
public:
	CEnemigoMenor(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed)
		: CMovil(ruta, _area, _subImagenesX, _subImagenesY, _speed) {
	}
	CEnemigoMenor(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed)
		: CMovil(_imagen, _area, _subImagenesX, _subImagenesY, _speed) {
	}

	~CEnemigoMenor() {
		
	}

	EstadosEnemigo estadoActual;
	
	float timeToChange = 7;
	float currentTime;

	float distanciaMin = 24;
	float distanciaMax = 300;

	short damage = 1;

	const short vidasIniciales = 2;

	float timeTodie = 10;
	float currentHealthStatusTime = 0;

	void Update(Graphics^ graficador) {
		Animar(graficador);
		float distance;
		PointF punto;
		float x1 = area.X + area.Width / 2;
		float y1 = area.Y + area.Height / 2;
		float x2 = CManagerDeNivel::GetInstance()->GetJugador()->GetArea().X + CManagerDeNivel::GetInstance()->GetJugador()->GetArea().Width / 2;
		float y2 = CManagerDeNivel::GetInstance()->GetJugador()->GetArea().Y + CManagerDeNivel::GetInstance()->GetJugador()->GetArea().Height / 2;
		distance = CManagerMatematico::GetInstance()->GetDistancia(x1, y1, x2, y2);

		switch (estadoActual)
		{
		case EstadosEnemigo::EIdle:
			CheckCurrentTime();
			break;
		case EstadosEnemigo::EMoving:
			Mover(graficador);
			CheckCurrentTime();
			
			if (distance <= distanciaMax) {
				estadoActual = EstadosEnemigo::EChasing;
			}
			break;
		case EstadosEnemigo::EChasing:
			punto = CManagerMatematico::GetInstance()->GetVectorDireccion(x1,y1,x2,y2);
			dX = punto.X * speed;
			dY = punto.Y * speed;
			if (distance > distanciaMin) {
				Mover(graficador);
			}
			if (distance > distanciaMax) {
				estadoActual = EstadosEnemigo::EIdle;
			}
			break;
		case EstadosEnemigo::EDying:
			if (currentHealthStatusTime >= timeTodie) {
				delete this;
			}
			else {
				currentHealthStatusTime += 0.6;
			}
			

			break;
		default:
			break;
		}
	}

private:

	void CheckCurrentTime() {
		if (currentTime >= timeToChange) {
			Random r;
			int number = r.Next(1, 101);
			if (number <= 25) {
				estadoActual = EstadosEnemigo::EIdle;
			} else if (number > 25 && number <= 100) {
				estadoActual = EstadosEnemigo::EMoving;
				float xD = r.Next(-1,2) * r.NextDouble();
				float yD = r.Next(-1,2) * r.NextDouble();
				dX = speed * xD;
				dY = speed * yD;
			}
			currentTime = 0;
		}
		else {
			currentTime += 0.5;
		}
	}
};


ref class CEnemigoMayor : public CMovil {
public:
	CEnemigoMayor(String^ ruta, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed, float _attackSpeed)
		: CMovil(ruta, _area, _subImagenesX, _subImagenesY, _speed) {
		armas = gcnew List<CArma^>();
		balas = gcnew List<CBala^>();
		attackSpeed = _attackSpeed;
	}
	CEnemigoMayor(Bitmap^ _imagen, Rectangle _area, short _subImagenesX, short _subImagenesY, float _speed, float _attackSpeed)
		: CMovil(_imagen, _area, _subImagenesX, _subImagenesY, _speed) {
		armas = gcnew List<CArma^>();
		balas = gcnew List<CBala^>();
		attackSpeed = _attackSpeed;
	}

	~CEnemigoMayor() {
		for (int i = 0; i < armas->Count; i++)
		{
			delete armas[i];
		}
		for (int i = 0; i < balas->Count; i++)
		{
			delete balas[i];
		}
	}

	List<CBala^>^ balas;
	List<CArma^>^ armas;

	EstadosEnemigo estadoActual;

	float timeToChange = 15;
	float currentTime;
	float distanciaMin = 24;
	float distanciaMax = 300;

	const short velocidadPorNivel = 1;
	const short vidasIniciales = 10;

	float attackSpeed;
	float attacktime;

	float timeTodie = 10;
	float currentHealthStatusTime = 0;

	void ChangeDistanciaMin(float newMin) {
		distanciaMin = newMin;
	}
	void ChangeDistanciaMax(float newMax) {
		distanciaMax = newMax;
	}

	void Update(Graphics^ graficador) {
		for (int i = 0; i < balas->Count; i++)
		{
			balas[i]->Update(graficador);
		}
		for (int i = 0; i < armas->Count; i++)
		{
			armas[i]->Update(graficador,(CEntidad^)this);
		}

		Animar(graficador);
		float distance;
		PointF punto;
		float x1 = area.X + area.Width / 2;
		float y1 = area.Y + area.Height / 2;
		float x2 = CManagerDeNivel::GetInstance()->GetJugador()->GetArea().X + CManagerDeNivel::GetInstance()->GetJugador()->GetArea().Width / 2;
		float y2 = CManagerDeNivel::GetInstance()->GetJugador()->GetArea().Y + CManagerDeNivel::GetInstance()->GetJugador()->GetArea().Height / 2;
		distance = CManagerMatematico::GetInstance()->GetDistancia(x1, y1, x2, y2);
		switch (estadoActual)
		{
		case EstadosEnemigo::EIdle:
			CheckCurrentTime();
			break;
		case EstadosEnemigo::EMoving:
			Mover(graficador);
			CheckCurrentTime();
			if (distance <= distanciaMax) {
				estadoActual = EstadosEnemigo::EChasing;
			}

			for (short i = 0; i < armas->Count; i++)
			{
				armas[i]->CambiarObjetivo(x1,y1 -50);
			}
			break;
		case EstadosEnemigo::EChasing:
			punto = CManagerMatematico::GetInstance()->GetVectorDireccion(x1, y1, x2, y2);
			dX = punto.X * (speed + CManagerDeNivel::GetInstance()->GetNumeroNivel()*velocidadPorNivel);
			dY = punto.Y * (speed + CManagerDeNivel::GetInstance()->GetNumeroNivel()*velocidadPorNivel);

			for (short i = 0; i < armas->Count; i++)
			{
				armas[i]->CambiarObjetivo(x2,y2);
			}

			//Revisar la distancia con el jugador
			if (distance > distanciaMin) {
				Mover(graficador);
			}
			else {
				estadoActual = EstadosEnemigo::EShoot;
				attacktime = 0;
			}
			if (distance > distanciaMax) {
				estadoActual = EstadosEnemigo::EIdle;
			}
			break;
		case EstadosEnemigo::EShoot:
			for (short i = 0; i < armas->Count; i++)
			{
				armas[i]->CambiarObjetivo(x2, y2);
			}
			if (attacktime >= attackSpeed - CManagerDeNivel::GetInstance()->GetNumeroNivel()) {
				attacktime = 0;
				Disparar(10, 16);
			}
			else {
				attacktime += 0.5;
			}
			break;
		case EstadosEnemigo::EDying:
			if (currentHealthStatusTime >= timeTodie) {
				delete this;
			}
			else {
				currentHealthStatusTime += 0.6;
			}
			break;
		default:
			break;
		}
	}

	void AgregarArma(CArma^ arma) {
		armas->Add(arma);
	}

	void Disparar(short indiceInicial, short indiceFinal) {
		for (int i = 0; i < armas->Count; i++)
		{
			armas[i]->Disparar(indiceInicial, indiceFinal, balas);
		}
	}

private:

	void CheckCurrentTime() {
		if (currentTime >= timeToChange) {
			Random r;
			int number = r.Next(1, 101);
			if (number <= 25) {
				estadoActual = EstadosEnemigo::EIdle;
			}
			else if (number > 25 && number <= 100) {
				estadoActual = EstadosEnemigo::EMoving;
				float xD = r.Next(-1, 2) * r.NextDouble();
				float yD = r.Next(-1, 2) * r.NextDouble();
				dX = speed * xD;
				dY = speed * yD;
			}
			currentTime = 0;
		}
		else {
			currentTime += 0.5;
		}
	}
};

ref class CMainGameManager
{
public:
	static CMainGameManager^ instance;

	List<CEnemigoMayor^>^ eneMayor;
	List<CEnemigoMenor^>^ eneMenor;
	CPlayer^ mainPlayer;

	short enemigosEsteNivel;
	short enemigosActuales;

	short minutosPorPartida = 3;
	float tiempoActual;

	short vidasIniciales = 10;
	bool juegoEnProgreso;

	CMainGameManager(CPlayer^jugador) {
		instance = this;
		mainPlayer = jugador;
		eneMayor = gcnew List<CEnemigoMayor^>();
		eneMenor = gcnew List<CEnemigoMenor^>();
		labels = gcnew List<Label^>();
		buttons = gcnew List<Button^>();
	}
	~CMainGameManager() {
		//Agregados deletes luego de la revision
		for (short i = 0; i < eneMayor->Count; i++)
		{
			delete eneMayor[i];
		}
		delete eneMayor;
		for (short i = 0; i < eneMenor->Count; i++)
		{
			delete eneMenor[i];
		}
		delete eneMenor;
		for (short i = 0; i < labels->Count; i++)
		{
			delete labels[i];
		}
		delete labels;
		for (short i = 0; i < buttons->Count; i++)
		{
			delete buttons[i];
		}
		delete buttons;
		delete mainTimer;
		delete timeText;
		//No eliminamos al mainPlayer porque se elimina dentro del Form
	}

	Timer^ mainTimer;
	List<Label^>^ labels;
	Label^ timeText;
	Label^ pauseText;
	List<Button^>^ buttons;

	short startingEnemys;
	short currentEnemys;

	void StartNivel() {
		
		for (short i = 0; i < eneMayor->Count; i++)
		{
			eneMayor[i]->balas->Clear();
			eneMayor[i]->armas->Clear();
		}
		eneMayor->Clear();
		eneMenor->Clear();
		startingEnemys = 0;
		currentEnemys = 0;

		mainPlayer->ChangeDamageMultiplier();

		short add = CManagerDeNivel::GetInstance()->GetNumeroNivel();
		//Enemigos Menores
		Random r;
		short numeroDeMenores = r.Next(10, 15);
		numeroDeMenores += numeroDeMenores * (0.1*add);
		for (short i = 0; i < numeroDeMenores; i++)
		{
			CEnemigoMenor^ temp = gcnew CEnemigoMenor("Assets\\MinorEnemy\\Idle.png", Rectangle(0, 0, 32, 32), 6, 1, 2);
			short x = r.Next(temp->GetArea().Width,CManagerDeNivel::GetInstance()->GetAreaDeVentana().Width - temp->GetArea().Width);
			short y = r.Next(temp->GetArea().Height, CManagerDeNivel::GetInstance()->GetAreaDeVentana().Height - temp->GetArea().Height);
			temp->SetArea(Rectangle(x,y,temp->GetArea().Width,temp->GetArea().Height));
			temp->SetVida(temp->vidasIniciales + add);
			eneMenor->Add(temp);
			currentEnemys++;
		}
		//Enemigos Mayores
		short numeroDeMayores = r.Next(3, 6 + add);
		for (short i = 0; i < numeroDeMayores; i++)
		{
			CEnemigoMayor^ temp = gcnew CEnemigoMayor("Assets\\MajorEnemy\\Idle.png", Rectangle(500, 0, 64, 64), 8, 1, 2+add,20);
			CArma^ tempArma = gcnew CArma("Assets\\MajorEnemy\\Orb.png", Rectangle(0, 0, 32, 32), 16, 1,32);
			tempArma->DefinirMinMaxIndice(0, 10, true);
			tempArma->ChangeImageRoute("Assets\\MajorEnemy\\Projectile.png");
			short x = r.Next(temp->GetArea().Width, CManagerDeNivel::GetInstance()->GetAreaDeVentana().Width - temp->GetArea().Width);
			short y = r.Next(temp->GetArea().Height, CManagerDeNivel::GetInstance()->GetAreaDeVentana().Height - temp->GetArea().Height);
			temp->SetArea(Rectangle(x, y, temp->GetArea().Width, temp->GetArea().Height));
			temp->SetVida(temp->vidasIniciales + add);

			temp->AgregarArma(tempArma);
			temp->ChangeDistanciaMin(100 + add*20);
			temp->ChangeDistanciaMax(200 + add*20);
			eneMayor->Add(temp);
			currentEnemys++;
		}
		startingEnemys = currentEnemys;
	}

	void SetTimeText(Label^ text) {
		timeText = text;
	}
	void SetMainTimer(Timer^ timer) {
		mainTimer = timer;
	}

	void SetPauseText(Label^ text) {
		pauseText = text;
	}

	void AlternateMainTimer() {
		mainTimer->Enabled = !mainTimer->Enabled;
		if (juegoEnProgreso) {
			pauseText->Enabled = !mainTimer->Enabled;
			pauseText->Visible = !mainTimer->Enabled;
			timeText->Enabled = mainTimer->Enabled;
			timeText->Visible = mainTimer->Enabled;
		}
	}

	void AgregarBoton(Button^ button){
		buttons->Add(button);
	}

	void AgregarTexto(Label^ label) {
		labels->Add(label);
	}

	void SetMenuVisibility(bool isVisible) {
		for (short i = 0; i < labels->Count; i++)
		{
			labels[i]->Visible = isVisible;
			labels[i]->Enabled = isVisible;
		}
		for (short i = 0; i < buttons->Count; i++)
		{
			buttons[i]->Visible = isVisible;
			buttons[i]->Enabled = isVisible;
		}
	}
	
	void YouWin(Graphics^ graficador) {
		graficador->Clear(Color::White);
		labels[2]->Text = "GANASTE";
		labels[3]->Text = "GANASTE";
		buttons[0]->Text = "GANASTE";
		AlternateMainTimer();
		juegoEnProgreso = false;
		SetMenuVisibility(true);
	}

	void MainUpdate(Graphics^ graficador) {
		timeText->Text = Math::Round(tiempoActual).ToString();
		if (juegoEnProgreso)
		{
			mainPlayer->Update(graficador);
			for (int i = 0; i < eneMenor->Count; i++)
			{
				eneMenor[i]->Update(graficador);
			}
			for (int i = 0; i < eneMayor->Count; i++)
			{
				eneMayor[i]->Update(graficador);
			}

			CheckColision();
			if (tiempoActual < 0 || mainPlayer->vida <= 0) {
				GameOver(graficador);
			}
			else {
				if (currentEnemys <= startingEnemys/2) {
					if (CManagerDeNivel::GetInstance()->GetNumeroNivel() < CManagerDeNivel::GetInstance()->GetNivelMaximo() - 1) {
						CManagerDeNivel::GetInstance()->SubirNivel();
						StartNivel();
					}
					else {
						YouWin(graficador);
						GameOver(graficador);
					}
				}
				tiempoActual -= 0.06;
			}
		}
	}

	void CheckColision() {
		for (short i = 0; i < mainPlayer->balas->Count; i++)
		{
			for (short j = 0; j < eneMayor->Count; j++)
			{
				if (mainPlayer->balas[i]->CheckCollison(eneMayor[j]->GetArea()) && (mainPlayer->balas[i]->estadoActual != EstadosBala::Impact || mainPlayer->balas[i]->quickFixAreaDuration)) {
					eneMayor[j]->ChangeVida(-mainPlayer->balas[i]->daño);
					if (eneMayor[j]->vida <= 0) {
						eneMayor[j]->estadoActual = EstadosEnemigo::EDying;
						eneMayor->Remove(eneMayor[j]);
						currentEnemys--;
						mainPlayer->ChangeVida(5);
						mainPlayer->Crecer(0.08);
					}
				}
			}
			for (short j = 0; j < eneMenor->Count; j++)
			{
				if (mainPlayer->balas[i]->CheckCollison(eneMenor[j]->GetArea()) && (mainPlayer->balas[i]->estadoActual != EstadosBala::Impact || mainPlayer->balas[i]->quickFixAreaDuration)) {
					eneMenor[j]->ChangeVida(-mainPlayer->balas[i]->daño);
					if (eneMenor[j]->vida <= 0) {
						eneMenor[j]->estadoActual = EstadosEnemigo::EDying;
						eneMenor->Remove(eneMenor[j]);
						currentEnemys--;
						mainPlayer->ChangeVida(1);
						mainPlayer->Crecer(0.05);
					}
				}
			}
		}
		for (short i = 0; i < eneMayor->Count; i++)
		{
			for (int j = 0; j < eneMayor[i]->balas->Count; j++)
			{
				if (eneMayor[i]->balas[j]->CheckCollison(mainPlayer->GetArea()) && eneMayor[i]->balas[j]->estadoActual != EstadosBala::Impact) {
					mainPlayer->TakeDamage(-eneMayor[i]->balas[j]->daño);
				}
			}
		}
		for (short i = 0; i < eneMenor->Count; i++)
		{
			if (eneMenor[i]->GetArea().IntersectsWith(mainPlayer->GetArea())) {
				mainPlayer->TakeDamage(-eneMenor[i]->damage);
			}
		}
	}

	void GameOver(Graphics^ graficador) {
		graficador->Clear(Color::White);
		AlternateMainTimer();
		juegoEnProgreso = false;
		SetMenuVisibility(true);
	}

	void StartGame() {
		CManagerDeNivel::GetInstance()->CambiarDeNivel(0);
		mainPlayer->SetVida(vidasIniciales);
		SetMenuVisibility(false);
		juegoEnProgreso = true;
		tiempoActual = minutosPorPartida * 60;

		labels[2]->Text = "Vidas";
		labels[3]->Text = "Minutos de Juego";
		buttons[0]->Text = "INICIAR JUEGO";

		MessageBox::Show("TECLAS:\nMovimiento = Flechas\nDisparo = Mouse Click\nCambiar Ataque = NumPad1 y NumPad2 / Q y W");

		mainTimer->Enabled = false;
		StartNivel();
	}

	void ChangeLives(short ammount) {
		vidasIniciales += ammount;
		if (vidasIniciales < minVidas) {
			vidasIniciales = minVidas;
		}
		else if (vidasIniciales > maxVidas) {
			vidasIniciales = maxVidas;
		}
	}

	void ChangeTime(short ammount) {
		minutosPorPartida += ammount;
		if (minutosPorPartida < minTiempo) {
			minutosPorPartida = minTiempo;
		}
		else if (minutosPorPartida > maxTiempo) {
			minutosPorPartida = maxTiempo;
		}
	}

private:
	short minVidas = 10;
	short maxVidas = 100;

	short minTiempo = 3;
	short maxTiempo = 10;
};
