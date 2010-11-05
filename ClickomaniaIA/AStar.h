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
#include <list>
using namespace std;

template <class T> class AStar {
public:

    class Node {
    public:
        T *data;
        Node *parent;
        Node *child;
        float g;
        float h;
        float f;

        Node() : data(0), parent(0), child(0), g(0.0f), h(0.0f), f(0.0f) {
        }
    };

    class NodeCompare {
    public:

        bool operator() (const Node *x, const Node*y) const {
            return x->f < y->f;
        }
    };
public:
    class Node;
    AStar();
    AStar(const AStar& orig);

    int step() {
        if (this->open.empty()) {
            return 0;
        }
        this->steps++;

        Node *currentNode = this->open.front(); //nodo actual
        pop_heap(this->open.begin(), this->open.end(), Node::NodeCompare);
        this->open.pop_back(); //pop_heap se lleva el primero al final
        if (*currentNode == *this->goalNode) {//hacer esto bien,
            this->goalNode = currentNode->parent;
            while (false) {// <------------------------------------------
                //montar el camino solucion
            }
            //falta liberar memoria etc etc
        } else {
            typename vector<Node*>::iterator closed_node;
            typename vector<Node*>::iterator open_node;
            typename list<Node*>::iterator child;
            //generar hijos
            list<Node*> childs = currentNode->data->getChilds();
            //para cada hijo

            int ESTIMACION = 0;
            for (child = childs.begin(); child != childs.end(); child++) {
                float newg = currentNode->g + ESTIMACION;
                //  se busca en abiertos
                for (open_node = this->open.begin(); open_node != this->open.end(); open_node++) {
                    if (*(open_node->data) == *(child->data)) {
                        break;
                    }
                }
                //se busca en cerrados
                for (closed_node = this->closed.begin(); closed_node != this->closed.end(); closed_node++) {
                    if (*(closed_node->data) == *(child->data)) {
                        break;
                    }
                }

                if (open_node != open.end()) {
                    if (open_node->g <= newg) {
                        delete *child;
                        child = *open_node;
                    }
                }

                if (closed_node != closed.end()) {
                    if (closed_node->g <= newg) {
                        delete *child;
                        child = *closed_node;
                    }
                }

                child->g = newg;
                child->h = ESTIMACION;
                child->f = child->g + child->h;
                child->parent = currentNode;

                if (closed_node != closed.end()) {
                    closed.erase(closed_node);
                }

                if (open_node != open.end()) {
                    open.erase(open_node);
                    make_heap(open.begin(), open.end(), Node::NodeCompare);
                }
                open.push_back(child);
                push_heap(open.begin(), open.end(), Node::NodeCompare);
            }
            this->closed.push_back(currentNode);
        }
        return 0; //cambiar
    }
    void run();

    void setBaseNode(Node *baseNode) {
        this->baseNode = baseNode;
        this->baseNode->g = 0;
        this->baseNode->h = 0; //algo hay que estimar, funcionCota()?
        this->open.push_back(this->baseNode);
        push_heap(this->open.begin(), this->open.end(), Node::NodeCompare);
        this->steps = 0;
    }

    virtual ~AStar();
private:
    vector<Node*> open;
    vector<Node*> closed;
    Node *currentSolution;
    Node *baseNode;
    Node *goalNode;
    int steps;
};

#endif	/* ASTAR_H */

