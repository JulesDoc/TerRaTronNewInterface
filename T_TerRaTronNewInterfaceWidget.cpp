
#include <QSignalSpy>
#include <QMessageBox>
#include <QCoreApplication>
#include <algorithm>
#include <regex>

#include "T_Configuration.hpp"
#include "Ui_TerRaTronNewInterfaceWidget.h"
#include "T_TerRaTronNewInterfaceWidget.hpp"
#include "codeeditor.h"
#include "T_Message.hpp"

T_TerRaTronNewInterfaceWidget::T_TerRaTronNewInterfaceWidget(QWidget *parent)
	:QWidget(parent)
{
	m_workerThread = new QThread(this);
	m_worker = new T_TerRaTronNewInterfaceObject(/*this*/); // Orphan object
	//The instance m_worker cannot be moved to thread if it has a parent, that is why it is orphan.
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
	m_ui->progressBar->hide();
	clearResetAll();

	connect(m_ui->actionSave_and_revalidate, &QAction::triggered, this, &T_TerRaTronNewInterfaceWidget::saveValidate);
	connect(m_ui->autoValidate_checkBox, &QCheckBox::toggled, this, &T_TerRaTronNewInterfaceWidget::autoValidate);
	connect(m_ui->errorsWarn_textBrowser, &QTextBrowser::anchorClicked, this, &T_TerRaTronNewInterfaceWidget::onAnchorClicked);
	connect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(handleCursorPositionChanged()));

	m_highlighter1 = new T_NtcElectHighlighter(false, m_ui->fileContent_textEdit->document());
}

void T_TerRaTronNewInterfaceWidget::openFile()
{
	if (m_inValidation) return;
	QDir currentDir;
	m_fileName = QFileDialog::getOpenFileName(this, QString(), T_Configuration::getSettingsPath(),
		                                      tr("All files (*.*);;xml (*.xml);;txt (*.txt)"));
	if (m_fileName.isEmpty()) return;
	if (m_fileIsOpen) closeFile();

	infoMessageOnFlyFactory("Opening file, please wait ...", Qt::darkBlue);
	launchProgressBar();
	QCoreApplication::processEvents();

	//Refresh settingsPath (QSettings member) with new path
	T_Configuration::setSettingsPath(currentDir.absoluteFilePath(m_fileName));

	QFile fileHandler(m_fileName);
	if (!fileHandler.open(QFile::ReadWrite | QFile::Text)) return;
	m_ui->fileContent_textEdit->setPlainText(fileHandler.readAll());

	m_fileIsOpen = true;
	m_ui->labelTextWarning->setVisible(false);
	m_pathFile = currentDir.absoluteFilePath(m_fileName);
	fileHandler.close();
	/****Validate when opening file, by default*****/
	validate();
	/*********/
}

