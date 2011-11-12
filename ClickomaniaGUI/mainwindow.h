#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <iostream>
#include "Board.h"
#include "AStar.h"

using namespace std;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	Board *board;

private:
	Ui::MainWindow *ui;
	static QColor intToColor(int color);
	void actualizarTabla();

private slots:
	void on_pushButton_7_clicked();
	void on_pushButton_21_clicked();
	void on_pushButton_20_clicked();
	void on_pushButton_3_clicked();
	void on_tableWidget_cellEntered(int row, int column);
	void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
	void on_tableWidget_cellClicked(int row, int column);
};

#endif // MAINWINDOW_H
