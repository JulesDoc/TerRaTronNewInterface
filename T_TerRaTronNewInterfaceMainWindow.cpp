
#include "T_TerRaTronNewInterfaceMainWindow.hpp"
#include "T_TerRaTronNewInterfaceWidget.hpp"
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

	/*Widget settings*/
	m_widget = new T_TerRaTronNewInterfaceWidget(this);
	setCentralWidget(m_widget);
	connect(m_ui->actionOpen_file, &QAction::triggered, m_widget, &T_TerRaTronNewInterfaceWidget::readFile);
	
	m_ui->actionClose_file->setVisible(false);
}