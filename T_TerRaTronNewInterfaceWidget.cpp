#include <QtWidgets>
#include <QMainWindow>
#include "T_TerRaTronNewInterfaceWidget.hpp"
#include "ui_TerRaTronNewInterfaceWidget.h"

T_TerRaTronNewInterfaceWidget::T_TerRaTronNewInterfaceWidget(QWidget *parent): 
	QWidget(parent)
{
	initializeGUI();
}

T_TerRaTronNewInterfaceWidget::~T_TerRaTronNewInterfaceWidget()
{
	delete m_ui;
}

void T_TerRaTronNewInterfaceWidget::initializeGUI() 
{
	m_ui = new Ui_TerRaTronNewInterfaceWidget;
	m_ui->setupUi(this);
}

bool T_TerRaTronNewInterfaceWidget::isUseXML() const
{
	return m_bUseXML;
}

void T_TerRaTronNewInterfaceWidget::display(const QString &fileContent)
{
	m_ui->fileContent_textEdit->clear();
	m_ui->fileContent_textEdit->insertPlainText(fileContent);
}

QAction *T_TerRaTronNewInterfaceWidget::actionSaveAndRevalidate() const
{
	return m_ui->actionSave_and_revalidate;
}

QAction *T_TerRaTronNewInterfaceWidget::actionViewFile() const
{
	return m_ui->actionView_file;
}

QAction *T_TerRaTronNewInterfaceWidget::actionOpenContainingFolder() const
{
	return m_ui->actionOpen_containing_folder;
}