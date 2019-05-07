
#include <QTreeView>
#include <QDir>
#include <QPointer>
#include <QVBoxLayout>
#include <QMessageBox>
#include "T_TerRaTronNewInterfaceNoticeTreeView.hpp"
#include "Ui_TerRaTronNewInterfaceNoticeTreeView.h"

T_TerRaTronNewInterfaceNoticeTreeView::T_TerRaTronNewInterfaceNoticeTreeView(QWidget *parent) :
	QWidget(parent)
{
	initializeGUI();
}

void T_TerRaTronNewInterfaceNoticeTreeView::initializeGUI()
{
	m_ui = new Ui_TerRaTronNewInterfaceNoticeTreeView;
	m_ui->setupUi(this);
}

T_TerRaTronNewInterfaceNoticeTreeView::~T_TerRaTronNewInterfaceNoticeTreeView()
{

}

void T_TerRaTronNewInterfaceNoticeTreeView::updateTreeView(QString fileName, const T_NtcElect& rcNtcElect)
{
	m_ui->treeWidget->clear();
	//updating with an empty file means clear the tree and return
	if (fileName.isEmpty()) return;

	PRECONDITION(!rcNtcElect.isUnknown());

	bool allClean = true;
	QFont parentFont("Times", 10, QFont::Bold);
	QFont headFont("Times", 9, QFont::Bold);
	QFont noticeFont("Times", 9, QFont::Bold);
	m_ui->treeWidget->setColumnCount(1);
	
		QTreeWidgetItem *parentItem = new QTreeWidgetItem(m_ui->treeWidget);
		parentItem->setText(0, fileName);
		parentItem->setFont(0, parentFont);
		parentItem->setToolTip(0, QDir::toNativeSeparators(fileName));
		parentItem->setIcon(0, QIcon());

		QTreeWidgetItem *headItemSection = new QTreeWidgetItem(parentItem);
		headItemSection->setText(0, tr("Head Section"));
		headItemSection->setFont(0, headFont);
		parentItem->addChild(headItemSection);

		QTreeWidgetItem *headItemErrors = new QTreeWidgetItem(headItemSection);
		headItemErrors->setText(0, tr("Head Errors: %1").arg(rcNtcElect.getHeadSection().getErrorCount()));
		headItemSection->addChild(headItemErrors);
		headItemSection->setIcon(0, correspondingIcon(rcNtcElect.getHeadSection().getErrorCount()));

		QTreeWidgetItem *headItemWarnings = new QTreeWidgetItem(headItemSection);
		headItemWarnings->setText(0, tr("Head Warnings: %1").arg(rcNtcElect.getHeadSection().getWarningCount()));
		headItemSection->addChild(headItemWarnings);
		if (rcNtcElect.getHeadSection().getErrorCount() > 0 || rcNtcElect.getHeadSection().getWarningCount() > 0) {
			allClean = false;
			m_ui->treeWidget->expandItem(parentItem);
			m_ui->treeWidget->expandItem(headItemSection);
		}

	const int indexNoticeCount = rcNtcElect.getNoticeSectionCount();
	//Multiple notices
	for (int i = 0; i < indexNoticeCount; ++i) {
		if (rcNtcElect.getNoticeSectionAt(i).getErrorCount() > 0 || rcNtcElect.getNoticeSectionAt(i).getWarningCount() > 0) {
			allClean = false;
			QTreeWidgetItem *noticeItemSection = new QTreeWidgetItem(parentItem);
			noticeItemSection->setText(0, tr("Notice %1").arg(i + 1));
			parentItem->addChild(noticeItemSection);
			noticeItemSection->setFont(0, noticeFont);
			noticeItemSection->setIcon(0, correspondingIcon(rcNtcElect.getNoticeSectionAt(i).getErrorCount()));

			QTreeWidgetItem *noticeItemErrors = new QTreeWidgetItem(noticeItemSection);
			noticeItemErrors->setText(0, tr("Notice Errors: %1").arg(rcNtcElect.getNoticeSectionAt(i).getErrorCount()));
			noticeItemSection->addChild(noticeItemErrors);

			QTreeWidgetItem *noticeItemWarnings = new QTreeWidgetItem(noticeItemSection);
			noticeItemWarnings->setText(0, tr("Notice Warnings: %1").arg(rcNtcElect.getNoticeSectionAt(i).getWarningCount()));
			noticeItemSection->addChild(noticeItemWarnings);
			m_ui->treeWidget->expandItem(parentItem);
			m_ui->treeWidget->expandItem(noticeItemSection);
		}
	}

	if (allClean) {
		QTreeWidgetItem *noticeItemSection = new QTreeWidgetItem(parentItem);
		if (indexNoticeCount == 1) noticeItemSection->setText(0, tr("There is 1 notice, it is correct"));
		else noticeItemSection->setText(0, tr("There are %1 notices, all are correct").arg(indexNoticeCount));
		parentItem->addChild(noticeItemSection);
		noticeItemSection->setFont(0, noticeFont);
		noticeItemSection->setIcon(0, QIcon(QString::fromLatin1(":/images/ok.png")));
		m_ui->treeWidget->expandItem(parentItem);
	}

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