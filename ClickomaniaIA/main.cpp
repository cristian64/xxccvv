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
#include <ctime>

using namespace std;

void aplicarSecuencia(Board & board) {
    char caracter1, caracter2, caracter3, caracter4;
    int puntos = 0;
    int puntosTotales = 0;
    int x, y;
    board.show();
    cout << "----------------------------------------";
    do {
        cin >> caracter1;
        if (caracter1 != '-') {
            cin >> x;
            cin >> caracter2;
            cin >> y;
            cin >> caracter3;
            cin >> caracter4;
            //cout << caracter1 << x << caracter2 << y << caracter3 << caracter4;

            std::set<std::pair<int, int> > broza = board.getGroupMove(x, y);
            //cout << board.score(broza) << endl;
            puntos = board.removeGroup(broza);

            puntosTotales += puntos;
            cout << endl << "(" << x << "," << y << ")->" << puntos << ", " << puntosTotales << "\n";
            board.show();
            cout << "----------------------------------------";
            if (puntos <= 0) {
                cout << "\nERRORRRRRRRRRRRR\n";
                return;
            }
        }
    } while (caracter1 != '-');
    cout << endl;
}

void manual(Board &board) {
    int x = 0, y = 0;
    int puntos = 0;
    do {
        list<std::set<std::pair<int, int> > > lista = board.getGroupMoves();
        list<set<pair<int, int> > >::iterator it;

        for (it = lista.begin(); it != lista.end(); it++) {
            cout << "(" << it->begin()->first << ", " << it->begin()->second << ")\n";
        }

        //board.showMoves(lista);
        cin >> x >> y;
        //std::set<std::pair<int, int> > broza = board->getGroupMove(atoi(argv[1]), atoi(argv[2]));
        std::set<std::pair<int, int> > broza = board.getGroupMove(x, y);
        cout << board.score(broza) << endl;
        puntos += board.removeGroup(broza);
        board.show();
        cout << endl;
        cout << puntos << endl;
    } while (x != -1);
    return;
}

void backtrackingProgresivo(Board &board) {
    // Inicialización.
    Algorithms::maxMoves.clear();
    Algorithms::maxScore = 0;
    int profundidad = 0;
    Board board2;
    list<pair<int, int> > maxMoves;
    int maxScore = 0;
    while (board.getGroupMoves().size() > 0) {
        // Se ejecuta el algoritmo otra vez continuando desde la mejor solución encontrada en la iteración anterior.
        Algorithms::currentMoves = Algorithms::maxMoves;
        Algorithms::currentScore = Algorithms::maxScore;
        profundidad += 3;
        Algorithms::backtrackingLimitado(board, 4, board2);

        maxMoves.insert(maxMoves.end(), Algorithms::maxMoves.begin(), Algorithms::maxMoves.end());
        maxScore += Algorithms::maxScore;

        Algorithms::currentMoves.clear();
        Algorithms::currentScore = 0;
        Algorithms::maxMoves.clear();
        Algorithms::maxScore = 0;

        board = board2;
        // Se aplica la mejor secuencia de movimientos obtenidos al tablero.
        /*
        list<pair<int, int> >::iterator it;
        for (it = Algorithms::maxMoves.begin(); it != Algorithms::maxMoves.end(); it++) {
            board.removeGroup(board.getGroupMove(it->first, it->second));
            cout << "(" << it->first << "," << it->second << "),";
        }
        cout << "-> " << Algorithms::maxScore << endl;
         * */
        board2.show();
        //board.show();
        cout << "----------------------------------------------------------------------------------------------------------" << endl;
    }
    list<pair<int, int> >::iterator it;
    for (it = maxMoves.begin(); it != maxMoves.end(); it++) {
        cout << "(" << it->first << "," << it->second << "),";
    }
    cout << "-> " << maxScore << endl;
}

int main(int argc, char** argv) {
    time_t t1, t2;
    time(&t1);
    Board board(argv[1]);
    if (argc < 3) {
        aplicarSecuencia(board);
        //parchazo para mirar los movimientos posibles al pasarle un tablero intermedio
        //board.show();
        //board.getGroupMoves();
        //board.showMoves(board.getGroupMoves());
    } else {
        switch (atoi(argv[2])) {
            case 0:
                manual(board);
                break;
            case 1:
                Algorithms::currentMoves.clear();
                Algorithms::currentScore = 0;
                Algorithms::backtracking(board);
                Algorithms::showMax();
                break;
            case 2:
                Algorithms::greedy(board);
                Algorithms::currentMoves.clear();
                Algorithms::currentScore = 0;
                Algorithms::bound(board);
                Algorithms::showMax();
                break;
            case 3:
                backtrackingProgresivo(board);
                break;
            case 4:
                break;
            default:
                break;
        }
    }
    cout << endl;
    time(&t2);
    cout << "Tiempo: " << difftime(t2, t1) << endl;
    return 0;
}
