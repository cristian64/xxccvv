/*
 * File:   Board.cpp
 * Author: juen
 *
 * Created on 24 de octubre de 2010, 16:12
 */

#include "Board.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <queue>
#include <set>
#include <list>
#include <utility>
#include <cmath>
#include <sstream>
#include <stdlib.h>

using namespace std;

//por probar, pero no aporta nada, no creo que merezca la pena hacerlo pr0
//eso implicaria asi sin pensar mucho uno rollo tipo singleton con contador
//de usos para liberarlo con el ultimo tablero instanciado.
int Board::useMask[400];

//Ver Board::entropicBound()
double Board::log2colors;
double Board::ln2 = log(2); //ln(2) precalculado

Board::Board(int colors, int rows, int columns) {
	this->rows = rows;
	this->columns = columns;
	this->colors = colors;
	total = remaining = rows * columns;
	this->board = new int[rows * columns];
		srand(time(NULL));
	for (int i = 0; i < rows * columns; i++) {
		board[i] = rand() % colors + 1;
	}
	Board::log2colors = log2(this->colors);
}

Board::Board(const std::string path) {

	std::ifstream is;
	is.open(path.c_str(), std::fstream::in);
	if (!is.is_open()) {
		std::cerr << "NO HAY ARCHIVO\n";
	}
	is >> this->rows;
	is >> this->columns;
	is >> this->colors;
	total = remaining = rows * columns;
	this->board = new int [total];
	for (int i = 0; i < total; i++) {
		is >> this->board[i];
	}
	Board::log2colors = log2(this->colors);
}

Board::Board(const Board& o) {
	this->rows = o.rows;
	this->columns = o.columns;
	this->colors = o.colors;
	total = o.total;
	remaining = o.remaining;
	this->board = new int[total];
	memcpy(this->board, o.board, total * sizeof (int));
}

Board& Board::operator=(const Board& orig) {
	if (this != &orig) {
		if (this->board != NULL) {
			delete [] this->board;
		}
		this->rows = orig.rows;
		this->columns = orig.columns;
		this->colors = orig.colors;
		total = orig.total;
		remaining = orig.remaining;
		this->board = new int[total];
		memcpy(this->board, orig.board, total * sizeof (int));
	}
	return *this;
}

bool Board::operator==(const Board& o) const {
	if (remaining != o.remaining) {
		return false;
	}

	if (this->rows != o.rows || this->columns != o.columns || this->colors != o.colors) {
		return false;
	}

	if (memcmp(this->board, o.board, total * sizeof (int)) != 0) {
		return false;
	}
	return true;
}

Board::~Board() {
	delete [] this->board;
}

int* Board::getBoard() const {
	return board;
}

int Board::getColors() const {
	return colors;
}

void Board::setColors(int colors) {
	this->colors = colors;
}

int Board::getColumns() const {
	return columns;
}

void Board::setColumns(int columns) {
	this->columns = columns;
}

int Board::getRows() const {
	return rows;
}

void Board::setRows(int rows) {
	this->rows = rows;
}

int Board::getRemaining() const {
	return remaining;
}

int Board::getTotal() const {
	return total;
}

int Board::getPosition(int column, int row) const {
	return this->board[row * this->columns + column];
}

int Board::setPosition(int column, int row, int color) {
	return this->board[row * this->columns + column] = color;
}

