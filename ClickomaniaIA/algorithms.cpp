#include "algorithms.h"

using namespace std;
void backtracking(Board board, list<pair<int, int> > &maxMoves, int &maxScore, list<pair<int, int> > actualMoves, int actualScore) {
    list<set<pair<int, int> > > moves = board.getGroupMoves();
    if (moves.size() == 0) {//la partida acaba cuando no quedan grupos
        /*
        list<pair<int, int> >::iterator it;
        for (it = actual.begin(); it != actual.end(); it++) {
            cout << "(" << it->first << ", " << it->second << "), ";
        }
          cout << actualScore << endl;
         */

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
            backtracking(temp, maxMoves, maxScore, actualMoves, actualScore + board.score(*it));
            actualMoves.pop_back();
        }
    }
    return;
}
