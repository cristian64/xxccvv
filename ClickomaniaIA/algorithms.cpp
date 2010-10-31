#include "algorithms.h"

using namespace std;

int Algorithms::maxScore = 0;
list<pair<int, int> > Algorithms::maxMoves;

void Algorithms::backtracking(Board board, list<pair<int, int> > currentMoves, int currentScore) {

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
            backtracking(temp, currentMoves, currentScore + board.score(*it));
            currentMoves.pop_back();
        }
    }
    return;
}

void Algorithms::bound(Board board, list<pair<int, int> > currentMoves, int currentScore) {
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
                backtracking(temp, currentMoves, currentScore + board.score(*it));
            currentMoves.pop_back();
        }
    }
    return;
}

