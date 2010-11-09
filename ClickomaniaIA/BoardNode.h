/* 
 * File:   BoardNode.h
 * Author: juen
 *
 * Created on 7 de noviembre de 2010, 3:41
 */

#ifndef BOARDNODE_H
#define	BOARDNODE_H

#include "NodeInterface.h"
#include "Board.h"
#include <utility>
#include <list>

class BoardNode : public Board, public NodeInterface<BoardNode> {
public:
    BoardNode();
    BoardNode(const BoardNode& orig);
    BoardNode(const std::string path);
    virtual ~BoardNode();
    std::list<std::pair <BoardNode*, int> >childList() const;
    BoardNode & operator=(const BoardNode &o);
    bool operator==(const BoardNode &o) const;
    int heuristic(BoardNode *goal) const;
    int x; //movimiento que ha llevado al tablero actual
    int y;
    int g; //vamos a ver si hay alguna sucesion de putuaciones mal
protected:


};

#endif	/* BOARDNODE_H */

