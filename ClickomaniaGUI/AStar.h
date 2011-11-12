#ifndef AESTRELLA_H
#define AESTRELLA_H

#include "Board.h"
#include "AStarNode.h"
#include "AStarHeap.h"
#include "AStarSet.h"

#include <queue>
#include <set>
#include <list>
#include <iostream>
#include <ctime>

using namespace std;

class AStar {
public:

	/**
	 * @brief Ejecuta el algoritmo A* para encontrar la mejor combinación de movements para obtener la solución óptima.
	 *
	 * @param board Tablero que se quiere resolver.
	 * @return Devuelve la lista de movements que hay que aplicar sobre el tablero y la puntuación obtenida.
	 */
	static pair<int, list<pair<int, int> > > run(const Board &board) {

		// Definición de ambas listas (al salirse de ámbito se liberará la memoria de cada nodo introducido en ellas).
		SetAEstrella closedList;
		HeapAEstrella openList;

		// La lista abierta comienza con un nodo inicial y la lista cerrada comienza vacía.
		openList.push(new AStarNode(board));

		// Referencia que apunta al mejor nodo encontrado (inicialmente el nodo final se corresponde con el nodo vacío).
		const AStarNode *goalNode = openList.top();

		// El algoritmo se ejecuta mientras queden nodos en la lista abierta o el mejor de esos nodos no sea potencialmente mejor que el nodo final ya encontrado.
		while (!openList.empty() && goalNode->getG() < openList.top()->getF()) {
			// Se extrae el nodo que hay encima de la cola de prioridad y pasa a la lista cerrada.
			AStarNode* currentNode = openList.pop();
			if (!closedList.push(currentNode))
				continue;

			// Se genera toda su descendencia.
			list<AStarNode*> childs = currentNode->generateChilds();

			// Si no tiene descendencia, significa que es un nodo final (nodo hoja).
			if (childs.empty()) {
				//  Si es mejor que el anterior, se almacena el nuevo.
				if (goalNode->getG() < currentNode->getG()) {
					goalNode = currentNode;
					//cout << "Encuentro un nodo final mejor: " << currentNode->getG() << endl;
				}
			} else {
				// Si tiene descendencia, se procesan todos los nodos hijos.
				for (list<AStarNode*>::const_iterator i = childs.begin(); i != childs.end(); i++) {
					AStarNode *child = *i;

					// Si el nodo generado no es potencialmente mejor que el encontrado hasta el momento,
					//se destruye y se pasa al siguiente.
					if (child->getF() <= goalNode->getG()) {
						delete child;
						continue;
					}

					// Se comprueba si está en la lista cerrada.
					if (closedList.getNodo(child) != NULL) {
						// Si ya está en la lista de nodos cerrados, liberamos el nodo porque ya ha sido procesado.
						delete child;
					} else {
#ifndef NO_COMPROBAR
						// Pasamos de las comprobaciones de la lista abierta y lo introducimos directamente.
						openList.push(child);
#else
						// Se comprueba si está en la lista abierta.
						AStarNode *openListNode = openList.getNodo(child);
						if (openListNode != NULL) {
							// Si la procedencia del nodo es mejor que la del nodo que está en la lista, se actualiza la procedencia y se vuelve a introducir en la lista.
							if (openListNode->getF() < child->getF()) {
								openList.remove(openListNode);
								delete openListNode;
								openList.push(child);
							} else {
								// Si su procedencia no es mejor que la del que ya estaba, se libera la memoria.
								delete child;
							}
						} else {
							// Si no estaba en la lista abierta, se introduce.
							openList.push(child);
						}
#endif
					}
				}
			}
		}

		//cout << "Tamaño de la lista abierta: " << openList.size() << endl;
		//cout << "Tamaño de la lista cerrada: " << closedList.size() << endl;

		// Se genera el solution final.
		pair<int, list<pair<int, int> > > solution;
		solution.first = goalNode->getG();

		// Se construye la lista de movements a partir del nodo final encontrado.
		list<pair<int, int> > movements;
		while (goalNode != NULL) {
			movements.push_front(goalNode->getMove());
			goalNode = goalNode->getParentNode();
		}
		movements.erase(movements.begin());
		solution.second = movements;

		//el solution debe mostrarse aqui, a veces tarda segundos en liberar la memoria.
		return solution;
	}
};

#endif // AESTRELLA_H
