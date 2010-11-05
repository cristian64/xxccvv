#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileInfoList>
#include <sstream>
#include "AStar.h"
#include "AStarPathfinder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	board = NULL;
	// Redimensiona las celdas y crea un item para cada una de ellas.
	for (int i = 0; i < 20; i++)
	{
		ui->tableWidget->setColumnWidth(i, 30);
		ui->tableWidget->setRowHeight(i, 30);
		ui->tableWidget->verticalHeaderItem(i)->setSizeHint(QSize(28, 30));
		ui->tableWidget->horizontalHeaderItem(i)->setSizeHint(QSize(30, 28));
		ui->tableWidget->verticalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
		for (int j = 0; j < 20; j++)
		{
			ui->tableWidget->setItem(i, j, new QTableWidgetItem());
                        ui->tableWidget->item(i, j)->setText("1");
                        ui->tableWidget->item(i, j)->setTextColor(QColor(255, 255, 255));
		}
	}

	// Se bloquea la redimensión de las cabeceras.
	ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
        this->ui->tableWidget->setMouseTracking(false);
}

MainWindow::~MainWindow()
{
	// Elimina los items creados.
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			QTableWidgetItem* item = ui->tableWidget->item(i, j);
			ui->tableWidget->removeCellWidget(i, j);
			delete item;
		}
	}

	// Eliminamos la lista de ficheros.
	while (ui->listWidget->item(0) != NULL)
	{
		QListWidgetItem* item = ui->listWidget->item(0);
		ui->listWidget->removeItemWidget(item);
		delete item;
	}
	// Y el tablero si llegó a crearse.
	if (board != NULL)
		delete board;
    delete ui;
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if(ui->tableWidget->item(row, column)->text()=="1"){
      ui->tableWidget->item(row, column)->setBackgroundColor(QColor(0, 0, 0));
      ui->tableWidget->item(row, column)->setText("999");
      //ui->tableWidget->item(row, column)->setTextColor(QColor(0, 0, 0));
      ui->tableWidget->item(row, column)->setTextColor(QColor(255, 255, 255));
  }else{
      ui->tableWidget->item(row, column)->setBackgroundColor(QColor(255, 255, 255));
      ui->tableWidget->item(row, column)->setText("1");
      //ui->tableWidget->item(row, column)->setTextColor(QColor(255, 255, 255));
      ui->tableWidget->item(row, column)->setTextColor(QColor(0, 0, 0));
  }
}

void MainWindow::on_tableWidget_cellEntered(int row, int column){
    this->on_tableWidget_cellClicked(row, column);
}

void MainWindow::on_pushButton_clicked()
{
    AStar<Node> pathfinder;
    Node inicio(0, 0);
    Node fin(19, 19);
    inicio.setMap(this->ui->tableWidget);
    fin.setMap(this->ui->tableWidget);

    pathfinder.setBaseNode(&inicio);
    pathfinder.setGoalNode(&fin);


    cout << pathfinder.run() << endl;
    cout << pathfinder.steps << endl;

}
