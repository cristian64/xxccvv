/* 
 * File:   Board.cpp
 * Author: juen
 * 
 * Created on 24 de octubre de 2010, 16:12
 */

#include "Board.h"
#include <fstream>
#include <string.h>
#include <cassert>
#include <iostream>
#include <queue>
#include <set>
#include <list>
#include <map>
#include <utility>

using namespace std;

Board::Board() {
    this->rows = new int;
    this->columns = new int;
    this->colors = new int;
    this->board = NULL;
}

Board::Board(const std::string path) {
    std::ifstream is;
    is.open(path.c_str(), std::fstream::in);
    if (!is.is_open()) {
        std::cerr << "NO HAY ARCHIVO\n";
    }
    this->rows = new int;
    this->columns = new int;
    this->colors = new int;
    is >> *this->rows;
    is >> *this->columns;
    is >> *this->colors;
    int elements = *this->rows * * this->columns;
    this->board = new int [elements];
    for (int i = 0; i < elements; i++) {
        is >> this->board[i];
    }
}

Board::Board(const Board& orig) {
    this->rows = new int;
    this->columns = new int;
    this->colors = new int;
    *this->rows = *orig.rows;
    *this->columns = *orig.columns;
    *this->colors = *orig.colors;
    int elements = *this->rows * * this->columns;
    this->board = new int[elements];
    memcpy(this->board, orig.board, elements * sizeof (int));
}

Board& Board::operator=(const Board& orig) {
    if (this != &orig) {
        delete [] this->board;
        *this->rows = *orig.rows;
        *this->columns = *orig.columns;
        *this->colors = *orig.colors;
        int elements = *this->rows * * this->columns;
        this->board = new int[elements];
        memcpy(this->board, orig.board, elements * sizeof (int));
    }
    return *this;
}

bool Board::operator==(const Board& o) {
    if (*this->rows != *o.rows && *this->columns != *o.columns && *this->colors != *o.colors) {
        return false;
    }

    if (memcmp(this->board, o.board, *this->rows * * this->columns * sizeof (int) != 0)) {
        return false;
    }
    return true;
}

Board::~Board() {
    delete [] this->board;
    delete this->colors;
    delete this->columns;
    delete this->rows;
}

int* Board::getBoard() const {

    return board;
}

int Board::getColors() const {

    return *colors;
}

void Board::setColors(int colors) {

    *this->colors = colors;
}

int Board::getColumns() const {

    return *columns;
}

void Board::setColumns(int columns) {

    *this->columns = columns;
}

int Board::getRows() const {

    return *rows;
}

void Board::setRows(int rows) {

    *this->rows = rows;
}

inline int Board::getPosition(int column, int row) {
    return this->board[row * (*this->columns) + column];
}

inline int Board::setPosition(int column, int row, int color) {
    return this->board[row * (*this->columns) + column] = color;
}

void Board::gravity() {
    //vertical
    //desde arriba, busca la primera baldosa tal que la de debajo este vacia
    for (int j = 0; j<*this->columns; j++) {
        for (int i = *this->rows - 1; i >= 0; i--) {
            int k = i - 1;
            if (k >= 0) {
                if (this->getPosition(j, k) == 0) {
                    while ((k < *this->rows)) {//desplaza las baldosas una posicion
                        this->setPosition(j, k, this->getPosition(j, k + 1));
                        k++;
                    }
                    /*
                    //ya se rellenan en el while, aunque eso implique dos accesos
                    for (k; k< *this->rows; k++) {//rellena a 0 las que faltan
                        this->setPosition(j, k, 0);
                    }
                     */
                    //por arriba entran ceros
                    this->setPosition(j, *this->rows - 1, 0);
                }
            }
        }
    }
    //lateral
    for (int j = *this->columns - 2; j >= 0; j--) {
        if (this->getPosition(j, 0) != 0) { // si la actual tiene baldosas
            int k = j - 1;
            if (k >= 0) { // si la anterior no se sale del tablero
                if (this->getPosition(k, 0) == 0) { // si la anterior esta vacia
                    for (int col = k; col< *this->columns - 1; col++) {//copia
                        for (int i = 0; i<*this->rows; i++) {
                            this->setPosition(col, i, this->getPosition(col + 1, i));
                        }
                    }
                    //por el lateral entran ceros
                    for (int i = 0; i<*this->rows; i++) {
                        this->setPosition(*this->columns - 1, i, 0);
                    }
                }
            }
        }
    }
}

