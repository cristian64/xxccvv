/* 
 * File:   AStar.h
 * Author: juen
 *
 * Created on 4 de noviembre de 2010, 14:44
 */

#ifndef ASTAR_H
#define	ASTAR_H
#include "AStarNode.h"
#include <vector>
using namespace std;

class AStar {
public:
    AStar();
    AStar(const AStar& orig);
    void step();
    void setBaseNode(AStarNode *baseNode);

    virtual ~AStar();
private:
    vector<AStarNode*> open;
    vector<AStarNode*> closed;
    AStarNode *currentSolution;
    AStarNode *baseNode;
    AStarNode *goalNode;
    int steps;
};

#endif	/* ASTAR_H */

