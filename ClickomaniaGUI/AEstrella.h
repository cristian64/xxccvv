#ifndef AESTRELLA_H
#define AESTRELLA_H

#include "Board.h"
#include "NodoAEstrella.h"
#include "HeapAEstrella.h"
#include "SetAEstrella.h"

#include <queue>
#include <set>
#include <list>
#include <iostream>

using namespace std;

class AEstrella
{
	public:

		/**
		 * @brief Ejecuta el algoritmo A* para encontrar la mejor combinación de movimientos para obtener la solución óptima.
		 *
		 * @param board Tablero que se quiere resolver.
		 * @return Devuelve la lista de movimientos que hay que aplicar sobre el tablero y la puntuación obtenida.
		*/
		pair<int, list<pair<int, int> > > run(const Board &board) const
		{
			// Definición de ambas listas (al salirse de ámbito se liberará la memoria de cada nodo introducido en ellas).
			SetAEstrella listaCerrada;
			HeapAEstrella listaAbierta;

			// La lista abieta comienza con un nodo inicial y la lista cerrada comienza vacía.
			listaAbierta.push(new NodoAEstrella(board));

			// Referencia que apunta al mejor nodo encontrado (inicialmente el nodo final se corresponde con el nodo vacío).
			const NodoAEstrella *nodoFinal = listaAbierta.top();

			// El algoritmo se ejecuta mientras queden nodos en la lista abierta o el mejor de esos nodos no sea potencialmente mejor que el nodo final ya encontrado.
			while (!listaAbierta.empty() && nodoFinal->getG() < listaAbierta.top()->getF())
			{
				// Se extrae el nodo que hay encima de la cola de prioridad y pasa a la lista cerrada.
				NodoAEstrella* nodoActual = listaAbierta.pop();
				listaCerrada.push(nodoActual);

				// Se genera toda su descendencia.
				list<NodoAEstrella*> descendencia = nodoActual->generarDescendencia();

				// Si no tiene descendencia, significa que es un nodo final (nodo hoja).
				if (descendencia.empty())
				{
					//  Si es mejor que el anterior, se almacena el nuevo.
					if (nodoFinal->getG() < nodoActual->getG())
					{
						nodoFinal = nodoActual;
						//cout << "Encuentro un nodo final mejor: " << nodoActual->getG() << endl;
					}
				}
				else
				{
					// Si sí tiene descendencia, se procesan todos los nodos hijos.
					for (list<NodoAEstrella*>::const_iterator i = descendencia.begin(); i != descendencia.end(); i++)
					{
						NodoAEstrella *descendiente = *i;

						// Se comprueba si está en la lista cerrada.
						if (listaCerrada.getNodo(descendiente) != NULL)
						{
							// Si ya está en la lista de nodos cerrados, liberamos el nodo porque ya ha sido procesado.
							delete descendiente;
						}
						else
						{
							// Se comprueba si está en la lista abierta.
							NodoAEstrella *anterior = listaAbierta.getNodo(descendiente);
							if (anterior != NULL)
							{
								// Si la procedencia del nodo es mejor que la del nodo que está en la lista, se actualiza la procedencia y se vuelve a introducir en la lista.
								if (anterior->getF() < descendiente->getF())
								{
									listaAbierta.remove(anterior);
									delete anterior;
									listaAbierta.push(descendiente);
								}
								else
								{
									// Si su procedencia no es mejor que la del que ya estaba, se libera la memoria.
									delete descendiente;
								}
							}
							else
							{
								// Si no estaba en la lista abierta, se introduce.
								listaAbierta.push(descendiente);
							}
						}
					}
				}
			}

			cout << "Tamaño de la lista abierta: " << listaAbierta.size() << endl;
			cout << "Tamaño de la lista cerrada: " << listaCerrada.size() << endl;

			// Se genera el resultado final.
			pair<int, list<pair<int, int> > > resultado;
			resultado.first = nodoFinal->getG();

			// Se construye la lista de movimientos a partir del nodo final encontrado.
			list<pair<int, int> > movimientos;
			while (nodoFinal != NULL)
			{
				movimientos.push_front(nodoFinal->getMovimiento());
				nodoFinal = nodoFinal->getNodoPadre();
			}
			movimientos.erase(movimientos.begin());
			resultado.second = movimientos;

			return resultado;
		}
};

#endif // AESTRELLA_H
