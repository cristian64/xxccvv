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
	int getRemaining() const;
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
	list<set<pair<int, int> > > getGroups() const;


	/**
	 * @brief Calcula el grupo de la casilla indicada y lo devuelve.
	 *
	 * @param fila Fila de la celda.
	 * @param columna Columna de la celda.
	 * @return Devuelve el grupo de la casilla indicada por la fila y la columna.
	 */
	set<pair<int, int> > getGroup(int row, int column) const;

	/**
	 * @brief Recorre el tablero recursivamente por inundación buscando las casillas del mismo color y las
	 * introduce en un conjunto recibido por referencia en los parámetros.
	 *
	 * @param celda Celda que se va a comprobar.
	 * @param mascara Vector que indica si la celda ha sido ocupada o no.
	 * @param grupo Conjunto de celdas donde se coleccionarán las celdas. Tiene un valor por referencia.
	 */

	void floodFill(int tile, set<pair<int, int> > &group) const;

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

	//log(2) precalculado
	static double ln2;

	static void setUseMaskSize(int size);
	static void deleteUseMask();

private:
	static int useMask[400];
	static double log2colors;
	int *board;
	int rows;
	int columns;
	int colors;
	int total;
	int remaining;

};

#endif	/* BOARD_H */

