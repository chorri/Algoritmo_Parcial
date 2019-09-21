#pragma once
#include "Nodo.h"

template<typename T, T NADA = 0>
class Lista
{
	Nodo* ini;
	unsigned int lon;

public:
	Lista(): ini(nullptr), lon(0) {}
	~Lista(){}

	void AgregarInicio(T elem) {
		Nodo* nuevo = new Nodo(elem, ini, nullptr);
		if (lon > 0) {
			ini->ant = nuevo;	
		}
		ini = nuevo;
		
		++lon;
	}
	void AgregarFinal(T elem) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		Nodo* nuevo = new Nodo(elem, nullptr, aux);
		if (lon > 0) {
			aux->sig = nuevo;
		}
		else ini = nuevo;

		++lon;
	}
	void AgregarPosicion(T elem, unsigned int pos) {
		if (pos > 0 && pos <= lon + 1) {
			Nodo* aux = ini;
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
				Nodo* nuevo = new Nodo(elem, aux, aux->ant);
				nuevo->ant->sig = nuevo;
				nuevo->sig->ant = nuevo;

				++lon;
			}
		}
		else //error;
	}
	void EliminarInicio() {
		if (lon > 0) {
			Nodo* aux = ini;
			ini = aux->sig;
			if (lon > 1) {
				ini->ant = nullptr;
			}
			delete aux;
			--lon;
		}
		else //error;
	}
	void EliminarFinal() {
		if (lon > 0) {
			Nodo* aux = ini;
			for (unsigned int = 1; i < lon; ++i) {
				aux = aux->sig;
			}
			if (lon > 1) {
				aux->ant->sig = nullptr;
			}
			else ini = nullptr;

			delete aux;
			--lon;
		}
		else //error;
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
				Nodo* aux = ini;
				for (unsigned int = 1; i < pos; ++i) {
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
		else //error;
	}

	Nodo* At(unsigned int pos) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i < pos; ++i) {
			aux = aux->sig;
		}
		return aux;
	}

	void Remove(T val) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i <= lon; ++i) {
			if (aux->elem == val) {
				EliminarPosicion(i);
				--i;
			}
			else aux = aux->sig;
		}
	}

	void borrarTodo() {
		while (lon > 0) {
			EliminarFinal();
		}
	}
};
