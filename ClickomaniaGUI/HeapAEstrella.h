#ifndef HEAPAESTRELLA_H
#define HEAPAESTRELLA_H

#include <algorithm>
#include <iostream>
#include <list>

#include "NodoAEstrella.h"

using namespace std;



class HeapAEstrella
{
private:

	/**
	 * @brief Clase auxiliar que representa el comparador que requiere std::set<T, Comparator>.
	 * El comparador utiliza determinados atributos de los nodos para poder ordenarlos y acceder
	 * a ellos ordenadamente y rápidamente.
	*/
	class Comparador
	{
		public:

			bool operator() (const NodoAEstrella* n1, const NodoAEstrella* n2) const
			{
				bool comparacion = false;

				// Internamente, set<...> ordenará el conjunto según F, luego "restantes" y finalmente el propio tablero en sí.
				if (n1->getF() > n2->getF())
					comparacion = true;
				else if (n1->getF() < n2->getF())
					comparacion = false;
				else if (n1->getBoard()->getRestantes() > n2->getBoard()->getRestantes())
					comparacion = true;
				else if (n1->getBoard()->getRestantes() < n2->getBoard()->getRestantes())
					comparacion = false;
				else
				{
					int *tablero1 = n1->getBoard()->getBoard();
					int *tablero2 = n2->getBoard()->getBoard();
					int total = n1->getBoard()->getTotal();
					for (int i = 0; i < total; i++)
					{
						if (tablero1[i] < tablero2[i])
							comparacion = true;
						else if (tablero1[i] > tablero2[i])
							comparacion = false;
						else
							continue;
						break;
					}
				}

				return comparacion;
			}
	};

	set<NodoAEstrella*, Comparador> lista;

public:

	/**
	 * @brief Destructor. Libera la memoria de cada nodo que todavía esté introducido en la lista.
	 *
	*/
	~HeapAEstrella()
	{
		for (set<NodoAEstrella*, Comparador>::iterator i = lista.begin(); i != lista.end(); i++)
			delete *i;
	}

	/**
	 * @brief Introduce un nodo en la lista.
	 *
	 * @param nodo Nodo que se va a introducir en la lista.
	*/
	void push(NodoAEstrella *nodo)
	{
		lista.insert(nodo);
	}

	/**
	 * @brief Devuelve una referencia al nodo que coincida con el nodo indicado en los parámetros.
	 * Es decir, el nodo será esencialmente el mismo, pero en otra posición de memoria diferente.
	 *
	 * @param nodo Nodo que se va a comprobar si ya está en la lista.
	 * @return Devuelve la referencia al nodo que ya estaba en la lista y que coincide con el indicado por parámetros. Si no existe, devuelve NULL.
	*/
	NodoAEstrella* getNodo(NodoAEstrella *nodo) const
	{
		set<NodoAEstrella*, Comparador>::iterator i = lista.find(nodo);
		if (i == lista.end())
			return NULL;
		else
			return *i;
	}

	/**
	 * @brief Elimina el elemento y devuelve la referencia.
	 *
	 * @return Devuelve una referencia al elemento del conjunto.
	*/
	NodoAEstrella* pop()
	{
		set<NodoAEstrella*, Comparador>::iterator i = lista.begin();
		if (i == lista.end())
			return NULL;
		else
		{
			NodoAEstrella* cima = *i;
			lista.erase(i);
			return cima;
		}
	}

	/**
	 * @brief Obtiene una referencia al primer elemento de la lista pero sin eliminar el elemento.
	 *
	 * @return Devuelve una referencia al primer elemento de la lista.
	*/
	const NodoAEstrella* top() const
	{
		set<NodoAEstrella*, Comparador>::iterator i = lista.begin();
		if (i == lista.end())
			return NULL;
		else
			return *i;
	}

	/**
	 * @brief Esa referencia será eliminada de la lista.	 *
	 *
	 * @param nodo Nodo que se va a comprobar si ya está en la lista para eliminarlo.
	*/
	void remove(NodoAEstrella *nodo)
	{
		lista.erase(nodo);
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

#endif // HEAPAESTRELLA_H
