#include <QStandardPaths>
#include <QTextStream>
#include "ui_mainWindow.h"
#include "T_MainWindow.hpp"
#include "T_TronSection.hpp"
#include "T_TronDBBundle.hpp"
#include "T_TronParser.hpp"
#include "T_TronContext.hpp"
#include "T_TronValidator.hpp"
#include "T_DBTarget.hpp"
#include "T_NtcElect.hpp"

#pragma once

#include "T_NoticesWidgetDef.h"
#include "T_ParseElect.hpp"
#include "T_BaseDBConnectionCreator.hpp"
#include "T_TerRaRefDBPool.hpp"
#include "T_BaseQtProgram.hpp"

class Ui_TerRaNVMainWindow;
class T_TerRaNVWidget;

class T_NOTICESWIDGETDEF_CLASS T_TerRaNVMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	T_TerRaNVMainWindow(QWidget *parent = 0);
	virtual ~T_TerRaNVMainWindow();

	void initialize(RCT_BaseDBConnectionCreator rcDBConnectionCreator, RT_IDWM rIDWM); // Has to be called after the creation of the object
	RT_ParseElect getParseElect() const;

	inline void setWorkingDirectory(const QDir &dir) { m_dir = dir; }
	inline const QDir &getWorkingDirectory() const { return m_dir; }

private:
	void initializeGUI();
	T_TerRaNVWidget *nvWidget() const;
	void invoke_checkNtcElect(const QString &fileName);

private slots:
	void openFile();
	void closeFile();
	void viewFile();
	void editFile();
	void validate();
	void setUseXML(bool b);
	void setDrawLineNumberInErrorWarningMessage(bool b);
	void showAboutMessage();
	void enableDisableButtons();

	// Attributes
	// **********
private:
	Ui_TerRaNVMainWindow *m_ui;
	QDir m_dir;

	// databases
	// *********
	APT_BaseDBConnectionCreator m_apDBConnectionCreator;
	// Terrestrial database
	APT_TerRaBase m_apTerRaBase;
	// Reference database
	T_TerRaRef::APT_BaseRefBase m_apBaseRefBase; // T_PublicRefBase or T_MainRefBase
	T_TerRaRef::APT_BaseRefBaseCacheManager m_apBaseRefBaseCacheManager;
	APT_TerRaRefDBPool m_apTerRaRefDBPool;
	// GLAD databases
	T_GLAD::APT_GLADBase m_apGLADBase;
	// Parser
	APT_ParseElect m_apParseElect;
};

// Private class T_TerRaNVQtProgram
// ********************************

class T_TerRaNVQtProgram : public T_BaseQtProgram
{
public:
	T_TerRaNVQtProgram() {}
	virtual T_DOUBLE getVersionNumber() const { return 1.0; }
	T_String getModuleName() const { return TEXT("TerRaNV"); }
};
























/*T_MainWindow::T_MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui_MainWindow;
	ui->setupUi(this);
	connect(ui->actionOpen_file, SIGNAL(triggered()), this, SLOT(on_OpenFile()));
	//connect(ui->actionQuit, &QAction::triggered, this, &T_MainWindow::quitApplication);
	connect(ui->actionQuit, &QAction::triggered, this, [=]
	{
		QCoreApplication::quit();
	});
	QIcon const openFilePixMap(":/images/open.png");
	ui->actionOpen_file->setIcon(openFilePixMap);
}
T_MainWindow::~T_MainWindow()
{
	delete ui;
}
	
void T_MainWindow::on_OpenFile()
{
	fileName = QFileDialog::getOpenFileName(this, tr("Open text file"), QDir::homePath(), tr("Text files (*.txt)"));
	if (fileName.isEmpty()) return;

	QFile fileHandler(fileName);
	if (!fileHandler.open(QIODevice::ReadOnly | QIODevice::Text)) return;

	QTextStream inputStream(&fileHandler);
	//In case I want read all:QString fileComplete = in.readAll();
	fileLine = inputStream.readLine();
	while(!fileLine.isNull())
	{
		fileContent.append(fileLine + "\n");		
		fileLine = inputStream.readLine();
		//TODO: Process line by line
	}
	ui->fileContentOutput->clear();
	ui->fileContentOutput->insertPlainText(fileContent);

	// Connect to the database
	T_TronDBBundle aDBBundle(T_DBTarget::DEVL, T_DBName::TRS_DB, T_TronContext::ONLINE_VALIDATION);
	T_TronParser parser(aDBBundle);
	T_TronValidator validator(aDBBundle);

	QFile file("fileName");
	bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ok)
	{
		// Parse
		T_NtcElect rNtcElect;
		parser.parse(&file, rNtcElect);
		// Validate
		ok = validator.validate(rNtcElect);
	}

	fileHandler.close();
}

/*void T_MainWindow::on_Quit()
{
	QCoreApplication::quit();
}*/

