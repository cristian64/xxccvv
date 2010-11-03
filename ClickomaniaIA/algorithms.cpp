#include "algorithms.h"

using namespace std;

int Algorithms::maxScore = 0;
int Algorithms::currentScore = 0;
list<pair<int, int> > Algorithms::maxMoves;
list<pair<int, int> > Algorithms::currentMoves;
int Algorithms::tiempoAnterior = 0;
int Algorithms::cantidadPodas = 0;
int Algorithms::llamadasRecursivas = 0;
int Algorithms::sumaAlturaPodas = 0;

void Algorithms::backtracking(Board &board) {
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos
        if (currentScore > Algorithms::maxScore) {
            Algorithms::maxMoves = currentMoves;
            Algorithms::maxScore = currentScore;
            Algorithms::showMax();
        }
    } else {
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            currentMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            //temp.gravity();
            int groupScore = board.score(*it);
            currentScore += groupScore;
            Algorithms::backtracking(temp);
            currentScore -= groupScore;
            currentMoves.pop_back();
        }
    }
    return;
}

void Algorithms::backtrackingLimitado(Board &board, int profundidad, Board &board2) {
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0 || currentMoves.size() >= (unsigned) profundidad) {//la partida acaba cuando no quedan grupos
        if (currentScore > Algorithms::maxScore) {
            Algorithms::maxMoves = currentMoves;
            Algorithms::maxScore = currentScore;
            list<pair<int, int> >::iterator it;
            board2 = board;

            for (it = Algorithms::maxMoves.begin(); it != Algorithms::maxMoves.end(); it++) {
                cout << "(" << it->first << "," << it->second << "),";
            }
            cout << "-> " << Algorithms::maxScore << endl;
        }
    } else {
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            currentMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            temp.gravity();
            int groupScore = board.score(*it);
            currentScore += groupScore;
            Algorithms::backtrackingLimitado(temp, profundidad, board2);
            currentScore -= groupScore;
            currentMoves.pop_back();
        }
    }
    return;
}

bool comparador(const set<pair<int, int> > &g1, const set<pair<int, int> > &g2) {
    return g1.size() < g2.size();
}

void Algorithms::bound(Board &board) {
    //cout << board.funcionCota() << endl;
    //return;
    llamadasRecursivas++;
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos

        if (currentScore > Algorithms::maxScore) {
            Algorithms::maxMoves = currentMoves;
            Algorithms::maxScore = currentScore;
            Algorithms::showMax();
        }
    } else {
        moves.sort(comparador);
        list<set<pair<int, int> > >::iterator it;
        int groupScore;
        for (it = moves.begin(); it != moves.end(); it++) {
            currentMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            //temp.gravity();
            groupScore = board.score(*it);
            currentScore += groupScore;
            //bound(temp, currentMoves, currentScore);


            if (temp.funcionCota() + currentScore > Algorithms::maxScore) {
                bound(temp);

            } else {
                cantidadPodas++;
                sumaAlturaPodas += currentMoves.size();
            }
/*
            int tiempoActual = time(NULL);
            if (tiempoActual > tiempoAnterior + 3) {
                tiempoAnterior = tiempoActual;
                // INTRODUCIR LAS COSAS QUE SE HARAN CADA 3 SEGUNDOS
                //cout << "Cantidad de podas:    " << cantidadPodas << endl;
                //cout << "Cantidad de llamadas: " << llamadasRecursivas << endl;
                //cout << "Altura media de poda: " << sumaAlturaPodas / (float) cantidadPodas << endl;
            }
 */
            currentScore -= groupScore;
            currentMoves.pop_back();
        }
    }
    return;
}

void Algorithms::greedy(Board board) {
    maxMoves.clear();
    maxScore = 0;

    list<set<pair<int, int> > > moves = board.getGroupMoves();
    while (moves.size() > 0) {
        set<pair<int, int> > *mayorGrupo = NULL;
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            if (mayorGrupo == NULL || (*it).size() > mayorGrupo->size()) {
                mayorGrupo = &(*it);
            }
        }

        maxMoves.push_back(*mayorGrupo->begin());
        maxScore += board.score(*mayorGrupo);

        board.removeGroup(*mayorGrupo);
        board.gravity();
        moves = board.getGroupMoves();
    }

    showMax();
    return;
}

void Algorithms::showMax() {
    list<pair<int, int> >::iterator it;
    for (it = Algorithms::maxMoves.begin(); it != Algorithms::maxMoves.end(); it++) {
        cout << "(" << it->first << "," << it->second << "),";
    }
    cout << "-> " << Algorithms::maxScore << endl;
}
