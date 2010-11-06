/* 
 * File:   AStarPathfinder.h
 * Author: juen
 *
 * Created on 5 de noviembre de 2010, 15:06
 */

#ifndef ASTARPATHFINDER_H
#define	ASTARPATHFINDER_H
#include <list>
#include <utility>
 #include <QtGui/QTableWidget>
using namespace std;

class Node {
public:
    Node();
    Node(int x, int y);
    Node(const Node& orig);
    ~Node();
    void setMap(QTableWidget *map);
    list<pair <Node*,int> > childList();
    bool operator==(Node &o);
     Node& operator=(Node &o);

    int x;
    int y;
private:
    QTableWidget *map;

};

#endif	/* ASTARPATHFINDER_H */

