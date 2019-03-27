#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QListWidget>
#include <QPointer>
#include "T_Database.hpp"
#include "T_TronDBBundle.hpp"

class Ui_TerRaTronNewInterfaceWidget;

class T_TerRaTronNewInterfaceWidget : public QWidget
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceWidget(QWidget *parent = 0);
	virtual ~T_TerRaTronNewInterfaceWidget();
	void display(const QString&);

	bool isUseXML() const;

	QAction *actionSaveAndRevalidate() const;
	QAction *actionViewFile() const;
	QAction *actionOpenContainingFolder() const;

private:
	void initializeGUI();
	

private slots:

private:
	Ui_TerRaTronNewInterfaceWidget *m_ui;
	bool m_bUseXML;


	
};
