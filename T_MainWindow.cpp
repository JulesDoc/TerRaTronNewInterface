#include <QStandardPaths>
#include <QTextStream>
#include "ui_mainWindow.h"
#include "T_MainWindow.hpp"



T_MainWindow::T_MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui_MainWindow;
	ui->setupUi(this);
	connect(ui->actionOpen_file, SIGNAL(triggered()), this, SLOT(on_OpenFile()));
	//connect(ui->actionQuit, &QAction::triggered, this, &T_MainWindow::quitApplication);
	connect(ui->actionQuit, &QAction::triggered, this, [=]
	{
		QCoreApplication::quit();
	});
	QIcon const openFilePixMap(":/images/open.png");
	ui->actionOpen_file->setIcon(openFilePixMap);
}
T_MainWindow::~T_MainWindow()
{
	delete ui;
}
	
void T_MainWindow::on_OpenFile()
{
	fileName = QFileDialog::getOpenFileName(this, tr("Open text file"), QDir::homePath(), tr("Text files (*.txt)"));
	if (fileName.isEmpty()) return;

	QFile fileHandler(fileName);
	if (!fileHandler.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QTextStream inputStream(&fileHandler);
	//In case I want read all:QString fileComplete = in.readAll();
	fileLine = inputStream.readLine();
	while(!fileLine.isNull())
	{
		fileContent.append(fileLine + "\n");		
		fileLine = inputStream.readLine();
		//TODO: Process line by line
	}
	ui->fileContentOutput->clear();
	ui->fileContentOutput->insertPlainText(fileContent);

	fileHandler.close();
}

/*void T_MainWindow::on_Quit()
{
	QCoreApplication::quit();
}*/

