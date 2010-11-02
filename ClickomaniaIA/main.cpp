/* 
 * File:   main.cpp
 * Author: juen
 *
 * Created on 24 de octubre de 2010, 16:12
 */

#include <cstdlib>
#include <iostream>
#include <list>
#include "Board.h"
#include <cstring>
#include "algorithms.h"

using namespace std;

int main(int argc, char** argv) {
    Board board(argv[1]); // = new Board(argv[1]);
    if (false) {//para jugar de forma manual
        /*
        int x, y;
        int puntos = 0;
        do {
            
            list<std::set<std::pair<int, int> > > lista = board->getGroupMoves();
            board->showMoves(lista);
            cin >> x >> y;
            //std::set<std::pair<int, int> > broza = board->getGroupMove(atoi(argv[1]), atoi(argv[2]));
            std::set<std::pair<int, int> > broza = board->getGroupMove(x, y);
            cout << board->score(broza) << endl;
            puntos += board->removeGroup(broza);
            board->gravity();
            board->show();
            cout << endl;
            cout << puntos << endl;

        } while (x != -1);
         */
    } else {
        if (strcmp(argv[2], "1") == 0) {
            Algorithms::currentMoves.clear();
            Algorithms::currentScore = 0;
            Algorithms::backtracking(board);
        } else {
            cout << "Valor máximo del tablero: " << board.funcionCota() << endl;
            Algorithms::greedy(board);
            Algorithms::currentMoves.clear();
            Algorithms::currentScore = 0;
            Algorithms::bound(board);
        }
        Algorithms::showMax();
    }
    cout << endl;
    //delete board;

    return 0;
}

