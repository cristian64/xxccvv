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
    Board board(argv[1]);
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
        cout << "Valor máximo del tablero: " << board.funcionCota() << endl;
        if (strcmp(argv[2], "1") == 0) {
            Algorithms::currentMoves.clear();
            Algorithms::currentScore = 0;
            Algorithms::backtracking(board);
        } else if (strcmp(argv[2], "2") == 0) {
            Algorithms::greedy(board);
            Algorithms::currentMoves.clear();
            Algorithms::currentScore = 0;
            Algorithms::bound(board);
        }
        else
        {
            // Inicialización.
            Algorithms::maxMoves.clear();
            Algorithms::maxScore = 0;
            int profundidad = 0;
            while (board.getGroupMoves().size() > 0)
            {
                // Se ejecuta el algoritmo otra vez continuando desde la mejor solución encontrada en la iteración anterior.
                Algorithms::currentMoves = Algorithms::maxMoves;
                Algorithms::currentScore = Algorithms::maxScore;
                Algorithms::backtrackingLimitado(board, profundidad = profundidad + 3);

                // Se aplica la mejor secuencia de movimientos obtenidos al tablero.
                list<pair<int, int> >::iterator it;
                for (it = Algorithms::maxMoves.begin(); it != Algorithms::maxMoves.end(); it++) {
                    board.removeGroup(board.getGroupMove(it->first, it->second));
                    board.gravity();
                }

                cout << "----------------------------------------------------------------------------------------------------------" << endl;
            }
        }
        Algorithms::showMax();
    }
    cout << endl;

    return 0;
}