void T_TerRaTronNewInterfaceWidget::validate()
{
	launchProgressBar();
	m_ui->save_and_revalidate_button->setDisabled(true);
	Q_EMIT(disableWindowsInMain());
	m_ui->progressBar->setValue(10);
	infoMessageOnFlyFactory("Validating notices, please wait ...", Qt::darkBlue);
	//m_ui->progressBar->setValue(40);
	QCoreApplication::processEvents();
	//System is in Validation. Useful for big number of notices.
	m_inValidation = true;
	qRegisterMetaType<T_NtcElect>();
	QSignalSpy spy(m_worker, SIGNAL(resultReady(const T_NtcElect&)));
	QMetaObject::invokeMethod(m_worker, "validate", Q_ARG(QString, m_ui->fileContent_textEdit->toPlainText()));
	int num = 10;
	while (spy.count() != 1) 
	{
		long long i{};
		while (i < 60000000) {
			i++;
		}
		++num;
		if (num < 90) m_ui->progressBar->setValue(num);
	}
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

void T_TerRaTronNewInterfaceWidget::showResult(const T_NtcElect& rcNtcElect)
{
	m_ui->sectionContent_textEdit->clear();
	m_ui->errorsWarn_textBrowser->clear();

	m_ui->errorsWarn_textBrowser->setOpenExternalLinks(false);
	m_ui->errorsWarn_textBrowser->setOpenLinks(false);

	QString msgToShow;
	QString msgToLink;
	T_String messages;

	setNtcElect(rcNtcElect);
	m_NtcElect.getMessagesAsString(messages);
	
	m_ui->sectionContent_textEdit->insertPlainText(QString("File name: %1 \n").arg(m_pathFile));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of errors: %1 \n").arg(m_NtcElect.getErrorCount()));
	m_ui->sectionContent_textEdit->insertPlainText(QString("Total number of warnings: %1 \n").arg(m_NtcElect.getWarningCount()));
	m_ui->progressBar->setValue(95);
	QCoreApplication::processEvents();

	//Showing notices error messages in different lines of Html code
	//Dealing with a bunch of chars inside messages of NtcElect
	for (int i = 0; i < messages.size(); ++i) {
		msgToShow.append(messages.getAt(i));
		if ((messages.getAt(i) == '\n' && messages.getAt(i+1) == '\n') || i+1 == messages.size()){
			std::string temp = msgToShow.toStdString();
			remove_if(temp.begin(), temp.end(), isspace);
			msgToLink = QString::fromStdString(temp);
			m_ui->errorsWarn_textBrowser->append("<a href=" + msgToLink + ">" + msgToShow + "</a>");
			m_ui->errorsWarn_textBrowser->insertPlainText("\n");
			msgToShow.clear();
			msgToLink.clear();
		}
	}
	m_ui->ntcElectTreeWidget->updateTreeView(m_fileName, m_NtcElect);

	//When there are errors. Show colored lines and move cursor to first one.
	if (m_NtcElect.getErrorCount() > 0 || m_NtcElect.getWarningCount() > 0) {
		m_vecErrorsLineNumbers.clear();
		extractErrorLineNumber(messages);
		VERIFY(!m_vecErrorsLineNumbers.empty());
		disconnect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(handleCursorPositionChanged()));
		moveToErrorLine(m_vecErrorsLineNumbers[0]);
		connect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(handleCursorPositionChanged()));
		coloredErrorLines();
	}else m_ui->fileContent_textEdit->moveCursor(QTextCursor::Start);

	m_ui->save_and_revalidate_button->setDisabled(false);
	m_inValidation = false;
	m_ui->progressBar->setValue(100);
	m_ui->progressBar->hide();
	//Signal to mainWindow in order to activate actions / tool bar
	Q_EMIT(readValidateFileCompleted());
}

//Method used to show/hide the checkable box for autovalidation
void T_TerRaTronNewInterfaceWidget::showHide()
{
	if (!m_ui->autoValidate_checkBox->isVisible()) {
		m_ui->autoValidate_checkBox->setVisible(true);
		return;
	}
	if (m_ui->autoValidate_checkBox->isVisible()) {
		if (m_ui->autoValidate_checkBox->isChecked())
			m_ui->autoValidate_checkBox->setChecked(false);
		m_ui->autoValidate_checkBox->setVisible(false);
		return;
	}
}
void T_TerRaTronNewInterfaceWidget::autoValidate()
{
	//connect or disconnect the automatic validation when check/uncheck the box
	if (m_ui->autoValidate_checkBox->isChecked()) {
		connect(m_ui->fileContent_textEdit, &QPlainTextEdit::textChanged, this, &T_TerRaTronNewInterfaceWidget::validate);
		//Validate by default when it get activated
		validate();
		return;
	}
	disconnect(m_ui->fileContent_textEdit, &QPlainTextEdit::textChanged, this, &T_TerRaTronNewInterfaceWidget::validate);
}

void T_TerRaTronNewInterfaceWidget::onAnchorClicked(const QUrl &link)
{
	QString msgLine = link.toEncoded(QUrl::EncodeSpaces);
	std::string str = msgLine.toStdString();
	extractErrorLineNumber(msgLine);
	VERIFY(m_OnCLickLinkErrorNumber != 0);
	disconnect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(handleCursorPositionChanged()));
	moveToErrorLine(m_OnCLickLinkErrorNumber);
	connect(m_ui->fileContent_textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(handleCursorPositionChanged()));
	coloredErrorLines();
}

void T_TerRaTronNewInterfaceWidget::closeFile()
{
	dialogSaveFile();
	clearResetAll();
	//Signal to mainWindow in order to deactivate actions / tool bar
	Q_EMIT(closeFileCompleted());
}

void T_TerRaTronNewInterfaceWidget::handleCursorPositionChanged()
{
	showCursorPosition();
	coloredErrorLines();
}

void T_TerRaTronNewInterfaceWidget::setNtcElect(const T_NtcElect& rcNtcElect)
{
	m_NtcElect = rcNtcElect;
}

