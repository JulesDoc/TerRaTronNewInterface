#include <QtWidgets>
#include <QMainWindow>
#include "T_TronParser.hpp"
#include "T_TronValidator.hpp"
#include "T_TronContext.hpp"
#include "T_DBTarget.hpp"
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
void T_TerRaTronNewInterfaceMainWindow::parseValidate()
{
	// Connect to the database

	T_TronDBBundle aDBBundle(T_DBTarget::DEVL, T_DBName::TRS_DB, T_TronContext::ONLINE_VALIDATION);
	T_TronParser parser(aDBBundle);
	T_TronValidator validator(aDBBundle);

	/*PARSING se mueve de aqui*/
	bool ok = m_fileHandler.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ok)
	{
		// Parse
		T_NtcElect rNtcElect;
		parser.parse(&m_fileHandler, rNtcElect);
		// Validate
		ok = validator.validate(rNtcElect);
	}
	/*END PARSING VALIDATION*/
}


void T_TerRaTronNewInterfaceMainWindow::initializeGUI()
{
	m_ui = new Ui_TerRaTronNewInterfaceMainWindow;
	m_ui->setupUi(this);
	setCentralWidget(new T_TerRaTronNewInterfaceWidget(this));
	connect(m_ui->actionOpen_file, SIGNAL(triggered()), SLOT(openFile()));
	m_ui->toolBar->addAction(nvWidget()->actionSaveAndRevalidate());
	m_ui->actionClose_file->setVisible(false);
}

T_TerRaTronNewInterfaceWidget *T_TerRaTronNewInterfaceMainWindow::nvWidget() const
{
	return dynamic_cast<T_TerRaTronNewInterfaceWidget*>(centralWidget());
}

void T_TerRaTronNewInterfaceMainWindow::openFile() 
{
	const QString filter = nvWidget()->isUseXML() ? QString::fromUtf8("(*.xml)") : QString::fromUtf8("(*.txt)");
	m_fileName = QFileDialog::getOpenFileName(this, QString(), QDir::homePath(), filter);
	if (m_fileName.isEmpty()) return;

	m_fileHandler.setFileName(m_fileName);
	if (!m_fileHandler.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QTextStream inputStream(&m_fileHandler);
	QString fileLine = inputStream.readLine();
	QString fileContent;
	while (!fileLine.isNull())
	{
		fileContent.append(fileLine + "\n");
		fileLine = inputStream.readLine();
	}
	nvWidget()->display(fileContent);
	parseValidate();
}
