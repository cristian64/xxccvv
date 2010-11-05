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
        ~Node(){
            delete data;
            //delete parent;
            //delete child;
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
        baseNode = new Node;
        goalNode = new Node;
        steps = 0;
    }

    int step() {
        if (this->open.empty()) {
            return -1; //ESPACIO EXPLORADO SIN EXITO;
        }
        this->steps++;

        Node *currentNode = this->open.front(); //nodo actual
        pop_heap(this->open.begin(), this->open.end(), NodeCompare());
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
            typename vector<Node*>::iterator closed_node;//it sobre cerrados
            typename vector<Node*>::iterator open_node;//it sobre abiertos
            typename list<pair<Node*, int> >::iterator childNode;//it nodos tipo A* y el coste del paso
            typename list<pair<T*, int> >::iterator childObject; //it Nodos tipo T y el coste del paso
            list<pair<Node*, int> > childNodes;//lista de vecinos en nodos A*

            //pide los vecinos del nodo y el coste de transicion
            list<pair<T*, int> > childs = currentNode->data->childList();

            //convierte los nodos que nos da en nodosA*
            for (childObject = childs.begin(); childObject != childs.end(); childObject++) {
                Node* aux = new Node;    
                cout << (childObject->first)->x << ' ' << childObject->second << endl;
                aux->data = childObject->first;//iguala punteros
                childNodes.push_back(pair<Node*, int>(aux, childObject->second));
            }

            //para cada hijo
            for (childNode = childNodes.begin(); childNode != childNodes.end(); childNode++) {
                float childG = currentNode->g + childNode->second;
                cout <<"SOY G: " << childG << endl;
                //se busca en abiertos
                for (open_node = this->open.begin(); open_node != this->open.end(); open_node++) {
                    if ((*open_node)->data == childNode->first->data) {
                        cout << "ESTOY EN ABIERTOS" << endl;
                        break;
                    }
                }
                //<--------- ya se optimizara, si esta en abiertos no deberia estar en cerrados
                //se busca en cerrados
                for (closed_node = this->closed.begin(); closed_node != this->closed.end(); closed_node++) {
                    if (((*closed_node)->data) == childNode->first->data) {
                        cout << "ESTOY EN CERRADOS"<< endl;
                        break;
                    }
                }

                childNode->first->g = childG;
                //childNode->first->h = childNode->first->data->ESTIMACION_HASTA_OBJETIVO(this->goalNode); //<------------------
                childNode->first->f = childNode->first->g + childNode->first->h;
                childNode->first->parent = currentNode;

                if (open_node != open.end()) {//si se encontro en aviertos
                    if ((*open_node)->g <= childG) {//es peor que lo que tenemos
                        delete childNode->first;//olvida el nuevo
                       // (*childNode) = open_node;
                    }else{//es mejor que lo que tenemos
                        open.erase(open_node);//se saca el viejo
                        make_heap(open.begin(), open.end(), NodeCompare());//se reordena el heap
                        open.push_back(childNode->first);//se mete el nuevo
                        push_heap(open.begin(), open.end(), NodeCompare());//se reordena el heap
                    }
                }

                if (closed_node != closed.end()) {//si se encontro en cerrados
                    if ((*closed_node)->g <= childG) {//es peor que lo que teniamos
                        delete childNode->first;//olvida el nuevo
                        //childNode = *closed_node;
                    }else{//es mejor que lo que tenemos
                        closed.erase(closed_node);//se saca de cerrados
                        open.push_back(childNode->first);//se mete en abiertos
                        push_heap(open.begin(), open.end(), NodeCompare());//se reordena el heap
                    }
                }

                //si no se encontro->es nuevo
                if (open_node == open.end() && closed_node == closed.end()) {
                    open.push_back(childNode->first);//se mete en abiertos
                    push_heap(open.begin(), open.end(), NodeCompare());//se reordena el heap
                }

            }
            this->closed.push_back(currentNode);
            cout << "NODOS EN CLOSED: " << this->closed.size() << endl;
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
        push_heap(this->open.begin(), this->open.end(), NodeCompare());
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

    virtual ~AStar(){
    }
    int steps;
private:
    vector<Node*> open;
    vector<Node*> closed;
    Node *currentSolution;
    Node *baseNode;
    Node *goalNode;

};

#endif	/* ASTAR_H */

