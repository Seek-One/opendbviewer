/*
 * QOpenHistoryView.cpp
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>

#include "QOpenHistoryView.h"

QOpenHistoryView::QOpenHistoryView(QWidget* pParent)
	: QWidget(pParent)
{
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);
	pMainLayout->setContentsMargins(0,0,0,0);
	pMainLayout->setSpacing(0);

	m_pHistoryTreeView = new QTreeView(this);
	m_pHistoryTreeView->installEventFilter(this);

	m_pHistoryTreeView->setHeaderHidden(true);
	m_pHistoryTreeView->setRootIsDecorated(false);
	pMainLayout->addWidget(m_pHistoryTreeView);

	m_pHistoryNameLabel = new QLabel();
	m_pHistoryPathLabel = new QLabel();
	m_pHistoryHostLabel = new QLabel();
	m_pHistoryPortLabel = new QLabel();
	m_pHistoryUsernameLabel = new QLabel();

	m_pHistoryInfoWidget = makeHistoryInfo(this);
	pMainLayout->addWidget(m_pHistoryInfoWidget);
	m_pHistoryInfoWidget->setHidden(true);

	connect(m_pHistoryTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onHistoryItemClicked(const QModelIndex&)));
 	connect(m_pHistoryTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onHistoryItemDoubleClicked(const QModelIndex &)));
}

QOpenHistoryView::~QOpenHistoryView()
{
}

QTreeView* QOpenHistoryView::getHistoryTreeView() const
{
	return m_pHistoryTreeView;
}

QLabel* QOpenHistoryView::getHistoryNameLabel() const
{
	return m_pHistoryNameLabel;
}

QLabel* QOpenHistoryView::getHistoryPathLabel() const
{
	return m_pHistoryPathLabel;
}

void QOpenHistoryView::resizeEvent(QResizeEvent *event)
{
	if (event->size().width()!=event->oldSize().width()) {
		emit improveDesign(event->size().width());
	}
}

QWidget* QOpenHistoryView::getHistoryInfoWidget() const
{
	return m_pHistoryInfoWidget;
}

QWidget* QOpenHistoryView::makeHistoryInfo(QWidget* pParent) {
	QWidget* pMainWidget = new QWidget(pParent);
	pMainWidget->setMinimumWidth(m_pHistoryTreeView->width());

	pMainWidget->setStyleSheet("QWidget {border-right: 1px solid #888888; font-size:12px; color:black; font: bold;}");
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	pMainLayout->setContentsMargins(0,0,0,0);

	QGroupBox *pGroupBox = new QGroupBox(tr("Item Informations:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);
	pGroupBox->setContentsMargins(0,0,0,0);

	int iSpace = 7;
	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	pFormLayout->setContentsMargins(0,iSpace,0,0);
	//Create a second QFormLayout for the MySQL/PostgreSQL database

	{
		m_pHistoryNameLabel->setStyleSheet("font:none;");
		pFormLayout->addRow(tr("Name :"), m_pHistoryNameLabel);
	}

	{
		m_pHistoryPathLabel->setStyleSheet("font:none;");
		pFormLayout->addRow(tr("Path :"), m_pHistoryPathLabel);
	}
	return pMainWidget;
}


void QOpenHistoryView::onHistoryItemClicked(const QModelIndex& index)
{
	m_pHistoryInfoWidget->setVisible(true);

	emit openHistoryInfo(index);
}

void QOpenHistoryView::onHistoryItemDoubleClicked(const QModelIndex& index)
{
	m_pHistoryNameLabel->setText("");
	m_pHistoryPathLabel->setText("");
	m_pHistoryInfoWidget->setHidden(true);

	//TODO Switch with the different functions to open other type databases
	emit openHistorySQLiteDatabase(index);
}

bool QOpenHistoryView::eventFilter(QObject* pObject, QEvent* pEvent)
{
	if ((pObject == m_pHistoryTreeView) && (pEvent->type() == QEvent::KeyPress)) {
		QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
		QModelIndex currentIndex = m_pHistoryTreeView->currentIndex();
			if (pKeyEvent->key() == Qt::Key_Up && m_pHistoryTreeView->indexAbove(currentIndex)!=QModelIndex()) {
				m_pHistoryTreeView->setCurrentIndex(m_pHistoryTreeView->indexAbove(currentIndex));
				emit openHistoryInfo(m_pHistoryTreeView->currentIndex());
				return true;
			}
			if (pKeyEvent->key() == Qt::Key_Down && m_pHistoryTreeView->indexBelow(currentIndex)!=QModelIndex()) {
				m_pHistoryTreeView->setCurrentIndex(m_pHistoryTreeView->indexBelow(currentIndex));
				emit openHistoryInfo(m_pHistoryTreeView->currentIndex());
				return true;
			}
	}
	return QWidget::eventFilter(pObject, pEvent);
}

