#pragma once
#include "Nodo.h"

template<typename T>
ref class Pila
{
public:
	Nodo<T>^ ini;
	Nodo<T>^ aux;
	unsigned int lon;

	Pila(): ini(nullptr), lon(0){}
	~Pila(){}

	T Peek(int n) {
		aux = ini;
		for (unsigned int i = 1; i < n; ++i) {
			aux = aux->sig;
		}
		return aux->elem;
	}

	void PUSH(T elem) {
		aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		Nodo<T>^ nuevo = gcnew Nodo<T>(elem, nullptr, aux);
		if (lon > 0) {
			aux->sig = nuevo;
		}
		else {
			ini = nuevo;
		}

		++lon;
	}
	T PULL() {
		T devolver;
		aux = ini;
		for (unsigned int i = 1; i < lon; ++i) {
			aux = aux->sig;
		}
		devolver = aux->elem;
		delete aux;
		return devolver;
	}

	//Siempre iniciar como 1 para evitar errores
	void BorrarTodo(int n) {
		if (lon >= 1) {
			Nodo<T>^ actual = ini;
			for (unsigned int i = 1; i < n; ++i) {
				actual = actual->sig;
			}

			if (n < lon) {
				BorrarTodo(n + 1);
			}
			delete actual;
		}
	}


	//This is here only for testing
	void Remove(T val) {
		
	}
};
