#pragma once
//TODO: Study headers dependencies
#include <QWidget>
#include <QVector>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QPointer>
#include <QThread>

#include "T_Database.hpp"
#include "T_MessageContainer.hpp"
#include "T_TronDBBundle.hpp"
#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TerRaTronNewInterfaceNoticeTreeView.hpp"
#include "T_NtcElectHighlighter.hpp"

class Ui_TerRaTronNewInterfaceWidget;

class T_TerRaTronNewInterfaceWidget : public QWidget
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceWidget(QWidget *parent = 0);
	virtual ~T_TerRaTronNewInterfaceWidget();
	
private:
	void initializeGUI();
	void setNtcElect(const T_NtcElect&);
	void display(const QString&);
	void moveToFirstErrorLine();
	

public slots:
	void validate();
	void saveValidate();
	void save();
	void autoValidate();
	/******************/
	void readFile();
	void showResult(const T_NtcElect&);
	void handleCursorPositionChanged();
	void extractLineNumber();
	void showColoredErrorLines();
	
	

signals:
	void readFileCompleted();

private:
	Ui_TerRaTronNewInterfaceWidget *m_ui;
	T_NtcElect m_NtcElect;
	QPointer<T_NtcElectHighlighter> m_highlighter1;
	QPointer<T_TerRaTronNewInterfaceObject> m_worker;
	QPointer<QThread> m_workerThread;

	QVector<int> m_vecErrorsLineNumbers;

	QString m_pathFile{};
	QString m_fileName{};

	QDir m_dir;
};