#pragma once

#include <QMainWindow>
#include <QPointer>

#include "T_Database.hpp"

class Ui_TerRaTronNewInterfaceMainWindow;
class T_TerRaTronNewInterfaceNoticeTreeView;
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
	void activateActions();

private:
	Ui_TerRaTronNewInterfaceMainWindow *m_ui;
	RT_Database m_db;
	QPointer<T_TerRaTronNewInterfaceWidget> m_widget;
};