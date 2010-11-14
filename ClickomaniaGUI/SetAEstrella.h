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

	/**
	 * @brief Clase auxiliar que representa el comparador que requiere std::set<T, Comparator>.
	 * El comparador utiliza determinados atributos de los nodos para poder ordenarlos y acceder
	 * a ellos rápidamente.
	 * No importa qué criterio se sigan porque esta clase (la clase que utiliza el comparador) sólo
	 * es un conjunto de nodos. Es decir, no interesa el orden, sólo saber si un nodo está o no en el conjunto.
	 * Antes de comparar el tablero se utilizan otros atributos más ligeros.
	*/
	class Comparador
	{
		public:

			bool operator() (const NodoAEstrella* n1, const NodoAEstrella* n2) const
			{
				bool comparacion = false;

				// Internamente, set<...> ordenará el conjunto según H, luego "restantes" y finalmente el propio tablero en sí.
				// Pero podría ser cualquier otro criterio, porque sólo se utiliza esta clase como conjunto matemático sin orden.
				// De hecho, los métodos que se proporcionan no permiten extraer el máximo o el mínimo como sí haría un heap o
				// cola de prioridad.
				/*if (n1->getHash() > n2->getHash())
					comparacion = true;
				else if (n1->getHash() < n2->getHash())
					comparacion = false;*/
				if (n1->getH() > n2->getH())
					comparacion = true;
				else if (n1->getH() < n2->getH())
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
					int resultado = memcmp(tablero1, tablero2, total * sizeof(int));
					if (resultado < 0)
						comparacion = true;
					else if (resultado > 0)
						comparacion = false;
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
	~SetAEstrella()
	{
		for (set<NodoAEstrella*, Comparador>::iterator i = lista.begin(); i != lista.end(); i++)
			delete *i;
	}

	/**
	 * @brief Introduce un nodo en la lista.
	 *
	 * @param nodo Nodo que se va a introducir en la lista.
	*/
	bool push(NodoAEstrella *nodo)
	{
		pair<set<NodoAEstrella*, Comparador>::iterator, bool> resultado = lista.insert(nodo);
		if (!resultado.second)
		{
			delete nodo;
			return false;
		}
		return true;
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
	 * @return Devuelve la cantidad de elementos de la lista.
	*/
	int size() const
	{
		return lista.size();
	}
};

#endif // SETAESTRELLA_H
