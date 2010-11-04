#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		MainWindow::board = Board(argv[1]);
	}
	else
	{
		std::cerr << "ERROR: No se ha especificado un tablero de entrada" << std::endl;
		return -1;
	}

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
