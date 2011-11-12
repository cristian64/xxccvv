#ifndef HEAPAESTRELLA_H
#define HEAPAESTRELLA_H

#include <algorithm>
#include <iostream>
#include <list>

#include "AStarNode.h"

using namespace std;

class HeapAEstrella {
private:

	/**
	 * @brief Clase auxiliar que representa el comparador que requiere std::set<T, Comparator>.
	 * El comparador utiliza determinados atributos de los nodos para poder ordenarlos y acceder
	 * a ellos ordenadamente y rápidamente.
	 */
	class Comparator {
	public:

		bool operator() (const AStarNode* n1, const AStarNode* n2) const {
			bool comparation = false;
			// Internamente, set<...> ordenará el conjunto según F, luego "restantes" y finalmente el propio tablero en sí.
			if (n1->getF() > n2->getF()) {
				comparation = true;
			} else if (n1->getF() < n2->getF()) {
				comparation = false;
			} else if (n1->getBoard()->getRemaining() > n2->getBoard()->getRemaining()) {
				comparation = true;
			} else if (n1->getBoard()->getRemaining() < n2->getBoard()->getRemaining()) {
				comparation = false;
			} else {
				int *board1 = n1->getBoard()->getBoard();
				int *board2 = n2->getBoard()->getBoard();
				int total = n1->getBoard()->getTotal();
				int memcomparation = memcmp(board1, board2, total * sizeof (int));
				if (memcomparation < 0) {
					comparation = true;
				} else if (memcomparation > 0) {
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
	~HeapAEstrella() {
		for (set<AStarNode*, Comparator>::iterator i = list.begin(); i != list.end(); i++)
			delete *i;
	}

	/**
	 * @brief Introduce un nodo en la lista.
	 *
	 * @param nodo Nodo que se va a introducir en la lista.
	 */
	bool push(AStarNode *nodo) {
		pair < set<AStarNode*, Comparator>::iterator, bool> status = list.insert(nodo);
		if (!status.second) {
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
	AStarNode* getNodo(AStarNode *nodo) const {
		/*set<AStarNode*, Comparator>::iterator i = lista.find(nodo);
		if (i == lista.end())
										return NULL;
		else
										return *i;*/

		// Se recorre la lista de nodos manualmente en busca de un nodo que coincida.
		for (set<AStarNode*, Comparator>::iterator i = list.begin(); i != list.end(); i++) {
			AStarNode *oldNode = *i;

			// En este caso, no se comprueba con F porque es variante para 2 nodos iguales.
			if (oldNode->getH() == nodo->getH()) {
				if (oldNode->getBoard()->getRemaining() == nodo->getBoard()->getRemaining()) {
					int *board1 = oldNode->getBoard()->getBoard();
					int *board2 = nodo->getBoard()->getBoard();
					int total = oldNode->getBoard()->getTotal();
					if (memcmp(board1, board2, total * sizeof (int)) == 0)
						return oldNode;
				}
			}
		}
		return NULL;
	}

	/**
	 * @brief Elimina el elemento y devuelve la referencia.
	 *
	 * @return Devuelve una referencia al elemento del conjunto.
	 */
	AStarNode* pop() {
		set<AStarNode*, Comparator>::iterator i = list.begin();
		if (i == list.end()) {
			return NULL;
		} else {
			AStarNode* top = *i;
			list.erase(i);
			return top;
		}
	}

	/**
	 * @brief Obtiene una referencia al primer elemento de la lista pero sin eliminar el elemento.
	 *
	 * @return Devuelve una referencia al primer elemento de la lista.
	 */
	const AStarNode* top() const {
		set<AStarNode*, Comparator>::iterator i = list.begin();
		if (i == list.end())
			return NULL;
		else
			return *i;
	}

	/**
	 * @brief Esa referencia será eliminada de la lista.
	 *
	 * @param nodo Nodo que se va a comprobar si ya está en la lista para eliminarlo.
	 */
	void remove(AStarNode *node) {
		list.erase(node);
	}

	/**
	 * @return Devuelve verdadero si la lista está vacía.
	 */
	bool empty() const {
		return list.empty();
	}

	/**
	 * @return Devuelve la cantidad de elementos de la lista.
	 */
	int size() const {
		return list.size();
	}
};

#endif // HEAPAESTRELLA_H
