
#include <QTreeView>
#include <QDir>
#include <QPointer>
#include <QVBoxLayout>
#include <QMessageBox>

#include "T_TerRaTronNewInterfaceNoticeTreeView.hpp"


T_TerRaTronNewInterfaceNoticeTreeView::T_TerRaTronNewInterfaceNoticeTreeView(QWidget *parent) :
	QWidget(parent)
{
	m_treeWidget = new QTreeWidget(this);
	QPointer<QLayout> layout = new QVBoxLayout(this);
	layout->addWidget(m_treeWidget);
}


T_TerRaTronNewInterfaceNoticeTreeView::~T_TerRaTronNewInterfaceNoticeTreeView()
{
}

void T_TerRaTronNewInterfaceNoticeTreeView::updateTreeView(QString fileName, const T_NtcElect& rcNtcElect)
{
	m_treeWidget->clear();
	//updating with an empty file means clear and return
	if (fileName.isEmpty()) return;

	m_treeWidget->setColumnCount(1);
	
		QTreeWidgetItem *parentItem = new QTreeWidgetItem(m_treeWidget);
		parentItem->setText(0, fileName);
		parentItem->setToolTip(0, QDir::toNativeSeparators(fileName));
		parentItem->setIcon(0, QIcon());

		QTreeWidgetItem *headItemSection = new QTreeWidgetItem(parentItem);
		headItemSection->setText(0, tr("Head Section"));
		parentItem->addChild(headItemSection);

		QTreeWidgetItem *headItemErrors = new QTreeWidgetItem(headItemSection);
		headItemErrors->setText(0, tr("Head Errors: %1").arg(rcNtcElect.getHeadSection().getErrorCount()));
		headItemSection->addChild(headItemErrors);
		headItemSection->setIcon(0, correspondingIcon(rcNtcElect.getHeadSection().getErrorCount()));

		QTreeWidgetItem *headItemWarnings = new QTreeWidgetItem(headItemSection);
		headItemWarnings->setText(0, tr("Head Warnings: %1").arg(rcNtcElect.getHeadSection().getWarningCount()));
		headItemSection->addChild(headItemWarnings);

	const int indexNoticeCount = rcNtcElect.getNoticeSectionCount();
	for (int i = 0; i < indexNoticeCount; ++i) {
		
		QTreeWidgetItem *noticeItemSection = new QTreeWidgetItem(parentItem);
		noticeItemSection->setText(0, tr("Notice %1").arg(i+1));
		parentItem->addChild(noticeItemSection);
		noticeItemSection->setIcon(0, correspondingIcon(rcNtcElect.getNoticeSectionAt(i).getErrorCount()));

		QTreeWidgetItem *noticeItemErrors = new QTreeWidgetItem(noticeItemSection);
		noticeItemErrors->setText(0, tr("Notice Errors: %1").arg(rcNtcElect.getNoticeSectionAt(i).getErrorCount()));
		noticeItemSection->addChild(noticeItemErrors);

		QTreeWidgetItem *noticeItemWarnings = new QTreeWidgetItem(noticeItemSection);
		noticeItemWarnings->setText(0, tr("Notice Warnings: %1").arg(rcNtcElect.getNoticeSectionAt(i).getWarningCount()));
		noticeItemSection->addChild(noticeItemWarnings);

	}
	m_treeWidget->expandAll();
}

const QIcon &T_TerRaTronNewInterfaceNoticeTreeView::correspondingIcon(const int& num)
{
	static QIcon okIcon = QIcon(QString::fromLatin1(":/images/ok.png"));
	static QIcon warningIcon = QIcon(QString::fromLatin1(":/images/qmessagebox-warn.png"));
	static QIcon errorIcon = QIcon(QString::fromLatin1(":/images/qmessagebox-crit.png"));

	if (num > 0)
		return errorIcon;
	else if (num > 0)
		return warningIcon;
	return okIcon;
}