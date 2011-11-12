#include <QtGui/QApplication>
#include <iostream>
#include <QTime>

#include "mainwindow.h"
#include "AStar.h"
#include "Board.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc > 1) {
		QTime tiempo;
		tiempo.start();
		pair<int, list<pair<int, int> > > resultado = AStar::run(Board(argv[1]));

		QString tiempoStr = QString::number((tiempo.elapsed() / 1000) / 60) + "m " + QString::number((tiempo.elapsed() / 1000) % 60) + "s " + QString::number(tiempo.elapsed() % 1000) + "ms";

		QString flujo;
		for (list<pair<int, int> >::iterator i = resultado.second.begin(); i != resultado.second.end(); i++) {
			flujo += QString::fromStdString("(") + QString::number((*i).first) + QString::fromStdString(", ") + QString::number((*i).second) + QString::fromStdString(") ");
		}
		flujo += QString::number(resultado.first);

		cout << flujo.toStdString() << endl;
		cout << tiempoStr.toStdString() << endl;
		cout << "-----------------------------------------------------------" << endl;
	} else {
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		return a.exec();
	}
}
