#include <QApplication>
#include "T_TronContext.hpp"
#include "T_TerRaTronNewInterfaceMainWindow.hpp"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	T_DBTarget aDBTarget(T_DBTarget::DEVL);
	T_Database db(T_DBName::TRS_DB, aDBTarget);
	T_String text;
	text << db.getConnectionInfo();

	T_TerRaTronNewInterfaceMainWindow mainWindow(db);
	mainWindow.setWindowTitle(QObject::tr("Connected to %1").arg(text));
	mainWindow.showMaximized();

	return app.exec();
}