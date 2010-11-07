/*
 * File:   NodeInterface.h
 * Author: juen
 *
 * Created on 6 de noviembre de 2010, 20:18
 */

#ifndef NODEINTERFACE_H
#define	NODEINTERFACE_H
#include <list>
#include <utility>

template <class T> class NodeInterface {
public:
    //virtual NodeInterface(){};
    //virtual NodeInterface(const NodeInterface& orig){};

    virtual ~NodeInterface() {
    };
    virtual int heuristic(T* o) const = 0;
    virtual std::list< std::pair<T*, int> > childList() const = 0;
    virtual T & operator=(const T &o) = 0;
    virtual bool operator==(const T &o) const = 0;

    bool operator!=(const T &o) {
        return !this->operator ==(o);
    }
};

#endif	/* NODEINTERFACE_H */

