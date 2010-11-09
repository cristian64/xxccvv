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
        currentMaxScore = 0;
        spawnedNodes = 0;
    }

    virtual ~AStar() {
        typename multiset<Node*>::iterator it;
        for (it = this->open.begin(); it != this->open.end(); it++) {
            delete *(it);
        }
        for (it = this->closed.begin(); it != this->closed.end(); it++) {
            delete *(it);
        }
        delete goalNode;
    }

    int step() {
        if (open.empty()) {
            cout << "NODOS ACABADOS\n";
            return -1; //ESPACIO EXPLORADO SIN EXITO;
        }

        Node *currentNode = (*open.begin());
        open.erase(open.begin());
        closed.insert(currentNode);


        typename multiset<Node*>::iterator open_node; //it sobre abiertos
        typename multiset<Node*>::iterator closed_node; //it sobre cerrados
        typename list<pair<T*, int> >::iterator childObject; //it Nodos tipo T y el coste del paso
        typename list<Node*>::iterator childNode; //it nodos tipo A* y el coste del paso
        //iteradores que indican los limites de busqueda
        pair<typename multiset<Node*>::iterator, typename multiset<Node*>::iterator > open_node_search_it;
        pair<typename multiset<Node*>::iterator, typename multiset<Node*>::iterator > closed_node_search_it;

        list<pair<T*, int> > childObjects; //hijos del nodo actual
        list<Node*> childNodes; //lista de vecinos almacenada en nodos A*
        bool open_found = false; //indica si el hijo se ha encontrado en abiertos
        bool closed_found = false; //indica si el hijo se ha encontrado en cerrados

        //obtiene los hijos del nodo actual
        childObjects = currentNode->data->childList();

        //comprueba si estamos en un tablero terminal
        if (childObjects.size() == 0) {
            cout << "TERMINAL\n";
            //comprueba si la puntuacion es maxima
            if (currentNode->g >= currentMaxScore) {
                currentMaxScore = currentNode->g;
                Node *aux = currentNode;
                //muestra la partida
                do {
                    cout << "(" << aux->data->x << "," << aux->data->y << ")";
                    aux = aux->parent;
                } while (aux != baseNode);
                cout << " " << currentNode->g << endl;
            }
            return 1;
        }

        //envuelve los nodos tablero en nodos A*
        for (childObject = childObjects.begin(); childObject != childObjects.end(); childObject++) {
            Node *aux = new Node();
            aux->parent = currentNode;
            aux->g = currentNode->g + childObject->second;
            aux->data = childObject->first;
            aux->h = aux->data->heuristic(goalNode->data);
            aux->f = aux->g + aux->h;

            //evita añadir a open nodos malos
            if (aux->g > currentMaxScore) {
                currentMaxScore = aux->g;
            }

            if (aux->f >= currentMaxScore) {
                childNodes.push_back(aux);
                //cout << "nodo   bueno, (" << aux->g << ", " << aux->h << ") " << aux->f << " > " << currentMaxScore << endl;
            } else {
                //cout << "nodo brozoso, (" << aux->g << ", " << aux->h << ") " << aux->f << " < " << currentMaxScore << endl;
                delete aux;
            }

        }

        for (childNode = childNodes.begin(); childNode != childNodes.end(); childNode++) {
            open_node_search_it = open.equal_range(*childNode);
            closed_node_search_it = closed.equal_range(*childNode);
            for (open_node = open_node_search_it.first; open_node != open_node_search_it.second; open_node++) {
                if ((*(*childNode)->data) == (*(*open_node)->data)) {
                    if ((*childNode)->g > (*open_node)->g) {
                        cout << "ESTO NO OCURRIRA\n";
                        delete (*open_node)->data;
                        //se borra el mierdero de open
                        //al final se insertara el bueno (actual)
                        open.erase(open_node); 
                    } else {
                        // si el actual es peor, no hay que insertarlo, es
                        //como si lo hubieramos encontrado
                        open_found = true;
                    }
                    break;
                }
            }

            for (closed_node = closed_node_search_it.first; closed_node != closed_node_search_it.second; closed_node++) {
                if ((*(*childNode)->data) == (*(*closed_node)->data)) {
                    closed_found = true;
                    break;
                }
            }
            //si no fue generado anteoriormente se guarda como candidato
            if (!closed_found && !open_found) {
                open.insert(*childNode);
            } else {//si ya se genero, se borra
                delete *childNode;
            }
        }
        //==26630== All heap blocks were freed -- no leaks are possible

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
    multiset<Node*, OpenNodeCompare> open;
    multiset<Node*, ClosedNodeCompare> closed;
    Node *currentSolution;
    Node *baseNode;
    Node *goalNode;
    int steps;
    int currentMaxScore;
    int spawnedNodes;

};

#endif	/* ASTAR_H */

