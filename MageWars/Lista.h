#pragma once
#include "Nodo.h"

template<typename T, T NADA = 0>
class Lista
{
	struct Nodo;

	Nodo* ini;
	unsigned int lon;

public:
	Lista(): ini(nullptr), lon(0) {}
	~Lista(){}

	void AgregarInicio(T elem) {
		Nodo* nuevo = new Nodo(elem, ini, nullptr);
		ini->ant = nuevo;
		ini = nuevo;

		++lon;
	}
	void AgregarFinal(T elem) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		Nodo* nuevo = new Nodo(elem, nullptr, aux);
		aux->sig = nuevo;

		++lon;
	}
	void AgregarPosicion(T elem, unsigned int pos) {
		Nodo* aux = ini;
		for (unsigned int i = 1; i < pos; ++i) {
			aux = aux->sig;
		}
		Nodo* nuevo = new Nodo(elem, aux, aux->ant);
		nuevo->ant->sig = nuevo;
		nuevo->sig->ant = nuevo;

		++lon;
	}
	void EliminarInicio() {
		Nodo* aux = ini;
		ini = aux->sig;
		ini->ant = nullptr;

		delete aux;
	}
	void EliminarFinal() {
		Nodo* aux = ini;
		for (unsigned int = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		aux->ant->sig = nullptr;

		delete aux;
	}
	void EliminarPosicion(unsigned int pos) {
		Nodo* aux = ini;
		for (unsigned int = 1; i < pos; ++i) {
			aux = aux->sig;
		}
		aux->ant->sig = aux->sig;
		aux->sig->ant = aux->ant;

		delete aux;
	}
};

/*
template<typename T, T NADA>
struct Lista<T, NADA>::Nodo
{
	T elem;
	Nodo* sig;
	Nodo* ant;

	Nodo(T elem = NADA, Nodo*sig = nullptr, Nodo*ant= nullptr): elem(elem), sig(sig), ant(ant) {}
};
*/