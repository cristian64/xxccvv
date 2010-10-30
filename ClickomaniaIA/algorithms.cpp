#include "algorithms.h"

using namespace std;

static void backtracking::run(Board board, list<pair<int, int> > currentMoves, int currentScore) {
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos
        /*
        list<pair<int, int> >::iterator it;
        for (it = actual.begin(); it != actual.end(); it++) {
            cout << "(" << it->first << ", " << it->second << "), ";
        }
          cout << actualScore << endl;
         */

        if (currentScore > backtracking::maxScore) {
            backtracking::maxMoves = currentMoves;
            backtracking::maxScore = currentScore;
            list<pair<int, int> >::iterator it;
            for (it = backtracking::maxMoves.begin(); it != backtracking::maxMoves.end(); it++) {
                cout << "(" << it->first << ", " << it->second << "), ";
            }
            cout << backtracking::maxScore << endl;
        }
    } else {
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            currentMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            temp.gravity();
            run(temp, currentMoves, currentScore + board.score(*it));
            currentMoves.pop_back();
        }
    }
    return;
}

void bound(Board &board, list<pair<int, int> > &maxMoves, int &maxScore, list<pair<int, int> > actualMoves, int actualScore) {
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos
        if (actualScore > maxScore) {
            maxMoves = actualMoves;
            maxScore = actualScore;
            list<pair<int, int> >::iterator it;
            for (it = maxMoves.begin(); it != maxMoves.end(); it++) {
                cout << "(" << it->first << ", " << it->second << "), ";
            }
            cout << maxScore << endl;
        }
    } else {
        list<set<pair<int, int> > >::iterator it;
        for (it = moves.begin(); it != moves.end(); it++) {
            actualMoves.push_back(*it->begin());
            Board temp = board;
            temp.removeGroup(*it);
            temp.gravity();
            //if (temp.funcionCota() + actualScore > maxScore) {
            bound(temp, maxMoves, maxScore, actualMoves, actualScore + board.score(*it));
            //}
            actualMoves.pop_back();
        }
    }
    return;
}
