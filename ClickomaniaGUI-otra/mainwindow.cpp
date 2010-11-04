#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

Board MainWindow::board;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	for (int i = 0; i < 20; i++)
	{
		ui->tableWidget->setColumnWidth(i, 30);
		ui->tableWidget->setRowHeight(i, 30);
		for (int j = 0; j < 20; j++)
		{
			ui->tableWidget->setItem(i, j, new QTableWidgetItem());
		}
	}
	actualizarTabla();
	ui->tableWidget->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
	board.removeGroup(board.getGroupMove(column, row));
	actualizarTabla();
}


void MainWindow::on_tableWidget_cellEntered(int row, int column){
    actualizarTabla();
    std::set<std::pair<int, int> > move = board.getGroupMove(column, row);
    std::set<std::pair<int, int> >::iterator it;
    for (it = move.begin(); it != move.end(); it++){
      ui->tableWidget->item(it->second, it->first)->setBackgroundColor(QColor(0, 0, 0));
    }
}

// Dado un número entero, proporciona un color para Qt.
QColor MainWindow::intToColor(int color)
{
	switch (color)
	{
	case 0:
		return QColor(255, 255, 255);
	case 1:
		return QColor(199, 14, 174);
	case 2:
		return QColor(235, 29, 14);
	case 3:
		return QColor(26, 147, 9);
	case 4:
		return QColor(12, 97, 165);
	case 5:
		return QColor(232, 232, 16);
	case 6:
		return QColor(127, 52, 228);
	case 7:
		return QColor(235, 146, 21);
	case 8:
		return QColor(180, 10, 150);
	case 9:
		return QColor(27, 145, 112);
	default:
	    return QColor(0, 0, 0);
	}
}

// Pasa el tablero (Board) a la tabla de Qt (QtTableWidget).
void MainWindow::actualizarTabla()
{
	for (int i = 0; i < board.getRows(); i++)
	{
		for (int j = 0; j < board.getColumns(); j++)
		{
			QColor color;
			color = intToColor(board.probando(j, i));
			ui->tableWidget->item(i, j)->setBackgroundColor(color);
			ui->tableWidget->item(i, j)->setText(QString("%1").arg(board.probando(j, i)));
			ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter);
		}
	}
	this->setMouseTracking(true);
}
