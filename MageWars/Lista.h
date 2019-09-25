#pragma once
#include "Nodo.h"

//Eliminado todos los T NADA = 0



template<typename T>
ref class Lista
{
	Nodo<T>^ ini;
	Nodo<T>^ aux;
	Nodo<T>^ nuevo;

public:
	unsigned int lon = 0;

	Lista(): ini(nullptr), lon(0) {}
	~Lista(){}

	void AgregarInicio(T elem) {
		nuevo = gcnew Nodo<T>(elem, ini, nullptr);
		if (lon > 0) {
			ini->ant = nuevo;	
		}
		 ini= nuevo;
		
		lon++;
	}
	void AgregarFinal(T elem) {
		aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		nuevo = gcnew Nodo<T>(elem, nullptr, aux);
		if (lon > 0) {
			aux->sig = nuevo;
		}
		else
		{
			ini = nuevo;
		}
	
		lon++;
	}
	void AgregarPosicion(T elem, unsigned int pos) {
		if (pos > 0 && pos <= lon + 1) {
			aux = ini;
			for (unsigned int i = 1; i < pos; ++i) {
				aux = aux->sig;
			}
			if (pos == 1) {
				AgregarInicio(elem);
			}
			else if (pos == lon + 1) {
				AgregarFinal(elem);
			}
			else {
				nuevo = gcnew Nodo<T>(elem, aux, aux->ant);
				nuevo->ant->sig = nuevo;
				nuevo->sig->ant = nuevo;

				lon++;

			}
		}
		else {

		}//error;
	}
	void EliminarInicio() {
		if (lon > 0) {
			aux = ini;
			ini = aux->sig;
			if (lon > 1) {
				ini->ant = nullptr;
			}
			delete aux;
			--lon;
		}
		else {

		}//error;
	}
	void EliminarFinal() {
		if (lon > 0) {
			aux = ini;
			for (unsigned int i = 1; i < lon; ++i) {
				aux = aux->sig;
			}
			if (lon > 1) {
				aux->ant->sig = nullptr;
			}
			else ini = nullptr;

			delete aux;
			--lon;
		}
		else {

		}//error;
	}
	void EliminarPosicion(unsigned int pos) {
		if (lon > 0) {
			if (pos == 1) {
				EliminarInicio();
			}
			else if (pos == lon) {
				EliminarFinal();
			}
			else {
				aux = ini;
				for (unsigned int i = 1; i < pos; ++i) {
					aux = aux->sig;
				}
				if (lon > 1) {
					aux->ant->sig = aux->sig;
					aux->sig->ant = aux->ant;
				}
				else ini = nullptr;

				delete aux;
				--lon;
			}
		}
		else {

		}//error;
	}

	Nodo<T>^ At(unsigned int pos) {
		aux = ini;
		for (unsigned int i = 1; i < pos; ++i) {
			aux = aux->sig;
		}
		return aux;
	}

	T ElementoAt(unsigned int pos) {
		aux = ini;
		for (unsigned int i = 1; i < pos; ++i) {
			aux = aux->sig;
		}
		return aux->elem;
	}

	void Remove(T val) {
		aux = ini;
		for (unsigned int i = 1; i <= lon; ++i) {
			if (aux->elem == val) {
				EliminarPosicion(i);
				--i;
			}
			else aux = aux->sig;
		}
	}

	void BorrarTodo() {
		while (lon > 0) {
			EliminarFinal();
		}
	}
};
