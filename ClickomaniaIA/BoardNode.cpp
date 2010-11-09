/* 
 * File:   BoardNode.cpp
 * Author: juen
 * 
 * Created on 7 de noviembre de 2010, 3:41
 */
using namespace std;
#include "BoardNode.h"
#include <list>
#include <utility>
#include <iostream>

BoardNode::BoardNode() : Board() {
    x = -1;
    y = -1;
}

BoardNode::BoardNode(const BoardNode& orig) : Board(orig) {
    x = orig.x;
    y = orig.y;
}

BoardNode::BoardNode(const std::string path) : Board(path) {
    x = -1;
    y = -1;
    g = 0;
}

BoardNode::~BoardNode() {
}

list<pair <BoardNode*, int> > BoardNode::childList() const {
    list<pair<BoardNode*, int> > boards;
    BoardNode* board;
    list<set<pair<int, int> > > groupMoves = this->getGroupMoves();
    list<set<pair<int, int> > >::iterator it;
    for (it = groupMoves.begin(); it != groupMoves.end(); it++) {
        board = new BoardNode(*this);
        board->g = board->score(*it);
        board->removeGroup(*it);
        board->x = this->getMove(*it).first;
        board->y = this->getMove(*it).second;
        boards.push_back(pair<BoardNode*, int>(board, board->score(*it)));
    }
    return boards;
}

BoardNode & BoardNode::operator=(const BoardNode &o) {
    this->Board::operator=(o);
    this->x = o.x;
    this->y = o.y;
    return *this;
}

bool BoardNode::operator==(const BoardNode &o)const {
    return this->Board::operator==(o);
}

int BoardNode::heuristic(BoardNode* o) const {
    return this->Board::optimisticBound();
}