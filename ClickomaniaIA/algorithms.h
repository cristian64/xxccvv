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
public:

    static list<pair<int, int> > maxMoves;
    static list<pair<int, int> > currentMoves;
    static int maxScore;
    static int currentScore;
    static int tiempoAnterior;
    static int cantidadPodas;
    static int llamadasRecursivas;
    static int sumaAlturaPodas;

public:
    static void backtracking(Board &board);
    static void backtrackingLimitado(Board &board, int profundidad);
    static void bound(Board &board);
    static void greedy(Board board);

    static void showMax();
};
#endif	/* ALGORITHMS_H */

