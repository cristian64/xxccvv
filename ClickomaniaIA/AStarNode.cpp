/* 
 * File:   AStarNode.cpp
 * Author: juen
 * 
 * Created on 4 de noviembre de 2010, 14:46
 */

#include "AStarNode.h"
#include <vector>

template <typename T>
AStarNode<T>::AStarNode() {
}

template <typename T>
AStarNode<T>::AStarNode(const AStarNode& orig) {
}

template <typename T>
AStarNode<T>::~AStarNode() {
    typename std::list<AStarNode<T>* >::iterator it;
    for (it = this->childNodes.begin(); it != this->childNodes.end(); it++) {
        delete *it;
    }
    delete this->childNodes;
}

template <typename T>
float AStarNode<T>::GetF() const {
    return f;
}

template <typename T>
float AStarNode<T>::GetG() const {
    return g;
}

template <typename T>
void AStarNode<T>::SetG(float g) {
    this->g = g;
    this->f = this->g + this->h;
}

template <typename T>
float AStarNode<T>::GetH() const {
    return h;
}

template <typename T>
void AStarNode<T>::SetH(float h) {
    this->h = h;
    this->f = this->g + this->h;
}

template <typename T>
AStarNode<T>* AStarNode<T>::GetBestParent() const {
    return bestParent;
}

template <typename T>
void AStarNode<T>::SetBestParent(AStarNode* bestParent) {
    this->bestParent = bestParent;
}

template <typename T>
std::list<AStarNode<T>*>*AStarNode<T>::GetChildNodes() const {
    return childNodes;
}

template <typename T>
void AStarNode<T>::SetChildNodes(std::list<AStarNode*>* childNodes) {
    this->childNodes = childNodes;
}

template <typename T>
std::list<AStarNode*> AStarNode<T>::getSuccesors() {
    return this->data->getSuccesors();

}

template <typename T>
T* AStarNode<T>::getData() const {
    return this->data;
}

template <typename T>
void AStarNode<T>::setData(T* data) {
    this->data = data;
}