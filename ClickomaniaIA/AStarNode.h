/* 
 * File:   AStarNode.h
 * Author: juen
 *
 * Created on 4 de noviembre de 2010, 14:46
 */

#ifndef ASTARNODE_H
#define	ASTARNODE_H
#include <list>

template<typename T> class AStarNode {
public:
    AStarNode();
    AStarNode(const AStarNode& orig);
    virtual ~AStarNode();

    AStarNode<T>* GetBestParent() const;

    void SetBestParent(AStarNode* bestParent);

    std::list<AStarNode<T>*>* GetChildNodes() const;

    void SetChildNodes(std::list<AStarNode<T>*>* childNodes);

    float GetF() const;

    float GetG() const;

    void SetG(float g);

    float GetH() const;

    void SetH(float h);

    T* getData() const;
    void setData(T * data);

    std::list<AStarNode*> getSuccesors();

    class NodeCompare {
    public:

        bool operator() (const AStarNode<T> *x, const AStarNode<T> *y) const {
            return x->GetF() < y->GetF();
        }
    };

private:
    T *data;
    AStarNode *bestParent;
    std::list<AStarNode<T> *> * childNodes;
    float g;
    float h;
    float f;

};

#endif	/* ASTARNODE_H */