list<set<std::pair<int, int> > > Board::getGroupMoves() {
    int elements = *this->rows * * this->columns;
    bool *usedTiles = new bool [elements];
    memset(usedTiles, false, sizeof (false) * elements);
    list<set<std::pair<int, int> > > moves;
    for (int i = 0; i<*this->rows; i++) {
        for (int j = 0; j<*this->columns; j++) {
            if (!usedTiles[i * (*this->columns) + j]) {
                set<pair<int, int> > actualMove;
                actualMove = this->getGroupMove(j, i);
                if (actualMove.size() > 0) {
                    moves.push_back(actualMove);
                    set<std::pair<int, int> >::iterator it;
                    for (it = moves.back().begin(); it != moves.back().end(); it++) {
                        usedTiles[it->second * (*this->columns) + it->first] = true;
                    }
                }
            }
        }
    }

    delete [] usedTiles;
    return moves;

}

pair<int, int> Board::getMove(std::set<std::pair<int, int> > groupMove) {
    return *groupMove.begin();
}

std::set<std::pair<int, int> > Board::getGroupMove(int x, int y) {
    std::list<std::pair<int, int> > open;
    std::set<std::pair<int, int> > closed;
    set<std::pair<int, int> > connected;
    pair<int, int> actualTile;
    open.push_back(pair<int, int>(x, y));
    int xDir[] = {0, 0, -1, 1}; //4-vecindad cutre, algo pro estaria al pelo
    int yDir[] = {1, -1, 0, 0};
    int actualX, actualY;
    if (this->getPosition(x, y) != 0) {
        while (!open.empty()) {
            actualTile = open.front();
            open.pop_front();
            closed.insert(actualTile);
            connected.insert(actualTile);
            for (int dir = 0; dir < 4; dir++) {
                actualX = actualTile.first + xDir[dir];
                actualY = actualTile.second + yDir[dir];
                if (actualX >= 0 && actualX < *this->columns) {
                    if (actualY >= 0 && actualY < *this->rows) {
                        if (this->getPosition(actualX, actualY) == this->getPosition(x, y)) {
                            pair<int, int> actual(actualX, actualY);
                            if (closed.count(actual) == 0) {
                                open.insert(open.end(), actual);
                            }
                        }
                    }
                }
            }
        }
    }
    /*
        cout << "------------------------\n";
        set<pair<int, int> >::iterator it;
        for (it = connected.begin(); it != connected.end(); it++) {
            cout << "ELEMENTO " << it->first << " " << it->second << endl;
        }
     */
    if (connected.size() < 2) {

        connected.clear();
    }
    return connected;
}

/*
 *calcula la puntuacion de un grupo
 */
int Board::score(std::set<std::pair<int, int> > tiles) {
    return tiles.size()*(tiles.size() - 1);
}

/*
 * elimina un grupo del tablero
 * NO APLICA GRAVEDAD
 * Se podria discutir si crear otro que devuelva un nuevo tablero y no modifique
 * self, mas por comodidad que otra cosa
 * Devuelve la puntuacion del grupo eliminado
 */
int Board::removeGroup(std::set<std::pair<int, int> > tiles) {
    std::set<std::pair<int, int> >::iterator it;
    for (it = tiles.begin(); it != tiles.end(); it++) {
        this->setPosition(it->first, it->second, 0);
    }
    return tiles.size()*(tiles.size() - 1);
}

// Realiza una estimación optimista del valor del tablero en el mejor caso.
int Board::funcionCota() const
{
    int valorCota = 0;
    int total = *(this->rows) * *(this->columns);
    map<int, int> cantidad;

    // Recorremos el tablero contando únicamente la cantidad de fichas de cada color.
    for (int i = 0; i < total; i++)
    {
        if (board[i] != 0)
        {
            // Si ya se había leído el color, se incrementa; si no estaba, se establece a 1 la cantidad de ese color.
            if (cantidad.count(board[i]) > 0)
            {
                cantidad[board[i]] = cantidad[board[i]] + 1;
            }
            else
            {
                cantidad[board[i]] = 1;
            }
        }
    }

    // Se calcula la puntuación considerando que siempre se deshacen esos grupos de fichas.
    for (map<int, int>::iterator i = cantidad.begin(); i != cantidad.end(); ++i) {
        valorCota += (*i).second * ((*i).second - 1);
    }

    return valorCota;
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

void Board::show() {
    for (int i = 0; i< *this->rows; i++) {
        for (int j = 0; j< *this->columns; j++) {

            std::cout << this->board[i * * this->columns + j] << ' ';
        }
        std::cout << std::endl;
    }
}

