#pragma once
#include "Nodo.h"

template<typename T>
ref class Cola
{
	Nodo<T>^ ini;
	Nodo<T>^ fina;
	Nodo<T>^ aux;

	unsigned int lon;

public:
	Cola(): ini(nullptr), lon(0) {}
	~Cola(){}

	void Enqueue(T elem) {
		aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		Nodo<T>^ nuevo = new Nodo(elem, nullptr, aux);
		aux->sig = nuevo;

		++lon;
	}
	T Dequeue() {
		T devolver;
		aux = ini;
		ini = aux->sig;
		ini->ant = nullptr;

		devolver = aux->elem;
		delete aux;
		return devolver;
	}

	void BorrarTodo() {
		while (lon > 0) {
			aux = ini;
			for (unsigned int = 1; i < lon; ++i) {
				aux = aux->sig;
			}
			aux->ant->sig = nullptr;

			delete aux;
		}
	}
};
