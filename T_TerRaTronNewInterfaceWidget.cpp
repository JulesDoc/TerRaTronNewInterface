
#include <sstream> 

#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TerRaTronNewInterfaceWidget.hpp"
#include "ui_TerRaTronNewInterfaceWidget.h"
#include "t_xmlhighlighter.hpp"
#include "T_NtcElectHighlighter.hpp"
#include "codeeditor.h"
#include "T_Message.hpp"

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
	m_ui->labelTextWarning->setVisible(false);

	connect(m_ui->actionSave_and_revalidate, &QAction::triggered, this, &T_TerRaTronNewInterfaceWidget::saveValidate);
	connect(m_ui->autoValidate_checkBox, &QCheckBox::toggled, this, &T_TerRaTronNewInterfaceWidget::autoValidate);
	connect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), SLOT(handleCursorPositionChanged()));
	connect(m_ui->errorsWarn_textBrowser, SIGNAL(cursorPositionChanged()), SLOT(showColoredErrorLines()));

	m_highlighter1 = new T_NtcElectHighlighter(false, m_ui->fileContent_textEdit->document());
}

void T_TerRaTronNewInterfaceWidget::validate()
{
	const QSignalBlocker blocker(this);
	QMetaObject::invokeMethod(m_worker, "validate", Q_ARG(QString, m_ui->fileContent_textEdit->toPlainText()));
}

void T_TerRaTronNewInterfaceWidget::save()
{
	m_ui->labelTextWarning->setVisible(false);

	QFile file(m_fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << m_ui->fileContent_textEdit->toPlainText();
	}
	file.close();
}

void T_TerRaTronNewInterfaceWidget::saveValidate()
{
	save();
	validate();
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

	if (!fileHandler.open(QIODevice::ReadWrite | QIODevice::Text)) return;

	QTextStream inputStream(&fileHandler);
	QString fileLine = inputStream.readLine();

	while (!fileLine.isNull())
	{
		m_ui->fileContent_textEdit->appendPlainText(fileLine);
		fileLine = inputStream.readLine();
	}
	m_ui->labelTextWarning->setVisible(false);
	m_pathFile = m_dir.absoluteFilePath(m_fileName);
	fileHandler.close();

	validate();

	Q_EMIT(readFileCompleted());
	m_ui->autoValidate_checkBox->setDisabled(false);
	m_ui->save_and_revalidate_button->setDisabled(false);
}

void T_TerRaTronNewInterfaceWidget::showResult(const T_NtcElect& rcNtcElect)
{
	qDebug() << Q_FUNC_INFO;
	setNtcElect(rcNtcElect);
	
	T_String messages;
	rcNtcElect.getMessagesAsString(messages);

	m_ui->sectionContent_textEdit->clear();
	m_ui->errorsWarn_textBrowser->clear();
	
	m_ui->sectionContent_textEdit->insertPlainText(QString("File name: %1 \n").arg(m_pathFile));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of errors: %1 \n").arg(rcNtcElect.getErrorCount()));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of warnings: %1 \n").arg(rcNtcElect.getWarningCount()));
    m_ui->errorsWarn_textBrowser->insertPlainText(messages);
	m_ui->ntcElectTreeWidget->updateTreeView(m_fileName, rcNtcElect);
	extractLineNumber();
	showColoredErrorLines();
}

void T_TerRaTronNewInterfaceWidget::handleCursorPositionChanged()
{

	QTextCursor cursor = m_ui->fileContent_textEdit->textCursor();

	const int line = cursor.blockNumber() + 1;
	const int col = cursor.columnNumber();

	m_ui->fileContent_lineNumber->setText(tr("Ln %1").arg(line));
	m_ui->fileContent_colNumber->setText(tr("Col %1").arg(col));
}

void T_TerRaTronNewInterfaceWidget::showColoredErrorLines()
{
	moveToFirstErrorLine();

	if (m_NtcElect.getErrorCount() != 0 || m_NtcElect.getWarningCount() != 0) 
	{
		QList<QTextEdit::ExtraSelection> extraSelections;
		QTextEdit::ExtraSelection selection;
		QColor lineColor = QColor(Qt::red).lighter(160);
		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	
		for (int line : m_vecErrorsLineNumbers) 
		{
			QTextCursor cursor(m_ui->fileContent_textEdit->document()->findBlockByLineNumber(line-1));
			selection.cursor = cursor;
			selection.cursor.clearSelection();
			extraSelections.append(selection);
			m_ui->fileContent_textEdit->setExtraSelections(extraSelections);
		}
	}
}

void T_TerRaTronNewInterfaceWidget::extractLineNumber()
{
	T_String messages;
	m_NtcElect.getMessagesAsString(messages);
	std::string str = messages.toStdString();
	std::stringstream message;

	message << str;
	m_vecErrorsLineNumbers.clear();
	std::string temp;
	int line;
	while (!message.eof()) {
		message >> temp;
		if (temp == "Notice") {
			continue;
		}
		if (temp == "Line") {
			message >> temp;
			if (std::stringstream(temp) >> line) {
				m_vecErrorsLineNumbers.push_back(line);
			}
		}
	}
}

void T_TerRaTronNewInterfaceWidget::moveToFirstErrorLine()
{
	m_ui->fileContent_textEdit->moveCursor(QTextCursor::Start);
	if (!m_vecErrorsLineNumbers.empty()) {
		for (int i = 0; i < m_vecErrorsLineNumbers[0]; ++i) {
			m_ui->fileContent_textEdit->moveCursor(QTextCursor::Down);
		}
	}
}

void T_TerRaTronNewInterfaceWidget::display(const QString &fileContent)
{
	m_ui->fileContent_textEdit->clear();
	m_ui->fileContent_textEdit->insertPlainText(fileContent);
}

void T_TerRaTronNewInterfaceWidget::setNtcElect(const T_NtcElect& rcNtcElect)
{
	m_NtcElect = rcNtcElect;
}
