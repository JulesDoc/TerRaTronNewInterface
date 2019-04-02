#pragma once

#include <QMainWindow>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QListWidget>
#include <QPointer>
#include <QFile>
#include <QDir>

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
	void initializeGUI();

private slots:

private:
	Ui_TerRaTronNewInterfaceMainWindow *m_ui;
	RT_Database m_db;
	T_TerRaTronNewInterfaceWidget *m_widget;
	
	enum {
		TronKeyRole = Qt::UserRole,
		TronDataRole,
		TronSavedDataRole,
		TronChangedRole,
	};
};