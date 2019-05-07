#pragma once

#include <QWidget>
#include <QPointer>
#include <QTreeWidget>

#include "T_NtcElect.hpp"

class Ui_TerRaTronNewInterfaceNoticeTreeView;

class T_TerRaTronNewInterfaceNoticeTreeView : public QWidget
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceNoticeTreeView(QWidget *parent = nullptr);
	virtual ~T_TerRaTronNewInterfaceNoticeTreeView();
	void updateTreeView(QString fn, const T_NtcElect&);

private:
	void initializeGUI();

private:
	Ui_TerRaTronNewInterfaceNoticeTreeView *m_ui;
	QString m_fileName{};
	static const QIcon &correspondingIcon(const int& num);
};