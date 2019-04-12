
#include <QTreeView>
#include <QDir>
#include <QPointer>
#include <QVBoxLayout>

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
	m_treeWidget->setColumnCount(1);
	const int indexNoticeCount = rcNtcElect.getNoticeSectionCount();
	for (int i = 0; i < indexNoticeCount; ++i) {

		QTreeWidgetItem *parentItem = new QTreeWidgetItem(m_treeWidget);
		parentItem->setText(0, fileName);
		parentItem->setToolTip(0, QDir::toNativeSeparators(fileName));

		QTreeWidgetItem *headItemSection = new QTreeWidgetItem(parentItem);
		headItemSection->setText(0, tr("Head Section"));
		parentItem->addChild(headItemSection);

		QTreeWidgetItem *headItemErrors = new QTreeWidgetItem(headItemSection);
		headItemErrors->setText(0, tr("Head Errors: %1").arg(rcNtcElect.getHeadSection().getErrorCount()));
		headItemSection->addChild(headItemErrors);

		QTreeWidgetItem *headItemWarnings = new QTreeWidgetItem(headItemSection);
		headItemWarnings->setText(0, tr("Head Warnings: %1").arg(rcNtcElect.getHeadSection().getWarningCount()));
		headItemSection->addChild(headItemWarnings);

		QTreeWidgetItem *noticeItemSection = new QTreeWidgetItem(parentItem);
		noticeItemSection->setText(0, tr("Notice %1").arg(indexNoticeCount));
		parentItem->addChild(noticeItemSection);

		QTreeWidgetItem *noticeItemErrors = new QTreeWidgetItem(noticeItemSection);
		noticeItemErrors->setText(0, tr("Notice Errors: %1").arg(rcNtcElect.getNoticeSectionAt(0).getErrorCount()));
		noticeItemSection->addChild(noticeItemErrors);

		QTreeWidgetItem *noticeItemWarnings = new QTreeWidgetItem(noticeItemSection);
		noticeItemWarnings->setText(0, tr("Notice Warnings: %1").arg(rcNtcElect.getNoticeSectionAt(0).getWarningCount()));
		noticeItemSection->addChild(noticeItemWarnings);
	}
	m_treeWidget->expandAll();
}