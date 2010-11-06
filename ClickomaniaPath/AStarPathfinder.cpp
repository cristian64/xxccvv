/* 
 * File:   AStarPathfinder.cpp
 * Author: juen
 * 
 * Created on 5 de noviembre de 2010, 15:06
 */

#include "AStarPathfinder.h"
#include <iostream>
using namespace std;
Node::Node() {
}

Node::Node(const Node& orig) {
    this->x = orig.x;
    this->y = orig.y;
}

Node::~Node() {
    ;
}

Node::Node(int x, int y) {
    this->x = x;
    this->y = y;
}

bool Node::operator==(Node &o){
    //cout << "comparando NODOS\n";
    return this->x == o.x && this->y == o.y;
}

void Node::setMap(QTableWidget *map){
    this->map = map;
}

list< pair<Node*, int> > Node::childList() {
    list<pair<Node*, int> > childs;
    int xDir[] = {0, 1, 0, -1};
    int yDir[] = {1, 0, -1, 0};
    int currentX, currentY;
    for (int i = 0; i < 4; i++) {
        currentX = x + xDir[i];
        currentY = y + yDir[i];
          if (currentX < map->columnCount() && currentX >= 0) {
            if (currentY < map->rowCount() && currentY >= 0) {
                Node* aux = new Node();
                aux->x = currentX;
                aux->y = currentY;
                aux->map = this->map;
                childs.push_back(pair<Node*, int>(aux, this->map->item(currentY, currentX)->text().toInt()));
                //cout << "Generado vecino: " << currentX << ' ' << currentY << " "<<  this->map->item(currentY, currentX)->text().toInt() << endl;
            }
        }
    }
    return childs;
}

