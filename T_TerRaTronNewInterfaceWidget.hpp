#pragma once

#include <QWidget>
#include <QVector>
#include <QPointer>
#include <QThread>
#include <QSettings>

#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TerRaTronNewInterfaceNoticeTreeView.hpp"
#include "T_NtcElectHighlighter.hpp"

class Ui_TerRaTronNewInterfaceWidget;

class T_TerRaTronNewInterfaceWidget : public QWidget
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceWidget(QWidget *parent = nullptr);
	virtual ~T_TerRaTronNewInterfaceWidget();
	
private:
	void initializeGUI();
	
public:
	void dialogSaveFile();
	void setNtcElect(const T_NtcElect&);
	void showCursorPosition();
	void coloredErrorLines();
	void clearResetAll();
	void infoMessageOnFlyFactory(const char* msg, const QColor &);
	void extractErrorLineNumber(const QString&);
	void moveToErrorLine(int);
	void launchProgressBar();


public slots:
	void validate();
	void saveValidate();
	void save();
	void autoValidate();
	void showHide();
	void closeFile();
	void openFile();
	void showResult(const T_NtcElect&);
	void handleCursorPositionChanged();
	void onAnchorClicked(const QUrl &link);

signals:
	/*Signals sent from widget to mainWindow
	in order to activate/deactivate tool bar*/
	void taskCompleted();
	void disableWindowsInMain();

private:
	Ui_TerRaTronNewInterfaceWidget *m_ui;
	T_NtcElect m_NtcElect;
	T_String m_NoticeMessages;
	QSettings m_settings;
	bool m_fileIsOpen = false;
	bool m_inValidation = false;
	int m_OnCLickLinkErrorNumber{};

	QPointer<T_NtcElectHighlighter> m_highlighter1;
	QPointer<T_TerRaTronNewInterfaceObject> m_worker;
	QPointer<QThread> m_workerThread;

	QVector<int> m_vecErrorsLineNumbers;

	QString m_pathFile{};
	QString m_fileName{};
};