#pragma once
//TODO: Study headers dependencies
//#include <QMainWindow>
#include <QWidget>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QPointer>
#include <QThread>

#include "T_Database.hpp"
//#include "T_NtcElect.hpp"
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
	void display(const QString&);
	
private:
	void initializeGUI();

public slots:
	void validate();
	void saveValidate();
	void autoValidate();
	/******************/
	void readFile();
	void showResult(const T_NtcElect&);
	void handleCursorPositionChanged();

signals:
	void readFileCompleted(const QString &);

private:
	Ui_TerRaTronNewInterfaceWidget *m_ui;
	//T_NtcElectHighlighter *m_highlighter1;
	//T_TerRaTronNewInterfaceObject *m_worker;
	QPointer<T_NtcElectHighlighter> m_highlighter1;
	QPointer<T_TerRaTronNewInterfaceObject> m_worker;
	QPointer<QThread> m_workerThread;

	QString m_pathFile{};
	QString m_fileName{};
	QString m_fileContent{};

	QDir m_dir;
	
	
};