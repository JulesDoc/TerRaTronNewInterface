#include "T_MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	T_MainWindow w;
	w.show();
	return a.exec();
}
