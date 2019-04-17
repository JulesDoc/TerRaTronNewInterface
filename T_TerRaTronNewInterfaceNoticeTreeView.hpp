#pragma once

#include <QWidget>
#include <QPointer>
#include <QTreeWidget>

#include "T_NtcElect.hpp"
#include "T_MessageContainer.hpp"

class T_TerRaTronNewInterfaceNoticeTreeView : public QWidget
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceNoticeTreeView(QWidget *parent = 0);
	~T_TerRaTronNewInterfaceNoticeTreeView();
	void updateTreeView(QString, const T_NtcElect&);

private:
	QPointer<QTreeWidget> m_treeWidget;
	QString m_fileName{};
	static const QIcon &correspondingIcon(const int& num);
};