/*UTILITIES*/
void T_TerRaTronNewInterfaceWidget::launchProgressBar()
{
	PRECONDITION(m_ui->progressBar);
	m_ui->progressBar->show();
	m_ui->progressBar->setMinimum(0);
	m_ui->progressBar->setValue(5);
}
void T_TerRaTronNewInterfaceWidget::clearResetAll()
{
	m_vecErrorsLineNumbers.clear();
	if (m_ui->autoValidate_checkBox->isChecked())
		m_ui->autoValidate_checkBox->setChecked(false);
	m_ui->autoValidate_checkBox->setDisabled(true);
	m_ui->autoValidate_checkBox->setVisible(false);
	m_ui->fileContent_textEdit->clear();
	m_ui->labelTextWarning->setVisible(false);
	m_ui->sectionContent_textEdit->clear();
	m_ui->errorsWarn_textBrowser->clear();
	m_ui->save_and_revalidate_button->setDisabled(true);
	//It will clear treeWidget and return
	m_ui->ntcElectTreeWidget->updateTreeView("", m_NtcElect);
	m_fileIsOpen = false;
}

void T_TerRaTronNewInterfaceWidget::extractErrorLineNumber(const QString& msg)
{
	std::string str = msg.toStdString();
	remove_if(str.begin(), str.end(), isspace);
	std::smatch match;
	std::regex re("Line(\\d+):");
	std::sregex_iterator next(str.begin(), str.end(), re);
	std::sregex_iterator end;
	while (next != end) {
		match = *next;
		VERIFY(!match.empty());
		std::string temp = match[1].str();
		if (sender() == m_ui->errorsWarn_textBrowser) {
			m_OnCLickLinkErrorNumber = std::stoi(temp);
			return;
		}
		else
		{
			m_vecErrorsLineNumbers.push_back(std::stoi(temp));
			next++;
		}
	}
}

void T_TerRaTronNewInterfaceWidget::moveToErrorLine(int errorPosition)
{
	PRECONDITION(m_ui->sectionContent_textEdit);

	int sizeDocument = m_ui->fileContent_textEdit->document()->lineCount();
	if (errorPosition > (sizeDocument / 2)) 
	{
		int limit = sizeDocument - errorPosition;
		m_ui->fileContent_textEdit->moveCursor(QTextCursor::End);
		for (int i = 0; i <= limit; ++i) {
			m_ui->fileContent_textEdit->moveCursor(QTextCursor::Up);
		}
	}
	else 
	{
		m_ui->fileContent_textEdit->moveCursor(QTextCursor::Start);
		for (int i = 0; i < errorPosition; ++i) {
			m_ui->fileContent_textEdit->moveCursor(QTextCursor::Down);
		}
	}
}

void T_TerRaTronNewInterfaceWidget::dialogSaveFile()
{
	PRECONDITION(m_ui->labelTextWarning);
	//If user chooses closing file without a previous saving
	if (m_ui->labelTextWarning->isVisible())
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText("The content of the file has been modified");
		msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		switch (ret) {
		case QMessageBox::Save:
			save();
			break;
		case QMessageBox::Discard:
			break;
		case QMessageBox::Cancel:
			return;
			break;
		default:
			break;
		}
	}
}

void T_TerRaTronNewInterfaceWidget::showCursorPosition() {

	PRECONDITION(m_ui->sectionContent_textEdit);

	QTextCursor cursor = m_ui->fileContent_textEdit->textCursor();

	const int line = cursor.blockNumber() + 1;
	const int col = cursor.columnNumber();

	m_ui->fileContent_lineNumber->setText(tr("Ln %1").arg(line));
	m_ui->fileContent_colNumber->setText(tr("Col %1").arg(col));
}

//To color error lines
void T_TerRaTronNewInterfaceWidget::coloredErrorLines()
{
	PRECONDITION(m_ui->fileContent_textEdit);

	if (m_NtcElect.getErrorCount() != 0 || m_NtcElect.getWarningCount() != 0)
	{
		QList<QTextEdit::ExtraSelection> extraSelections;
		QTextEdit::ExtraSelection selection;
		QColor lineColor = QColor(Qt::red).lighter(160);
		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);

		for (int line : m_vecErrorsLineNumbers)
		{
			QTextCursor cursor(m_ui->fileContent_textEdit->document()->findBlockByLineNumber(line - 1));
			selection.cursor = cursor;
			selection.cursor.clearSelection();
			extraSelections.append(selection);
			m_ui->fileContent_textEdit->setExtraSelections(extraSelections);
		}
	}
}

void T_TerRaTronNewInterfaceWidget::infoMessageOnFlyFactory(const char* cStr, const QColor &c)
{
	PRECONDITION(m_ui->sectionContent_textEdit);
	m_ui->sectionContent_textEdit->clear();
	m_ui->sectionContent_textEdit->setTextColor(c);
	m_ui->sectionContent_textEdit->append(cStr);
	QFont serifFont("Times", 10, QFont::Bold);
	m_ui->sectionContent_textEdit->setFont(serifFont);
}