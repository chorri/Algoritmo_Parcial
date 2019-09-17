#pragma once

template<typename T, T NADA = 0>
struct Nodo
{
	T elem;
	Nodo* sig;
	Nodo* ant;

	Nodo(T elem = NADA, Nodo*sig = nullptr, Nodo*ant = nullptr) : elem(elem), sig(sig), ant(ant) {}
};