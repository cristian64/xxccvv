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
        /*
        bool operator==(Node &o){
            cout << "MIERDA HOSTIA YA\n";
            return false;
        }*/
    };

    class NodeCompare {
    public:

        bool operator() (const Node *x, const Node*y) const {
            return x->f > y->f;
        }
    };

public:

    AStar() {
        currentSolution = 0;
        baseNode = new Node;
        goalNode = new Node;
        steps = 0;
    }
    virtual ~AStar(){
        typename vector<Node*>::iterator it;//it sobre cerrados
        for (it = this->open.begin(); it != this->open.end(); it++) {
            delete *(it);
        }
        for (it = this->closed.begin(); it != this->closed.end(); it++) {
            delete *(it);
        }
        for (it = this->garbage.begin(); it != this->garbage.end(); it++) {
            //delete *(it);
        }
    }

    int step() {
        if (this->open.empty()) {
            return -1; //ESPACIO EXPLORADO SIN EXITO;
        }
        this->steps++;

        Node *currentNode = this->open.front(); //nodo actual
        pop_heap(this->open.begin(), this->open.end(), NodeCompare());
        this->open.pop_back(); //pop_heap se lleva el primero al final

        //cout << "comparando fin de busqueda\n";

        if (*(currentNode->data) == *(this->goalNode->data)) {//hacer esto bien, la comparacion sera mejorable?
            this->goalNode->parent = currentNode->parent;
            Node *parentNode = this->goalNode->parent;
            Node *childNode = this->goalNode;

            do {
                //cout <<"SOLUCION " <<  childNode->data->x << ',' << childNode->data->y << " "<< childNode->g <<  endl;
                parentNode->child = childNode;
                childNode = parentNode;
                parentNode = parentNode->parent;
            } while (this->baseNode != childNode);

            this->currentSolution = this->baseNode;
            cout <<"G DEL CAMINO" <<  currentNode->g << endl;

            typename vector<Node*>::iterator closed_node;//it sobre cerrados
            for (closed_node = this->closed.begin(); closed_node != this->closed.end(); closed_node++) {
                if ((*closed_node)->data == this->baseNode->data){
                       this->closed.erase(closed_node);
                       break;
                }
            }

            //this->closed.push_back(currentNode);
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
                //garbage.push_back(aux);
                aux->data = childObject->first;//iguala punteros
                childNodes.push_back(pair<Node*, int>(aux, childObject->second));

                //cout << (childObject->first)->x << ' ' << childObject->second << endl;
            }

            //para cada hijo
            for (childNode = childNodes.begin(); childNode != childNodes.end(); childNode++) {
                float childG = currentNode->g + childNode->second;
                //cout <<"G: " << childG << endl;
                //se busca en abiertos
                for (open_node = this->open.begin(); open_node != this->open.end(); open_node++) {
                    //cout << "buscando en abiertos\n";
                    if (*((*open_node)->data) == *(childNode->first->data)) {
                        //cout << "ESTOY EN ABIERTOS" << endl;

                        break;
                    }
                }
                //<--------- ya se optimizara, si esta en abiertos no deberia estar en cerrados
                //se busca en cerrados
                for (closed_node = this->closed.begin(); closed_node != this->closed.end(); closed_node++) {
                    /*
                    cout << "Soy: " << currentNode->data->x << ' ' << currentNode->data->y << endl;
                    cout << "buscando en cerrados a mi hijo:\n";
                    cout << (childNode->first->data)->x << endl;
                    cout << (childNode->first->data)->y << endl;
                    cout << "Y lo estoy comparando con:\n";
                    cout << ((*closed_node)->data)->x << endl;
                    cout << ((*closed_node)->data)->y << endl;
                    */

                    if (*((*closed_node)->data) == *(childNode->first->data)) {
                        //cout << "ESTOY EN CERRADOS"<< endl;
                        break;
                    }
                }

                childNode->first->g = childG;
                //childNode->first->h = childNode->first->data->ESTIMACION_HASTA_OBJETIVO(this->goalNode); //<------------------
                childNode->first->f = childNode->first->g + childNode->first->h;
                childNode->first->parent = currentNode;

                if (open_node != open.end()) {//si se encontro en abiertos
                    if ((*open_node)->g <= childG) {//es peor que lo que tenemos
                        delete childNode->first;//olvida el nuevo
                       // (*childNode) = open_node;
                    }else{//es mejor que lo que tenemos
                        //garbage.push_back(*open_node);
                        open.erase(open_node);//se saca el viejo, aqui seguramente se pierde un nodo
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
                        //garbage.push_back(*closed_node);
                        closed.erase(closed_node);//se saca de cerrados, aqui seguramente se pierde un nodo
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
            //cout << "NODOS EN CLOSED: " << this->closed.size() << endl;
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
        //this->baseNode->data = new T(*node); <-asi casca, a saber por que
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
        //this->goalNode->data = new T(*node); //<-idem del anterior
        this->goalNode->data =node;
        this->goalNode->parent = 0;
        this->goalNode->child = 0;
        this->goalNode->g = 0;
        this->goalNode->h = 0; //<---------------algo hay que estimar?, funcionCota()?
        this->goalNode->f = this->goalNode->g + this->goalNode->h;
    }

    void showSolution(){
        Node *currentNode = currentSolution;
        while (currentNode != goalNode){
            cout << currentNode->data->x << " " << currentNode->data->y << endl;
            currentNode = currentNode->child;
        }
        cout << currentNode->data->x << " " << currentNode->data->y << endl;
    }

    list<T*> solution(){
        list<T*> solution;
        Node* currentNode = this->baseNode;
        while(currentNode != this->goalNode){
            solution.push_back(currentNode->data);
            currentNode = currentNode->child;
        }
        solution.push_back(currentNode->data);//no

        return solution;
    }

    int steps;
private:
    vector<Node*> open;
    vector<Node*> closed;
    vector<Node*> garbage;
    Node *currentSolution;
    Node *baseNode;
    Node *goalNode;

};

#endif	/* ASTAR_H */

