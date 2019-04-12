#include <QtWidgets>
#include <QMainWindow>

#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TerRaTronNewInterfaceWidget.hpp"
#include "ui_TerRaTronNewInterfaceWidget.h"
#include "t_xmlhighlighter.hpp"
#include "T_NtcElectHighlighter.hpp"

T_TerRaTronNewInterfaceWidget::T_TerRaTronNewInterfaceWidget(QWidget *parent)
	:QWidget(parent),
	m_dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
{
	m_workerThread = new QThread(this);
	m_worker = new T_TerRaTronNewInterfaceObject(/*this*/); // Orphan object
    m_worker->moveToThread(m_workerThread);

	connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
	connect(m_worker, &T_TerRaTronNewInterfaceObject::resultReady, this, &T_TerRaTronNewInterfaceWidget::showResult);	
	
	m_workerThread->start();

	initializeGUI();
}

T_TerRaTronNewInterfaceWidget::~T_TerRaTronNewInterfaceWidget()
{
	m_workerThread->quit();
	m_workerThread->wait();

	delete m_ui;
}

void T_TerRaTronNewInterfaceWidget::initializeGUI() 
{
	m_ui = new Ui_TerRaTronNewInterfaceWidget;
	m_ui->setupUi(this);
	m_ui->autoValidate_checkBox->setDisabled(true);
	m_ui->save_and_revalidate_button->setDisabled(true);

	connect(m_ui->actionSave_and_revalidate, &QAction::triggered, this, &T_TerRaTronNewInterfaceWidget::saveValidate);
	connect(m_ui->autoValidate_checkBox, &QCheckBox::toggled, this, &T_TerRaTronNewInterfaceWidget::autoValidate);
	connect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), SLOT(handleCursorPositionChanged()));
	
	m_highlighter1 = new T_NtcElectHighlighter(false, m_ui->fileContent_textEdit->document());

}

void T_TerRaTronNewInterfaceWidget::validate()
{
	const QSignalBlocker blocker(this);

	m_ui->sectionContent_textEdit->setPlainText("Validating ...");
	m_fileContent = m_ui->fileContent_textEdit->toPlainText();

	QMetaObject::invokeMethod(m_worker, "validate", Q_ARG(QString, m_fileContent));
}

void T_TerRaTronNewInterfaceWidget::saveValidate()
{
	const QSignalBlocker blocker(this);

	m_ui->sectionContent_textEdit->setPlainText("Saving and validating ...");
	m_fileContent = m_ui->fileContent_textEdit->toPlainText();

	QFile file(m_fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << m_fileContent;
	}
	file.close();

	QMetaObject::invokeMethod(m_worker, "validate", Q_ARG(QString, m_fileContent));
}

void T_TerRaTronNewInterfaceWidget::autoValidate()
{
	if (m_ui->autoValidate_checkBox->isChecked()) {
		connect(m_ui->fileContent_textEdit, &QPlainTextEdit::textChanged, this, &T_TerRaTronNewInterfaceWidget::validate);
		validate();
		return;
	}
	disconnect(m_ui->fileContent_textEdit, &QPlainTextEdit::textChanged, this, &T_TerRaTronNewInterfaceWidget::validate);
}

void T_TerRaTronNewInterfaceWidget::readFile()
{
	m_fileName = QFileDialog::getOpenFileName(this, QString(), m_dir.absolutePath(), tr("All files (*.*);;xml (*.xml);;txt (*.txt)"));

	if (m_fileName.isEmpty()) return;

	m_dir = QFileInfo(m_fileName).absoluteDir().absolutePath();
	QFile fileHandler(m_fileName);
	//m_fileHandler.setFileName(m_fileName);

	if (!fileHandler.open(QIODevice::ReadWrite | QIODevice::Text)) return;

	QTextStream inputStream(&fileHandler);
	QString fileLine = inputStream.readLine();

	//m_ui->fileContent_textEdit->toPlainText();
	while (!fileLine.isNull())
	{
		m_fileContent.append(fileLine);
		fileLine = inputStream.readLine();
		if (!fileLine.isEmpty()) m_fileContent.append("\n");
	}

	m_pathFile = m_dir.absoluteFilePath(m_fileName);
	display(m_fileContent);
	fileHandler.close();

	QMetaObject::invokeMethod(m_worker, "validate", Q_ARG(QString, m_fileContent));

	m_ui->autoValidate_checkBox->setDisabled(false);
	m_ui->save_and_revalidate_button->setDisabled(false);
}

void T_TerRaTronNewInterfaceWidget::showResult(const T_NtcElect& rcNtcElect)
{
	qDebug() << Q_FUNC_INFO;
	
	T_String messages;
	rcNtcElect.getMessagesAsString(messages);
	
	m_ui->sectionContent_textEdit->clear();
	m_ui->errorsWarn_textBrowser->clear();
	
	m_ui->sectionContent_textEdit->insertPlainText(QString("File name: %1 \n").arg(m_pathFile));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of errors: %1 \n").arg(rcNtcElect.getErrorCount()));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of warnings: %1 \n").arg(rcNtcElect.getWarningCount()));
    m_ui->errorsWarn_textBrowser->insertPlainText(messages);
	m_ui->ntcElectTreeWidget->updateTreeView(m_fileName, rcNtcElect);
}

void T_TerRaTronNewInterfaceWidget::handleCursorPositionChanged()
{
	QTextCursor cursor = m_ui->fileContent_textEdit->textCursor();
	const int line = cursor.blockNumber() + 1;
	const int col = cursor.columnNumber();

	m_ui->fileContent_lineNumber->setText(tr("Ln %1").arg(line));
	m_ui->fileContent_colNumber->setText(tr("Col %1").arg(col));
}


void T_TerRaTronNewInterfaceWidget::display(const QString &fileContent)
{
	m_ui->fileContent_textEdit->clear();
	m_ui->fileContent_textEdit->insertPlainText(fileContent);
}