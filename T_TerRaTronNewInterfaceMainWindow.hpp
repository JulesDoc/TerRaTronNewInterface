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
	T_TerRaTronNewInterfaceMainWindow(RT_Database rDatabase, QWidget *parent = nullptr);
	virtual ~T_TerRaTronNewInterfaceMainWindow();

private:
	void initializeGUI();
	
public slots:
	/*Enable and disable StatusBar icons and options in Options*/
	/*If no file open -> all disable*/
	void activateActions();
	void deActivateActions();
	void close();
	void help();
	void closeEvent(QCloseEvent*);

private:
	Ui_TerRaTronNewInterfaceMainWindow *m_ui;
	RT_Database m_db;
	QPointer<T_TerRaTronNewInterfaceWidget> m_widget;
};