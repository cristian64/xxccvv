#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "Board.h"

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	static Board board;

private:
    Ui::MainWindow *ui;
	static QColor intToColor(int color);
	void actualizarTabla();

private slots:
	void on_tableWidget_cellClicked(int row, int column);
	void on_tableWidget_cellEntered(int row, int column);
};

#endif // MAINWINDOW_H