void Board::gravity() {
	//vertical
	//desde arriba, busca la primera baldosa tal que la de debajo este vacia
	for (int j = 0; j<this->columns; j++) {
		for (int i = this->rows - 1; i >= 0; i--) {
			int k = i - 1;
			if (k >= 0) {
				if (this->getPosition(j, k) == 0) {
					while ((k < this->rows - 1)) {//desplaza las baldosas una posicion
						this->setPosition(j, k, this->getPosition(j, k + 1));
						k++;
					}
					//por arriba entran ceros
					this->setPosition(j, this->rows - 1, 0);
				}
			}
		}
	}

	//horizontal
	for (int j = this->columns - 1; j >= 0; j--) {
		if (this->getPosition(j, 0) != 0) { // si la actual tiene baldosas
			if (j - 1 >= 0) {//si la anterior no se sale del tablero
				if (this->getPosition(j - 1, 0) == 0) { // si la anterior esta vacia
					for (int col = j - 1; col < this->columns - 1; col++) {
						for (int fil = 0; fil < this->rows; fil++) {
							this->setPosition(col, fil, this->getPosition(col + 1, fil));
						}
					}
					for (int i = 0; i<this->rows; i++) {
						this->setPosition(this->columns - 1, i, 0);
					}
				}
			}
		}
	}
}

list<set<std::pair<int, int> > > Board::getGroupMoves() const {
	return getGroups();
}

pair<int, int> Board::getMove(std::set<std::pair<int, int> > groupMove) const {
	return *groupMove.begin();
}

std::set<std::pair<int, int> > Board::getGroupMove(int x, int y) const {
	return getGroup(y, x);
}

/*
 *calcula la puntuacion de un grupo
 */
int Board::score(std::set<std::pair<int, int> > tiles) {
	return tiles.size()*(tiles.size() - 1);
}

/*
 * elimina un grupo del tablero
 * Se podria discutir si crear otro que devuelva un nuevo tablero y no modifique
 * self, mas por comodidad que otra cosa
 * Devuelve la puntuacion del grupo eliminado
 */
int Board::removeGroup(std::set<std::pair<int, int> > tiles) {
	std::set<std::pair<int, int> >::iterator it;
	for (it = tiles.begin(); it != tiles.end(); it++) {
		this->setPosition(it->first, it->second, 0);
	}
	this->gravity();
	remaining -= tiles.size();
	return tiles.size()*(tiles.size() - 1);
}

// Realiza una estimación optimista del valor del tablero en el mejor caso.

int Board::optimisticBound() const {
	int maxScore = 0;
	int count[this->colors];
	memset(count, 0, sizeof (int) * this->colors);

	// Recorremos el tablero contando únicamente la cantidad de fichas de cada color.
	for (int i = 0; i < total; i++) {
		if (this->board[i] > 0) {
			count[this->board[i] - 1]++;
		}
	}

	for (int i = 0; i< this->colors; i++) {
		maxScore += count[i]*(count[i] - 1);
	}
	return maxScore;
}

// esta centrado en 0, ahora mismo no vale para una mierda

double taylorLog2(double x, int grado) {
	x--;
	double r = 0;
	for (int i = 1; i <= grado; i++) {
		r += pow(-1, i + 1) * pow(x, i) / (i * Board::ln2);
	}
	return r;
}

//Estima el valor maximo del tablero aplicando la entropia
//TODO PRIMORDIAL Hay que buscar la serie de taylor del log2, el log es una operacion costosisima.
//En 100 segundos de ejecucion el log2 ocupa 14 segundos
//Precalculando el log2(nColores) baja a 10 segundos
//el 7 baja de ~47 a ~<45 segundos

float Board::entropyBound() const {

	int count[this->colors + 1];
	memset(count, 0, sizeof (int) * (this->colors + 1));

	// Recorremos el tablero contando únicamente la cantidad de fichas de cada color.

	for (int i = 0; i < total; i++) {
		count[this->board[i]]++;
	}

	int maxScore = 0;
	for (int i = 1; i< this->colors + 1; i++) {
		maxScore += count[i]*(count[i] - 1);
	}

	double entropy = 0;
	double prob = 0;
	//desde 1 para no contar la aportacion de las zonas vacias
	for (int i = 1; i< this->colors + 1; i++) {
		if (count[i] > 0) {
			prob = count[i] / (float) total;
			entropy += log2(prob) * prob;
			//entropia +=taylorLog2(prob, 20)*prob;
		}
	}

	entropy = -entropy;

	//double entropiaNormalizada = entropia / log(this->colors + 1);
	double normalicedEntropy = entropy / Board::log2colors;

	return (maxScore * normalicedEntropy);
}

