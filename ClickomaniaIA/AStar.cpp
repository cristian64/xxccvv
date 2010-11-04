/* 
 * File:   AStar.cpp
 * Author: juen
 * 
 * Created on 4 de noviembre de 2010, 14:44
 */

#include <queue>

#include "AStar.h"

AStar::AStar() {
}

AStar::AStar(const AStar& orig) {
}

AStar::~AStar() {
}

void AStar::setBaseNode(AStarNode *baseNode) {
    this->baseNode = baseNode;
    this->baseNode->SetG(0);
    this->baseNode->SetH(0); //algo hay que estimar, funcionCota()?
    this->open.push_back(this->baseNode);
    push_heap(this->open.begin(), this->open.end(), AStarNode::NodeCompare)
            this->steps = 0;
}

int AStar::step() {
    if (this->open.empty()) {
        return 0;
    }
    this->steps++;
    AStarNode *currentNode = this->open.front(); //nodo actual
    pop_heap(this->open.begin(), this->open.end(), AStarNode::NodeCompare);
    this->open.pop_back(); //pop_heap se lleva el primero al final
    if (*currentNode == *this->goalNode) {//hacer esto bien,
        this->goalNode = currentNode->GetBestParent();
        //falta liberar memoria etc etc
    } else {
        //generar hijos
        list<AStarNode*> childs = currentNode->GetChildNodes();
        //para cada hijo
        list<AStarNode*>::iterator child;
        for (child = childs.begin(); child != childs.end(); child++) {
            //  se busca en abiertos
            vector<AStarNode*>::iterator open_member;
            for (open_member = this->open.begin(); open_member != this->open.end(); open_member++) {
                if (*(open_member->getData()) == *(child->getData())) {
                    if (open_member->GetG() > child->GetG()) {
                        open_member->SetBestParent(child->GetBestParent());
                        open_member->SetG(child->GetG());
                        delete child;
                        break;
                    }
                }
            }

        }

        //      si es el caso y toca se actualiza
        //  se busca en cerrados
        //      si cuesta menos se borran sus hijos y se devuelvea a abiertos
        //si es nuevo se mete en abiertos
        //nodo se se mete en cerrados



    }
}