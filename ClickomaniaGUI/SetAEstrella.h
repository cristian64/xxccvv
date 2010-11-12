#ifndef SETAESTRELLA_H
#define SETAESTRELLA_H

#include <algorithm>
#include <iostream>
#include <list>

#include "NodoAEstrella.h"

using namespace std;

class SetAEstrella
{
private:
	list<NodoAEstrella*> lista;

public:

	/**
	 * @brief Destructor. Libera la memoria de cada nodo que todavía esté introducido en la lista.
	 *
	*/
	~SetAEstrella()
	{
		//TODO liberar memoria de las listas.
	}

	static bool comparador1(NodoAEstrella* n1, NodoAEstrella* n2)
	{
		return n1->getF() > n2->getF();
	}

	static bool comparador2(NodoAEstrella* n1, NodoAEstrella* n2)
	{
		return n1->getH() > n2->getH();
	}

	/**
	 * @brief Introduce un nodo en la lista.
	 *
	 * @param nodo Nodo que se va a introducir en la lista.
	*/
	void push(NodoAEstrella *nodo)
	{
		lista.push_back(nodo);
	}

	void sort1()
	{
		lista.sort(comparador1);
	}

	void sort2()
	{
		lista.sort(comparador2);
	}

	void show() const
	{
		if (lista.size() >= 2)
		{
			cout << "F: " << lista.front()->getF() << " " << lista.back()->getF() << endl;
			cout << "G: " << lista.front()->getG() << " " << lista.back()->getG() << endl;
			cout << "H: " << lista.front()->getH() << " " << lista.back()->getH() << endl;
		}
	}

	/**
	 * @brief Elimina el elemento y devuelve la referencia.
	 *
	 * @return Devuelve una referencia al elemento del conjunto.
	*/
	NodoAEstrella* pop()
	{
		NodoAEstrella* auxiliar = lista.front();
		lista.erase(lista.begin());
		return auxiliar;
	}

	/**
	 * @brief Obtiene una referencia al primer elemento de la lista pero sin eliminar el elemento.
	 *
	 * @return Devuelve una referencia al primer elemento de la lista.
	*/
	const NodoAEstrella* top() const
	{
		return lista.front();
	}

	/**
	 * @brief Devuelve una referencia al nodo que coincida con el nodo indicado en los parámetros.
	 * Es decir, el nodo será esencialmente el mismo, pero en otra posición de memoria diferente.
	 *
	 * @param nodo Nodo que se va a comprobar si ya está en la lista.
	 * @return Devuelve la referencia al nodo que ya estaba en la lista y que coincide con el indicado por parámetros. Si no existe, devuelve NULL.
	*/
	NodoAEstrella* getNodo(const NodoAEstrella *nodo) const
	{
		for (list<NodoAEstrella*>::const_iterator i = lista.begin(); i != lista.end(); i++)
		{
			if ((*i)->operator==(*nodo))
				return *i;
		}
		return NULL;
	}

	/**
	 * @brief Devuelve una referencia al nodo que coincida con el nodo indicado en los parámetros.
	 * Es decir, el nodo devuelto será esencialmente el mismo que el indicado, pero en otra posición de memoria diferente.
	 * Esa referencia será eliminada de la lista.
	 *
	 * @param nodo Nodo que se va a comprobar si ya está en la lista para eliminarlo.
	 * @return Devuelve la referencia al nodo que ya estaba en la lista y que coincide con el indicado por parámetros. Si no existe, devuelve NULL.
	*/
	NodoAEstrella* remove(const NodoAEstrella *nodo)
	{
		for (list<NodoAEstrella*>::iterator i = lista.begin(); i != lista.end(); i++)
		{
			if ((*i)->operator==(*nodo))
			{
				NodoAEstrella *eliminado = *i;
				lista.erase(i);
				return eliminado;
			}
		}
		return NULL;
	}

	/**
	 * @return Devuelve verdadero si la lista está vacía.
	*/
	bool empty() const
	{
		return lista.empty();
	}


	/**
	 * @return Devuelve la cantidad de elementos de la lista.
	*/
	int size() const
	{
		return lista.size();
	}
};

#endif // SETAESTRELLA_H
