#include "algorithms.h"

using namespace std;

int Algorithms::maxScore = 0;
list<pair<int, int> > Algorithms::maxMoves;
int Algorithms::tiempoAnterior = 0;
int Algorithms::cantidadPodas = 0;
int Algorithms::llamadasRecursivas = 0;

void Algorithms::backtracking(Board &board, list<pair<int, int> > &currentMoves, int currentScore) {

    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos
        /*
        list<pair<int, int> >::iterator it;
        for (it = actual.begin(); it != actual.end(); it++) {
            cout << "(" << it->first << ", " << it->second << "), ";
        }
          cout << actualScore << endl;
         */

        if (currentScore > Algorithms::maxScore) {
            Algorithms::maxMoves = currentMoves;
            Algorithms::maxScore = currentScore;
            list<pair<int, int> >::iterator it;
            for (it = Algorithms::maxMoves.begin(); it != Algorithms::maxMoves.end(); it++) {
                cout << "(" << it->first << ", " << it->second << "), ";
            }
            cout << Algorithms::maxScore << endl;
        }
    } else {
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            currentMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            temp.gravity();
            Algorithms::backtracking(temp, currentMoves, currentScore + board.score(*it));
            currentMoves.pop_back();
        }
    }
    return;
}

void Algorithms::bound(Board &board, list<pair<int, int> > &currentMoves, int currentScore) {
    llamadasRecursivas++;
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos
        /*
        list<pair<int, int> >::iterator it;
        for (it = actual.begin(); it != actual.end(); it++) {
            cout << "(" << it->first << ", " << it->second << "), ";
        }
          cout << actualScore << endl;
         */

        if (currentScore > Algorithms::maxScore) {
            Algorithms::maxMoves = currentMoves;
            Algorithms::maxScore = currentScore;
            list<pair<int, int> >::iterator it;
            for (it = Algorithms::maxMoves.begin(); it != Algorithms::maxMoves.end(); it++) {
                cout << "(" << it->first << ", " << it->second << "), ";
            }
            cout << Algorithms::maxScore << endl;
        }
    } else {
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            currentMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            temp.gravity();
            if (temp.funcionCota() + currentScore > Algorithms::maxScore)
            {
                bound(temp, currentMoves, currentScore + board.score(*it));
            }
            else
            {
                cantidadPodas++;
            }

            int tiempoActual = time(NULL);
            if (tiempoActual > tiempoAnterior + 3)
            {
                tiempoAnterior = tiempoActual;
                // INTRODUCIR LAS COSAS QUE SE HARAN CADA 3 SEGUNDOS
                cout << "Cantidad de podas: " << cantidadPodas << endl;
                cout << "Cantidad de llamadas: " << llamadasRecursivas << endl;
            }

            currentMoves.pop_back();
        }
    }
    return;
}

