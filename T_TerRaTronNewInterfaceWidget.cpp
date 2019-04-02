#include <QtWidgets>
#include <QMainWindow>

#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TerRaTronNewInterfaceWidget.hpp"
#include "ui_TerRaTronNewInterfaceWidget.h"

T_TerRaTronNewInterfaceWidget::T_TerRaTronNewInterfaceWidget(QWidget *parent): 
	QWidget(parent), m_dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)), m_worker{ new T_TerRaTronNewInterfaceObject }
{
    m_worker->moveToThread(&m_workerThread);

	connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

	connect(this, &T_TerRaTronNewInterfaceWidget::readFileCompleted, m_worker, &T_TerRaTronNewInterfaceObject::validate);
	connect(m_worker, &T_TerRaTronNewInterfaceObject::resultReady, this, &T_TerRaTronNewInterfaceWidget::showResult);	
	m_workerThread.start();

	initializeGUI();

	connect(m_ui->actionSave_and_revalidate, &QAction::triggered, this, &T_TerRaTronNewInterfaceWidget::validate);
	connect(m_ui->fileContent_textEdit, &QPlainTextEdit::textChanged, this, &T_TerRaTronNewInterfaceWidget::validate);
}

T_TerRaTronNewInterfaceWidget::~T_TerRaTronNewInterfaceWidget()
{
	m_workerThread.quit();
	m_workerThread.wait();

	delete m_ui;
}

void T_TerRaTronNewInterfaceWidget::initializeGUI() 
{
	m_ui = new Ui_TerRaTronNewInterfaceWidget;
	m_ui->setupUi(this);

	
}

void T_TerRaTronNewInterfaceWidget::validate()
{
	if (m_isReadingFile && sender() != (m_ui->actionSave_and_revalidate)) {
		m_isReadingFile = false;
		return;
	}

	m_ui->sectionContent_textEdit->setPlainText("Validating ...");
	m_fileContent = m_ui->fileContent_textEdit->toPlainText();

	if (sender() == m_ui->actionSave_and_revalidate) 
	{
		QFile file(m_fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream stream(&file);
			stream << m_fileContent;
		}
		file.close();
	}

	QByteArray fileContentQByte = m_fileContent.toUtf8();
	QBuffer buf(&fileContentQByte);

	bool ok = buf.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ok)
	{
		QMetaObject::invokeMethod(m_worker, "validate", Q_ARG(QString, m_fileContent));
	}
}

void T_TerRaTronNewInterfaceWidget::readFile()
{
	m_isReadingFile = true;
	const QString filter = QString::fromUtf8("(*.txt)");
	m_fileName = QFileDialog::getOpenFileName(this, QString(), m_dir.absolutePath(), filter);

	if (m_fileName.isEmpty()) return;

	m_dir = QFileInfo(m_fileName).absolutePath();
	m_fileHandler.setFileName(m_fileName);

	if (!m_fileHandler.open(QIODevice::ReadWrite | QIODevice::Text)) return;

	QTextStream inputStream(&m_fileHandler);
	QString fileLine = inputStream.readLine();

	while (!fileLine.isNull())
	{
		m_fileContent.append(fileLine);
		fileLine = inputStream.readLine();
		if (!fileLine.isEmpty()) m_fileContent.append("\n");
	}

	m_pathFile = m_dir.absoluteFilePath(m_fileName);
	display(m_fileContent);
	m_fileHandler.close();

    Q_EMIT readFileCompleted(m_fileContent);
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

void T_TerRaTronNewInterfaceWidget::showResult(T_TronMessageContainer messageContainer, T_String messages)
{
	qDebug() << Q_FUNC_INFO;

	m_ui->sectionContent_textEdit->clear();
	m_ui->errorsWarn_textBrowser->clear();

	m_ui->sectionContent_textEdit->insertPlainText(QString("File name: %1 \n").arg(m_pathFile));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of errors: %1 \n").arg(messageContainer.getErrorCount()));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of warnings: %1 \n").arg(messageContainer.getWarningCount()));
	m_ui->errorsWarn_textBrowser->insertPlainText(messages);
}

QAction *T_TerRaTronNewInterfaceWidget::actionSaveAndRevalidate() const
{
	return m_ui->actionSave_and_revalidate;
}