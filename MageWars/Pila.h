#pragma once
#include "Nodo.h"

template<typename T, T NADA = 0>
class Pila
{
	struct Nodo;

	Nodo* ini;
	unsigned int lon;

public:
	Pila(): ini(nullptr), lon(0){}
	~Pila(){}

	void PUSH(T elem) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		Nodo* nuevo = new Nodo(elem, nullptr, aux);
		aux->sig = nuevo;

		++lon;
	}
	T PULL() {
		T devolver;
		Nodo* aux = ini;
		for (unsigned int = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		aux->ant->sig = nullptr;

		devolver = aux->elem;
		delete aux;
		return devolver;
	}
};