void Board::showMoves(std::list<std::set<std::pair<int, int> > > lista) {
	list<set<std::pair<int, int> > >::iterator it1;
	for (it1 = lista.begin(); it1 != lista.end(); it1++) {
		if (it1->size() > 0) {
			cout << "MOV: ";
			Board::showMove(*it1);
			cout << "Puntuacion: " << it1->size() * (it1->size() - 1) << endl;
		}
	}
}

void Board::showMove(set<std::pair<int, int> > move) {
	set<pair<int, int> >::iterator it2;
	for (it2 = move.begin(); it2 != move.end(); it2++) {
		cout << "(" << it2->first << ", " << it2->second << "), ";
	}
}

void Board::show() const {
	for (int i = 0; i< this->rows; i++) {
		for (int j = 0; j< this->columns; j++) {
			std::cout << this->board[i * this->columns + j] << ' ';
		}
		std::cout << std::endl;
	}
}

string Board::toString() const {
	stringstream flujo;
	flujo << rows;
	flujo << endl;
	flujo << columns;
	flujo << endl;
	flujo << colors;
	flujo << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			flujo << this->getPosition(j, i);
			flujo << " ";
		}
		flujo << endl;
	}

	return flujo.str();
}

/**
 * @brief Calcula el grupo de la casilla indicada y lo devuelve.
 *
 * @param fila Fila de la celda.
 * @param columna Columna de la celda.
 * @return Devuelve el grupo de la casilla indicada por la fila y la columna.
 */
set<pair<int, int> > Board::getGroup(int row, int column) const {
	set<pair<int, int> > group;

	if (board[row * columns + column] != 0) {
		memset(useMask, 0, sizeof (int) * total);
		floodFill(row * columns + column, group);
		if (group.size() < 2) {
			group.clear();
		}
	}

	return group;
}

void Board::floodFill(int tile, set<pair<int, int> > &group) const {
	int row = tile / columns;
	int column = tile % columns;

	// Introducimos la casilla actual al grupo y la marcamos.
	group.insert(pair<int, int>(column, row));
	useMask[tile] = 1;

	// Comprobamos hacia dónde hay que inundar.
	if (column + 1 < columns) {
		int left = row * columns + (column + 1);
		if (useMask[left] == 0 && board[tile] == board[left]) {
			floodFill(left, group);
		}
	}

	if (column - 1 >= 0) {
		int right = row * columns + (column - 1);
		if (useMask[right] == 0 && board[tile] == board[right]) {
			floodFill(right, group);
		}
	}

	if (row - 1 >= 0) {
		int up = (row - 1) * columns + column;
		if (useMask[up] == 0 && board[tile] == board[up]) {
			floodFill(up, group);
		}
	}

	if (row + 1 < rows) {
		int down = (row + 1) * columns + column;
		if (useMask[down] == 0 && board[tile] == board[down]) {
			floodFill(down, group);
		}
	}
}

list<set<pair<int, int> > > Board::getGroups() const {
	list<set<pair<int, int> > > moves;
	memset(useMask, 0, sizeof (int) * total);

	// Se recorre el tablero. Cada casilla se procesa una vez (nos ayudamos de la máscara para ello).
	for (int i = 0; i < total; i++) {
		// Si no se ha procesado, se procesa aplicando el algoritmo recursivo.
		if (useMask[i] == 0 && board[i] != 0) {
			set<pair<int, int> > group;
			floodFill(i, group);
			if (group.size() >= 2) {
				moves.push_back(group);
			}
		}
	}

	// Se libera la memoria y se devuelve el resultado.
	//delete [] mascara;
	return moves;
}
