#include <QtWidgets>
#include <QMainWindow>
#include <iostream> 
#include "T_TronParser.hpp"
#include "T_TronValidator.hpp"
#include "T_TronContext.hpp"
#include "T_DBTarget.hpp"
#include "T_TerRaTronNewInterfaceMainWindow.hpp"
#include "T_TerRaTronNewInterfaceWidget.hpp"
#include "T_TerRaTronNewInterfaceObject.hpp"
#include "ui_TerRaTronNewInterfaceMainWindow.h"
#include "ui_TerRaTronNewInterfaceWidget.h"

T_TerRaTronNewInterfaceMainWindow::T_TerRaTronNewInterfaceMainWindow(RT_Database rDatabase, QWidget *parent)
	:QMainWindow(parent), m_db(rDatabase)
{
	initializeGUI();
}

T_TerRaTronNewInterfaceMainWindow::~T_TerRaTronNewInterfaceMainWindow() 
{
	delete m_ui;
}

void T_TerRaTronNewInterfaceMainWindow::initializeGUI()
{
	m_ui = new Ui_TerRaTronNewInterfaceMainWindow;
	m_ui->setupUi(this);

	m_widget = new T_TerRaTronNewInterfaceWidget(this);
	setCentralWidget(m_widget);
	
	m_ui->actionClose_file->setVisible(false);
	connect(m_ui->actionOpen_file, &QAction::triggered, m_widget, &T_TerRaTronNewInterfaceWidget::readFile);
}