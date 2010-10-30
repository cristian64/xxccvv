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

void backtracking(Board board, list<pair<int, int> > &maxMoves, int &maxScore, list<pair<int, int> > actualMoves, int actualScore);

#endif	/* ALGORITHMS_H */

