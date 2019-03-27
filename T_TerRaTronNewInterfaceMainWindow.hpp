#pragma once

#include <QMainWindow>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QListWidget>
#include <QPointer>
#include <QFile>
#include "T_Database.hpp"
#include "T_TronParser.hpp"
#include "T_TronValidator.hpp"


class Ui_TerRaTronNewInterfaceMainWindow;
class T_TerRaTronNewInterfaceWidget;

class T_TerRaTronNewInterfaceMainWindow : public QMainWindow
{
	Q_OBJECT
public:

	T_TerRaTronNewInterfaceMainWindow(RT_Database rDatabase, QWidget *parent = 0);
	virtual ~T_TerRaTronNewInterfaceMainWindow();

private:
	void parseValidate();
	void initializeGUI();
	T_TerRaTronNewInterfaceWidget *nvWidget() const;

private slots:
	void openFile();

private:
	Ui_TerRaTronNewInterfaceMainWindow *m_ui;
	RT_Database m_db;
	QString m_fileName{};
	QFile m_fileHandler;
	
	
	enum {
		TronKeyRole = Qt::UserRole,
		TronDataRole,
		TronSavedDataRole,
		TronChangedRole,
	};
};