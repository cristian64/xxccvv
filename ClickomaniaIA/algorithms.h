/* 
 * File:   algorithms.h
 * Author: juen
 *
 * Created on 31 de octubre de 2010, 1:19
 */

#ifndef ALGORITHMS_H
#define	ALGORITHMS_H

#include <list>
#include <iostream>
#include "Board.h"

using namespace std;

/*
 * board: Tablero de juego.
 * maxMoves: Secuencia de movimientos que ha llevado a la puntuacion maxima alcanzada.
 * maxScore: Puntuacion maxima alcanzada.
 * actualMoves: Secuencia actual de movimientos.
 * actualScore: Puntuacion asociada a la secuencia actual de movimientos.
 */

struct boardCompare {

    bool operator() (const Board& lhs, const Board & rhs) const {
        return lhs.heuristic(NULL) < rhs.heuristic(NULL);
    }
};

class Algorithms {
public:


    static list<pair<int, int> > maxMoves;
    static list<pair<int, int> > currentMoves;
    //static set<pair<Board, pair<list<pair<int, int> > > > >,  boardCompare> generatedBoards;
    static set<pair<Board, pair<int, list<pair<int, int> > > > > generatedBoards;
    static int maxScore;
    static int currentScore;
    static int tiempoAnterior;
    static int cantidadPodas;
    static int llamadasRecursivas;
    static int sumaAlturaPodas;

public:
    static void backtracking(Board &board);
    static void backtrackingLimitado(Board &board, int profundidad, Board &board2);
    static void bound(Board &board);
    static void greedy(Board board);

    static void showMax();
};
#endif	/* ALGORITHMS_H */

