#ifndef NODOAESTRELLA_H
#define NODOAESTRELLA_H

#include <utility>
#include <list>
#include "Board.h"
#include <sstream>

using namespace std;

class NodoAEstrella
{
	private:

		/**
		 * Tablero que representa el nodo.
		*/
		Board *board;

		/**
		 * Referenca al tablero inmediatamente anterior. Si la referencia es nula, significa que éste es el nodo raíz.
		 * Es constante y el compilador no permitirá su modificación.
		*/
		const NodoAEstrella *padre;

		/**
		 * Movimiento que se ha efectuado para alcanzar este tablero. Si no existe el padre, no se considerará este movimiento.
		*/
		pair<int, int> movimiento;

		/**
		 * Puntuación con la que se ha alcanzado este tablero. Si no existe el padre, esta puntuación es 0.
		*/
		float g;

		/**
		 * Representa el valor heurístico del tablero. Sólo se calcula una vez al asignar el tablero al nodo.
		*/
		float h;

		/**
		 * Valor total del tablero considerando la puntuación real obtenida hasta el momento (G) y la heurística (H).
		*/
		float f;

		/**
		 * Hash para el nodo.
		*/
		unsigned long int hash;

		/**
		 * @brief Recalcula la puntuación a partir del movimiento con el que se llegó al tablero
		 * y el nodo padre. Es decir, es la suma de la puntuación del tablero anterior y la cantidad
		 * de puntos que se obtiene al aplicar el movimiento sobre ese tablero anterior.
		 * Sólo se modifica cuando se modifica el padre.
		 * Implica recalcular el valor de F.
		*/
		void recalcularG()
		{
			if (padre != NULL)
				g = padre->g + Board::score(padre->board->getGroupMove(movimiento.first, movimiento.second));
			else
				g = 0;
			recalcularF();
		}

		/**
		 * @brief Extrae el valor de heurística del tablero asociado al nodo.
		 * Sólo se ejecuta una vez por cada nodo, después de asignar el tablero.
		 * Implica recalcular el valor de F.
		*/
		void recalcularH()
		{
			//h = board->optimisticBound();
			h = board->entropyBound();
			recalcularF();
		}

		/**
		 * @brief Recalcula la suma de F = G + H. Sólo se ejecuta cuando se modifica G o H.
		*/
		void recalcularF()
		{
			//float w = 0.55;
			//f = w * g + (1 - w) * h;
			//evita que ramifique tanto, relajacion.
			f = g + 0.8 * h;
		}

		/**
		 * @brief Recalcula el hash para el nodo.
		 *
		*/
		void recalcularHash()
		{
			stringstream flujo;
			int *tablero = board->getBoard();
			int total = min(board->getTotal(), 19);
			for (int i = 0; i < total; i++)
			{
				flujo << tablero[i];
			}
			hash = strtoul(flujo.str().c_str(), NULL, 0);
		}


	public:

		/**
		 * @brief Constructor sobrecargado que recibe el nodo padre y el movimiento que hay que aplicar sobre él
		 * para generar este nodo. Se reservará memoria para el tablero y se aplicará dicho movimiento.
		 *
		 * @param padre Referencia al nodo padre.
		 * @param movimiento Movimiento que hay que aplicar al padre para obtene el tablero.
		*/
		NodoAEstrella(const NodoAEstrella *padre, pair<int, int> movimiento)
		{
			this->padre = padre;
			this->movimiento = movimiento;
			board = new Board((*padre->board));
			board->removeGroup(board->getGroupMove(movimiento.first, movimiento.second));
			g = 0;
			h = 0;
			f = 0;
			recalcularG();
			recalcularH();
			//recalcularHash();
		}

		/**
		 * @brief Constructor sobrecargado que recibe una referencia al tablero del nodo raíz.
		 * Éste constructor sólo se invoca al crear el nodo inicial.
		 *
		 * @param board Referencia al tablero para el nodo raíz.
		*/
		NodoAEstrella(const Board &board)
		{
			padre = NULL;
			this->board = new Board(board);
			movimiento.first = -1;
			movimiento.second = -1;
			g = 0;
			h = 0;
			f = 0;
			recalcularG();
			recalcularH();
			//recalcularHash();
		}

		/**
		 * @brief Constructor de copia. Dado un nodo origen, crea otro idéntico.
		 * Hace uso del operador de asignación.
		 *
		 * @param origen Nodo origen que va a ser copiado.
		*/
		NodoAEstrella(const NodoAEstrella &origen)
		{
			board = NULL;
			*this = origen;
		}

		/**
		 * @brief Destructor por defecto.
		 *
		*/
		~NodoAEstrella()
		{
			if (board != NULL)
				delete board;
		}

		/**
		 * @brief Dado un nodo, asigna todos los atributos de este nodo al nodo que invocó el método.
		 *
		 * @param origen Nodo origen que va a ser copiado.
		 * @return Devuelve una copia por referencia del nodo para permitir asignaciones en cadena.
		*/
		NodoAEstrella& operator=(const NodoAEstrella &origen)
		{
			if (this != &origen)
			{
				if (board != NULL)
					delete board;
				board = new Board(*(origen.board));
				padre = origen.padre;
				movimiento = origen.movimiento;
				g = origen.g;
				h = origen.h;
				f = origen.f;
				hash = origen.hash;
			}
			return *this;
		}

		int getF() const
		{
			return f;
		}

		int getG() const
		{
			return g;
		}

		int getH() const
		{
			return h;
		}

		pair<int, int> getMovimiento() const
		{
			return movimiento;
		}

		Board* getBoard() const
		{
			return board;
		}

		unsigned long int getHash() const
		{
			return hash;
		}

		/**
		 * @brief Establece un nuevo padre al nodo. Esto ocurre cuando se encuentra un camino mejor que permite
		 * alcanzar este nodo con un resultado preferible (con un G preferible).
		 *
		 * @param padre Nuevo nodo padre.
		 * @param movimiento Movimiento que hay que aplicar en el tablero del nodo padre para llegar a este nodo.
		*/
		void setNodoPadre(const NodoAEstrella* padre, pair<int, int> movimiento)
		{
			this->padre = padre;
			this->movimiento = movimiento;
			recalcularG();
		}

		/**
		 * @return Devuelve la referencia al nodo padre. Es constante y el compilador no permitirá su modificación.
		*/
		const NodoAEstrella* getNodoPadre() const
		{
			return padre;
		}

		/**
		 * @brief Crea una lista con la descendencia del nodo según los movimientos posibles que se pueden aplicar.
		 * La lista de nodos debe liberarse después de su uso; el invocador del método debe encargarse de su liberación.
		 *
		 * @return Devuelve una lista con las referencias a los nodos hijos.
		*/
		list<NodoAEstrella*> generarDescendencia() const
		{
			list<NodoAEstrella*> descendencia;
			list<set<pair<int, int> > > movimientos = board->getGroupMoves();
			for (list<set<pair<int, int> > >::iterator i = movimientos.begin(); i != movimientos.end(); i++)
			{
				NodoAEstrella *hijo = new NodoAEstrella(this, *(i->begin()));
				descendencia.push_back(hijo);
			}
			return descendencia;
		}
};

#endif // NODOAESTRELLA_H
