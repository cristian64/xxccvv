#ifndef SETAESTRELLA_H
#define SETAESTRELLA_H

#include <algorithm>
#include <iostream>
#include <list>

#include "AStarNode.h"

using namespace std;

class SetAEstrella {
private:

	/**
	 * @brief Clase auxiliar que representa el comparador que requiere std::set<T, Comparator>.
	 * El comparador utiliza determinados atributos de los nodos para poder ordenarlos y acceder
	 * a ellos rápidamente.
	 * No importa qué criterio se sigan porque esta clase (la clase que utiliza el comparador) sólo
	 * es un conjunto de nodos. Es decir, no interesa el orden, sólo saber si un nodo está o no en el conjunto.
	 * Antes de comparar el tablero se utilizan otros atributos más ligeros.
	 */
	class Comparator {
	public:

		bool operator() (const AStarNode* n1, const AStarNode* n2) const {
			bool comparation = false;

			// Internamente, set<...> ordenará el conjunto según H, luego "restantes" y finalmente el propio tablero en sí.
			// Pero podría ser cualquier otro criterio, porque sólo se utiliza esta clase como conjunto matemático sin orden.
			// De hecho, los métodos que se proporcionan no permiten extraer el máximo o el mínimo como sí haría un heap o
			// cola de prioridad.
			if (n1->getH() > n2->getH()) {
				comparation = true;
			} else if (n1->getH() < n2->getH()) {
				comparation = false;
			} else if (n1->getBoard()->getRemaining() > n2->getBoard()->getRemaining()) {
				comparation = true;
			} else if (n1->getBoard()->getRemaining() < n2->getBoard()->getRemaining()) {
				comparation = false;
			} else {
				int *board1 = n1->getBoard()->getBoard();
				int *board2 = n2->getBoard()->getBoard();
				int total = n1->getBoard()->getTotal();
				int memComparation = memcmp(board1, board2, total * sizeof (int));
				if (memComparation < 0) {
					comparation = true;
				} else if (memComparation > 0) {
					comparation = false;
				}
			}

			return comparation;
		}
	};

	set<AStarNode*, Comparator> list;

public:

	/**
	 * @brief Destructor. Libera la memoria de cada nodo que todavía esté introducido en la lista.
	 *
	 */
	~SetAEstrella() {
		for (set<AStarNode*, Comparator>::iterator i = list.begin(); i != list.end(); i++)
			delete *i;
	}

	/**
	 * @brief Introduce un nodo en la lista.
	 *
	 * @param nodo Nodo que se va a introducir en la lista.
	 */
	bool push(AStarNode *node) {
		pair < set<AStarNode*, Comparator>::iterator, bool> status = list.insert(node);
		if (!status.second) {
			delete node;
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
	AStarNode* getNodo(AStarNode *nodo) const {
		set<AStarNode*, Comparator>::iterator i = list.find(nodo);
		if (i == list.end()) {
			return NULL;
		} else {
			return *i;
		}
	}

	/**
	 * @return Devuelve la cantidad de elementos de la lista.
	 */
	int size() const {
		return list.size();
	}
};

#endif // SETAESTRELLA_H
