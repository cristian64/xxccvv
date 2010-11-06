/* 
 * File:   AStar.h
 * Author: juen
 *
 * Created on 4 de noviembre de 2010, 14:44
 */

#ifndef ASTAR_H
#define	ASTAR_H
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

    AStar() {
        currentSolution = 0;
        baseNode = 0;
        goalNode = 0;
        steps = 0;
    }

    int step() {
        if (this->open.empty()) {
            return -1; //ESPACIO EXPLORADO SIN EXITO;
        }
        this->steps++;

        Node *currentNode = this->open.front(); //nodo actual
        pop_heap(this->open.begin(), this->open.end(), Node::NodeCompare());
        this->open.pop_back(); //pop_heap se lleva el primero al final
        if (*(currentNode->data) == *(this->goalNode->data)) {//hacer esto bien, la comparacion sera mejorable?
            this->goalNode = currentNode->parent;
            Node *parentNode = this->goalNode->parent;
            Node *childNode = this->goalNode;
            do {
                parentNode->child = childNode;
                childNode = parentNode;
                parentNode = parentNode->parent;
            } while (this->baseNode != childNode);

            //falta liberar memoria etc etc
            return 0; //OBJETIVO  ENCONTRADO
        } else {
            typename vector<Node*>::iterator closed_node;
            typename vector<Node*>::iterator open_node;
            typename list<Node*>::iterator childNode;
            typename list<T>::iterator childObject;
            //generar hijos
            list<T*> childs = currentNode->data->childList(); //<---------------------
            list<Node*> childNodes;
            for (childObject = childs.begin(); childObject != childs.end(); childObject++) {
                Node* aux = new Node;
                aux->data = *childObject;
                childNodes.push_back(aux);
            }

            //para cada hijo
            for (childNode = childNodes.begin(); childNode != childNodes.end(); childNode++) {
                float childG = currentNode->g + currentNode->data->COSTE_HASTA(childNode); //<-----------------------
                //se busca en abiertos
                for (open_node = this->open.begin(); open_node != this->open.end(); open_node++) {
                    open_node->dat
                    if (*(open_node->data) == *(childNode->data)) {
                        break;
                    }
                }
                //<--------- ya se optimizara
                //se busca en cerrados
                for (closed_node = this->closed.begin(); closed_node != this->closed.end(); closed_node++) {
                    if (*(closed_node->data) == *(childNode->data)) {
                        break;
                    }
                }

                if (open_node != open.end()) {
                    if (open_node->g <= childG) {
                        delete *childNode;
                        childNode = *open_node;
                    }
                }

                if (closed_node != closed.end()) {
                    if (closed_node->g <= childG) {
                        delete *childNode;
                        childNode = *closed_node;
                    }
                }

                childNode->g = childG;
                childNode->h = childNode->data->ESTIMACION_HASTA_OBJETIVO(this->goalNode); //<------------------
                childNode->f = childNode->g + childNode->h;
                childNode->parent = currentNode;

                if (closed_node != closed.end()) {
                    closed.erase(closed_node);
                }

                if (open_node != open.end()) {
                    open.erase(open_node);
                    make_heap(open.begin(), open.end(), Node::NodeCompare);
                }
                open.push_back(childNode);
                push_heap(open.begin(), open.end(), Node::NodeCompare);
            }
            this->closed.push_back(currentNode);
        }
        return 1; //busqueda incompleta
    }

    int run() {
        //se debe poder para la busqueda, habra que meter alguna historia
        //que mida el tiempo.
        int status = 1;
        while (status == 1) {//forma clasica, busca objetivos
            status = this->step();
        }
        return status;
    }

    void setBaseNode(T *node) {
        this->baseNode->data = node;
        this->baseNode->parent = 0;
        this->baseNode->child = 0;
        this->baseNode->g = 0;
        this->baseNode->h = 0; //<----------------algo hay que estimar, funcionCota()?
        this->baseNode->f = this->baseNode->g + this->baseNode->h;

        this->open.push_back(this->baseNode);
        push_heap(this->open.begin(), this->open.end(), Node::NodeCompare);
        this->steps = 0;
    }

    void setGoalNode(T *node) {
        this->goalNode->data = node;
        this->goalNode->parent = 0;
        this->goalNode->child = 0;
        this->goalNode->g = 0;
        this->goalNode->h = 0; //<---------------algo hay que estimar?, funcionCota()?
        this->goalNode->f = this->goalNode->g + this->goalNode->h;
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

