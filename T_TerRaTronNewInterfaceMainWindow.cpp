
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
	connect(m_widget, &T_TerRaTronNewInterfaceWidget::readFileCompleted, this, &T_TerRaTronNewInterfaceMainWindow::activateActions);
	connect(m_ui->actionValidate, &QAction::triggered, m_widget, &T_TerRaTronNewInterfaceWidget::validate);
	connect(m_ui->actionSave, &QAction::triggered, m_widget, &T_TerRaTronNewInterfaceWidget::save);
	connect(m_ui->actionSave_validate, &QAction::triggered, m_widget, &T_TerRaTronNewInterfaceWidget::saveValidate);
}

void T_TerRaTronNewInterfaceMainWindow::activateActions()
{
	m_ui->actionClose_file->setEnabled(true);
	m_ui->actionSave->setEnabled(true);
	m_ui->actionValidate->setEnabled(true);
	m_ui->actionSave_validate->setEnabled(true);
}