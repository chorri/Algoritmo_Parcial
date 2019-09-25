#pragma once

template<typename T>
ref class Nodo
{
public:
	T elem;
	Nodo^ sig;
	Nodo^ ant;


	Nodo(T elem, Nodo^sig, Nodo^ant) : elem(elem), sig(sig), ant(ant) {}
};
//Clase deprecada