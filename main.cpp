#include <QApplication>

#include "T_Configuration.hpp"
#include "T_NtcElect.hpp"
#include "T_DBTarget.hpp"
#include "T_Database.hpp"
#include "T_TerRaTronNewInterfaceMainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	T_Configuration::loadSettings();

	qRegisterMetaType<T_NtcElect>("T_NtcElect");

	T_DBTarget aDBTarget(T_Configuration::getSettingsDBTarget());
	T_Database db(T_Configuration::getSettingsDBName(), aDBTarget);

	T_String text; text << db.getConnectionInfo();

	T_TerRaTronNewInterfaceMainWindow mainWindow(db);
	mainWindow.setWindowTitle(QObject::tr("Connected to %1").arg(text));
	mainWindow.showMaximized();

	return app.exec();
}