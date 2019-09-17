#pragma once
#include "Nodo.h"

template<typename T, T NADA = 0>
class Cola
{
	struct Nodo;

	Nodo* ini;
	unsigned int lon;

public:
	Cola(): ini(nullptr), lon(0) {}
	~Cola(){}

	void Enqueue(T elem) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		Nodo* nuevo = new Nodo(elem, nullptr, aux);
		aux->sig = nuevo;

		++lon;
	}
	T Dequeue() {
		T devolver;
		Nodo* aux = ini;
		ini = aux->sig;
		ini->ant = nullptr;

		devolver = aux->elem;
		delete aux;
		return devolver;
	}
};
