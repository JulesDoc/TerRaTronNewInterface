#pragma once

#include <QMainWindow>
#include <QFileDialog.h>


class Ui_MainWindow;

class T_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	T_MainWindow(QWidget *parent = Q_NULLPTR);
	~T_MainWindow();

private:
	Ui_MainWindow * ui;
	QString currentDirectory;
	QString fileName;
	QString fileContent;
	QString fileLine;

private slots:
	void on_OpenFile();
	//void on_Quit();
};
