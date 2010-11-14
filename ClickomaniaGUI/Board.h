/*
 * File:   Board.h
 * Author: juen
 *
 * Created on 24 de octubre de 2010, 16:12
 */

#ifndef BOARD_H
#define	BOARD_H
#include <string>
#include <list>
#include <utility>
#include <set>

#include <string.h>

using namespace std;

class Board {
public:

	int* getBoard() const;
	int getColors() const;
	void setColors(int colors);
	int getColumns() const;
	void setColumns(int columns);
	int getRows() const;
	void setRows(int rows);
	int getRestantes() const;
	int getTotal() const;

	Board(int colores, int filas, int columnas);
	Board(const std::string path);
	Board(const Board& orig);
	virtual ~Board();
	Board & operator=(const Board& orig);
	bool operator==(const Board& o) const;

	/*
	 * hay que hacer todo a traves de los get y set porque el tablero es un
	 * vector, no una matriz.
	 */
	int getPosition(int column, int row) const;
	int setPosition(int column, int row, int color);

	/*
	 * aplica gravedad vertical y lateral
	 */
	void gravity();

	/*
	 * elimina (pone a 0) las baldosas que pertenecen a un grupo
	 * Tambien reajusta el tablero segun la gravedad
	 */
	int removeGroup(std::set<std::pair<int, int> > tiles);

	//calula el movimiento(grupo) que hay en las coordenadas
	std::set<std::pair<int, int> > getGroupMove(int x, int y) const;

	//calcula todos los movimientos(grupos) posibles dado un tablero
	std::list<std::set<std::pair<int, int> > > getGroupMoves() const;

	/**
	 * @brief
	 *
	 * @return Devuelve la lista de grupos que hay en el tablero.
	*/
	list<set<pair<int, int> > > getGrupos() const
	{
		list<set<pair<int, int> > > movimientos;
		//int *mascara = new int[total];
		memset(mascara, 0, sizeof(int) * total);

		// Se recorre el tablero. Cada casilla se procesa una vez (nos ayudamos de la máscara para ello).
		for (int i = 0; i < total; i++)
		{
			// Si no se ha procesado, se procesa aplicando el algoritmo recursivo.
			if (mascara[i] == 0 && board[i] != 0)
			{
				set<pair<int, int> > grupo;
				algoritmo(i, mascara, grupo);
				if (grupo.size() >= 2)
					movimientos.push_back(grupo);
			}
		}

		// Se libera la memoria y se devuelve el resultado.
		//delete [] mascara;
		return movimientos;
	}

	/**
	 * @brief Calcula el grupo de la casilla indicada y lo devuelve.
	 *
	 * @param fila Fila de la celda.
	 * @param columna Columna de la celda.
	 * @return Devuelve el grupo de la casilla indicada por la fila y la columna.
	*/
	set<pair<int, int> > getGrupo(int fila, int columna) const
	{
		set<pair<int, int> > grupo;

		if (board[fila * columns + columna] != 0)
		{
			//int *mascara = new int[total];
			memset(mascara, 0, sizeof(int) * total);

			algoritmo(fila * columns + columna, mascara, grupo);
			if (grupo.size() < 2)
				grupo.clear();
			//delete [] mascara;
		}

		return grupo;
	}

	/**
	 * @brief Recorre el tablero recursivamente por inundación buscando las casillas del mismo color y las
	 * introduce en un conjunto recibido por referencia en los parámetros.
	 *
	 * @param celda Celda que se va a comprobar.
	 * @param mascara Vector que indica si la celda ha sido ocupada o no.
	 * @param grupo Conjunto de celdas donde se coleccionarán las celdas. Tiene un valor por referencia.
	*/
	//void algoritmo(int celda, int *mascara, set<pair<int, int> > &grupo) const
	void algoritmo(int celda, int *mascara, set<pair<int, int> > &grupo) const
	{
		int fila = celda / columns;
		int columna = celda % columns;

		// Introducimos la casilla actual al grupo y la marcamos.
		grupo.insert(pair<int, int>(columna, fila));
		mascara[celda] = 1;

		// Comprobamos hacia dónde hay que inundar.
		if (columna + 1 < columns)
		{
			int izquierda = fila * columns + (columna + 1);
			if (mascara[izquierda] == 0 && board[celda] == board[izquierda])
				algoritmo(izquierda, mascara, grupo);
		}

		if (columna - 1 >= 0)
		{
			int derecha = fila * columns + (columna - 1);
			if (mascara[derecha] == 0 && board[celda] == board[derecha])
				algoritmo(derecha, mascara, grupo);
		}

		if (fila - 1 >= 0)
		{
			int arriba = (fila - 1) * columns + columna;
			if (mascara[arriba] == 0 && board[celda] == board[arriba])
				algoritmo(arriba, mascara, grupo);
		}

		if (fila + 1 < rows)
		{
			int abajo = (fila + 1) * columns + columna;
			if (mascara[abajo] == 0 && board[celda] == board[abajo])
				algoritmo(abajo, mascara, grupo);
		}
	}

	/*
	 * Devuelve la coordenada que define el movimiento (podria ser una cualquiera)
	 * pero el guion pide un formato determinado que, por construccion, corresponde
	 * con la primera baldosa del grupo.
	 */
	std::pair<int, int> getMove(const std::set<std::pair<int, int> > groupMove) const;

	//calcula la puntuacion asociada a un grupo
	//deberia ser static
	static int score(std::set<std::pair<int, int> > tiles);

	/// Realiza una estimación optimista del valor del tablero en el mejor caso.
	/**
	 * Se presupone que los grupos de baldosas del mismo color son adyacentes.
	 * \return Devuelve un valor entero con la estimación optimista del tablero.
	 */
	float entropyBound() const;
	int optimisticBound() const;

	//broza para mostrar por pantalla que deberia ser static
	static void showMoves(std::list<std::set<std::pair<int, int> > > lista);
	static void showMove(std::set<std::pair<int, int> > move);
	void show() const;

	string toString() const;

	std::list<std::pair<Board*, int> > childList() const;

private:
	static int mascara[300];
	static double log2colors;
	int *board;
	int rows;
	int columns;
	int colors;
	int total;
	int restantes;

};

#endif	/* BOARD_H */

