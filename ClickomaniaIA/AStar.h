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
#include <algorithm>
#include <set>
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

        ~Node() {
            delete data;
        }
    };

    class OpenNodeCompare {
    public:

        bool operator() (const Node* x, const Node* y) const {
            return x->f > y->f;
        }
    };

    class ClosedNodeCompare {
    public:

        bool operator() (const Node* x, const Node* y) const {
            return x->h > y->h;
        }
    };

public:

    AStar() {
        currentSolution = 0;
        baseNode = new Node;
        goalNode = new Node;
        steps = 0;
        spawnedNodes = 0;
    }

    virtual ~AStar() {
        //typename vector<Node*>::iterator it;
        typename multiset<Node*>::iterator it;
        for (it = this->open.begin(); it != this->open.end(); it++) {
            delete *(it);
        }
        for (it = this->closed.begin(); it != this->closed.end(); it++) {
            delete *(it);
        }
        //delete goalNode;
    }

    int step() {
        if (this->open.empty()) {
            cout << "NODOS ACABADOS\n";
            return -1; //ESPACIO EXPLORADO SIN EXITO;
        }

        this->steps++;

        //Node *currentNode = this->open.front(); //nodo actual
        //pop_heap(this->open.begin(), this->open.end(), NodeCompare());
        //this->open.pop_back(); //pop_heap se lleva el primero al final

        Node *currentNode = (*this->open.begin());
        this->open.erase(this->open.begin());

        /*if (*(currentNode->data) == *(this->goalNode->data)) {
            this->goalNode->parent = currentNode->parent;
            Node *parentNode = this->goalNode->parent;
            Node *childNode = this->goalNode;

            do {
                parentNode->child = childNode;
                childNode = parentNode;
                parentNode = parentNode->parent;
            } while (this->baseNode != childNode);

            this->currentSolution = this->baseNode;

            //se añade el nodo final a closed, mas que nada para liberarlo con los demas
            this->closed.push_back(currentNode);

            return 0; //OBJETIVO  ENCONTRADO
        } else {*/
        typename multiset<Node*>::iterator open_node;
        typename multiset<Node*>::iterator closed_node;
        //typename vector<Node*>::iterator closed_node; //it sobre cerrados
        //typename vector<Node*>::iterator open_node; //it sobre abiertos
        typename list<pair<Node*, int> >::iterator childNode; //it nodos tipo A* y el coste del paso
        typename list<pair<T*, int> >::iterator childObject; //it Nodos tipo T y el coste del paso
        list<pair<Node*, int> > childNodes; //lista de vecinos en nodos A*

        //pide los vecinos del nodo y el coste de transicion
        list<pair<T*, int> > childs = currentNode->data->childList();
        //cout << "CHILDSIZE: " << childs.size() << endl;
        //añadido para clickomania
        //-----------------------------------------------------------------
        if (currentNode->g > this->currentMaxScore) {
            this->goalNode = currentNode;
            currentMaxScore = currentNode->g;
            cout << "MAX puntuacion intermedia: " << currentMaxScore << endl;
        }

        if (currentNode->f < currentMaxScore) {
            closed.insert(currentNode);
            //closed.push_back(currentNode);
            return 1;
        }

        if (childs.size() == 0) {//hemos llegado a una solucion
            cout << "\nSOLUCION CON G: " << currentNode->g << endl;
            if (currentNode->g >= this->currentMaxScore) {
                this->goalNode = currentNode;
                currentMaxScore = currentNode->g;
                Node *aux = goalNode;
                do {
                    cout << "(" << aux->data->x << ", " << aux->data->y << ")";
                    aux = aux->parent;
                } while (aux != baseNode);
                cout << endl;
                return -1;
            }
        }
        //-----------------------------------------------------------------
        //envuelve los nodos del problema en nodos de A*
        for (childObject = childs.begin(); childObject != childs.end(); childObject++) {
            Node* aux = new Node;
            aux->data = childObject->first; //iguala punteros
            childNodes.push_back(pair<Node*, int>(aux, childObject->second));

        }
        bool open_found = false;
        bool closed_found = false;
        //para cada hijo
        for (childNode = childNodes.begin(); childNode != childNodes.end(); childNode++) {
            float childG = currentNode->g + childNode->second;
            float childH = childNode->first->data->heuristic(this->goalNode->data);
            //float childH = childNode->first->data->entropy
            childNode->first->g = childG;
            childNode->first->h = childH;
            childNode->first->f = childNode->first->g + childNode->first->h;
            childNode->first->parent = currentNode;

            //se busca en abiertos
            pair<typename multiset<Node*>::iterator, typename multiset<Node*>::iterator > it2;
            it2 = this->open.equal_range((childNode->first));

            for (open_node = it2.first; open_node != it2.second; open_node++) {
                if (*((*open_node)->data) == *(childNode->first->data)) {
                    //open_found = true;
                    break;
                }
            }

            it2 = this->closed.equal_range(childNode->first);
            for (closed_node = it2.first; closed_node != it2.second; closed_node++) {
                if (*((*closed_node)->data) == *(childNode->first->data)) {
                    //closed_found = true;
                    break;
                }
            }


            //cout << "MI PADRE ES:\n";
            //childNode->first->parent->data->show();

            if (open_found) {
                delete childNode->first; //olvida el nuevo
            } else if (closed_found) {
                delete childNode->first; //olvida el nuevo
            } else {
                spawnedNodes++;
                open.insert(childNode->first);
            }
        }
        this->closed.insert(currentNode);
        //}
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

    void setBaseNode(T node) {
        this->baseNode->data = new T;
        *this->baseNode->data = node;
        this->baseNode->parent = 0;
        this->baseNode->child = 0;
        this->baseNode->g = 0;
        this->baseNode->h = node.heuristic(this->goalNode->data);
        this->baseNode->f = this->baseNode->g + this->baseNode->h;
        //this->open.push_back(this->baseNode);
        this->open.insert(this->baseNode);
        //push_heap(this->open.begin(), this->open.end(), NodeCompare());
        this->steps = 0;
    }

    void setGoalNode(T node) {
        this->goalNode->data = new T;
        *this->goalNode->data = node;
        this->goalNode->parent = 0;
        this->goalNode->child = 0;
        this->goalNode->g = 0;
        this->goalNode->h = 0;
        this->goalNode->f = this->goalNode->g + this->goalNode->h;
    }

    void showSolution() {
    }

    list<T*> solution() {
        list<T*> solution;
        Node* currentNode = this->baseNode;
        while (currentNode != this->goalNode) {
            solution.push_back(currentNode->data);
            currentNode = currentNode->child;
        }
        solution.push_back(currentNode->data); //no

        return solution;
    }

    int getSteps() {
        return steps;
    }

    int getCurrentMaxScore() const {
        return currentMaxScore;
    }

    void setCurrentMaxScore(int currentMaxScore) {
        this->currentMaxScore = currentMaxScore;
    }

    int getSpawnedNodes() const {
        return spawnedNodes;
    }

protected:
    //vector<Node*> open;
    multiset<Node*, OpenNodeCompare> open;
    multiset<Node*, ClosedNodeCompare> closed;
    //vector<Node*> closed;
    Node *currentSolution;
    Node *baseNode;
    Node *goalNode;
    int steps;
    int currentMaxScore;
    int spawnedNodes;

};

#endif	/* ASTAR_H */

