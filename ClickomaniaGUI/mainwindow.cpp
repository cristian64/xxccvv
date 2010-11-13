#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AEstrella.h"
#include <QDir>
#include <QFileInfoList>
#include <QFileDialog>
#include <QTextStream>
#include <QTime>

#include <sstream>

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
		}
	}

	// Se bloquea la redimensión de las cabeceras.
	ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	// Cargamos los tableros.
	on_pushButton_clicked();
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

// Ejecuta una jugada en el tablero.
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
	if (board != NULL)
	{
		if (row < board->getRows() && column < board->getColumns())
		{
			// Se obtiene el grupo de la casilla indicada y se elimina.
			set<pair<int,int> > grupo = board->getGroupMove(column, row);
			board->removeGroup(grupo);
			actualizarTabla();

			// Se actualiza la puntuación.
			int ultimaPuntuacion = Board::score(grupo);
			int puntuacion = ui->label_4->text().toInt() + ultimaPuntuacion;
			ui->label_4->setText(QString::number(puntuacion));
			ui->label_5->setText(QString::number(ultimaPuntuacion));
		}
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
	// Se limpia el tablero.
	for(int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			QColor color = intToColor(0);
			ui->tableWidget->item(i, j)->setBackgroundColor(color);
		}
	}

	// Se carga el actual.
	for (int i = 0; i < board->getRows(); i++)
	{
		for (int j = 0; j < board->getColumns(); j++)
		{
			QColor color = intToColor(board->getPosition(j, i));
			ui->tableWidget->item(i, j)->setBackgroundColor(color);
		}
	}
}

// Recarga los ficheros de texto en la lista.
void MainWindow::on_pushButton_clicked()
{
	// Eliminamos la lista anterior.
	while (ui->listWidget->item(0) != NULL)
	{
		QListWidgetItem* item = ui->listWidget->item(0);
		ui->listWidget->removeItemWidget(item);
		delete item;
	}

	// Cargamos nuevamente la lista.
	QDir dir(".");
	QFileInfoList files = dir.entryInfoList();
	for (int i = 0; i < files.size(); ++i)
	{
		QFileInfo file = files.at(i);
		if (file.fileName().toLower().contains("tablero"))
			ui->listWidget->addItem(new QListWidgetItem(file.fileName()));
	}
}

// Aplica el siguiente movimiento de la secuencia.
void MainWindow::on_pushButton_2_clicked()
{
	QString secuencia = ui->lineEdit->text();

	// Se comprueba que hay al menos un "(x, y)" antes de leer.
	if (secuencia.contains(QRegExp("^(\\s*\\D\\s*\\d+\\s*\\D\\s*\\d+\\s*\\D\\s*)")))
	{
		// Se extrae el primer movimiento de la secuencia y se aplica.
		stringstream flujo(secuencia.toStdString());
		char caracter;
		int x, y;
		flujo >> caracter;
		flujo >> x;
		flujo >> caracter;
		flujo >> y;
		flujo >> caracter;
		on_tableWidget_cellClicked(y, x);

		// El resto del flujo se introduce en el campo de texto otra vez.
		string resto = "";
		flujo.read(&caracter, 1);
		while (!flujo.eof())
		{
			resto = resto + caracter;
			flujo.read(&caracter, 1);
		}
		ui->lineEdit->setText(QString::fromStdString(resto));
	}
}

// Carga el tablero seleccionado.
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
	// Se elimina el tablero si estaba cargado y se carga el nuevo.
	if (board != NULL)
		delete board;
	board = new Board(item->text().toStdString());
	actualizarTabla();

	// Se reinicia la puntuación.
	ui->label_4->setText("0");
	ui->label_5->setText("0");
}

// Resalta el grupo de celdas al pasar por encima con el ratón.
void MainWindow::on_tableWidget_cellEntered(int row, int column)
{
	if (board != NULL)
	{
		actualizarTabla();
		if (row < board->getRows() && column < board->getColumns())
		{
			std::set<std::pair<int, int> > move = board->getGroupMove(column, row);
			std::set<std::pair<int, int> >::iterator it;
			for (it = move.begin(); it != move.end(); it++)
			{
				ui->tableWidget->item(it->second, it->first)->setBackgroundColor(QColor(0, 0, 0));
			}
		}
	}
}

// Ejecuta el algoritmo AEstrella sobre el tablero Board y muestra los resultados por pantalla.
void MainWindow::on_pushButton_3_clicked()
{	
	QTime tiempo;
	tiempo.start();

	AEstrella aestrella;
	pair<int, list<pair<int, int> > > resultado = aestrella.run(*board);

	QString tiempoStr = QString::number((tiempo.elapsed() / 1000) / 60) + "m " + QString::number((tiempo.elapsed() / 1000) % 60) + "s " + QString::number(tiempo.elapsed() % 1000) + "ms";
	ui->label_9->setText(tiempoStr);

	QString flujo;
	for (list<pair<int, int> >::iterator i = resultado.second.begin(); i != resultado.second.end(); i++)
	{
		flujo += QString::fromStdString("(") + QString::number((*i).first) + QString::fromStdString(", ") + QString::number((*i).second) + QString::fromStdString(") ");
	}
	flujo += QString::number(resultado.first);
	ui->lineEdit->setText(flujo);

	cout << ui->listWidget->selectedItems().at(0)->text().toStdString() << endl;
	cout << flujo.toStdString() << endl;
	cout << tiempoStr.toStdString() << endl;
	cout << "-----------------------------------------------------------" << endl;
}

// Se limpia el tablero.
void MainWindow::on_pushButton_20_clicked()
{
	// Se limpia el tablero.
	for(int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			QColor color = intToColor(0);
			ui->tableWidget->item(i, j)->setBackgroundColor(color);
		}
	}

	// Se reinicia la puntuación.
	ui->label_4->setText("0");
	ui->label_5->setText("0");
}

// Guarda el tablero.
void MainWindow::on_pushButton_21_clicked()
{
	if (board != NULL)
	{
		QString nombre = QFileDialog::getSaveFileName(0, "Guardando el tablero...", QString(), "*.txt");
		if (nombre.length() > 0)
		{
			QFile fichero(nombre);
			if (fichero.open(QFile::WriteOnly))
			{
				QTextStream flujo(&fichero);
				flujo << QString::fromStdString(board->toString()) << endl; //TODO, guardar el tablero y no una mierdaca...
				fichero.close();
			}
		}
	}
}

// Genera un tablero aleatorio.
void MainWindow::on_pushButton_7_clicked()
{
	if (board != NULL)
		delete board;
	board = new Board(ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value());
	actualizarTabla();

	// Se reinicia la puntuación.
	ui->label_4->setText("0");
	ui->label_5->setText("0");
}
