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
//void backtracking(Board board, list<pair<int, int> > &maxMoves, int &maxScore, list<pair<int, int> > actualMoves, int actualScore);

class Algorithms {
private:

    static list<pair<int, int> > maxMoves;
    static int maxScore;
    static int tiempoAnterior;
    static int cantidadPodas;
    static int llamadasRecursivas;

public:
    static void backtracking(Board &board, list<pair<int, int> > &currentMoves, int currentScore);
    static void bound(Board &board, list<pair<int, int> > &currentMoves, int currentScore);
    static void greedy(Board board);


};
#endif	/* ALGORITHMS_H */

