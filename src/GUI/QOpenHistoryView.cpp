/*
 * QOpenHistoryView.cpp
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#include <QVBoxLayout>

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

	m_pHistoryInfoStackedWidget = new QStackedWidget();
	pMainLayout->addWidget(m_pHistoryInfoStackedWidget);
	m_pHistoryInfoStackedWidget->setMinimumWidth(m_pHistoryTreeView->width());
	m_pHistoryInfoStackedWidget->setHidden(true);

	connect(m_pHistoryTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onHistoryItemClicked(const QModelIndex&)));
 	connect(m_pHistoryTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onHistoryItemDoubleClicked(const QModelIndex &)));
}

QOpenHistoryView::~QOpenHistoryView()
{
}

QTreeView* QOpenHistoryView::getHistoryTreeView() const
{
	return m_pHistoryTreeView;
}

QStackedWidget* QOpenHistoryView::getHistoryInfoStackedWidget() const
{
	return m_pHistoryInfoStackedWidget;
}

QLabel* QOpenHistoryView::getHistoryNameLabel() const
{
	return m_pHistoryNameLabel;
}

QLabel* QOpenHistoryView::getHistoryPathLabel() const
{
	return m_pHistoryPathLabel;
}

QLabel* QOpenHistoryView::getHistoryHostLabel() const
{
	return m_pHistoryHostLabel;
}

QLabel* QOpenHistoryView::getHistoryPortLabel() const
{
	return m_pHistoryPortLabel;
}

QLabel* QOpenHistoryView::getHistoryUsernameLabel() const
{
	return m_pHistoryUsernameLabel;
}

void QOpenHistoryView::resizeEvent(QResizeEvent *event)
{
	if (event->size().width()!=event->oldSize().width()) {
		emit improveDesign(event->size().width());
	}
}

void QOpenHistoryView::onHistoryItemClicked(const QModelIndex& index)
{
	emit callHistoryInfo(index);
}

void QOpenHistoryView::onHistoryItemDoubleClicked(const QModelIndex& index)
{
	emit callHistoryDatabase(index);
}

bool QOpenHistoryView::eventFilter(QObject* pObject, QEvent* pEvent)
{
	if ((pObject == m_pHistoryTreeView) && (pEvent->type() == QEvent::KeyPress)) {
		QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
		QModelIndex currentIndex = m_pHistoryTreeView->currentIndex();
			if (pKeyEvent->key() == Qt::Key_Up && m_pHistoryTreeView->indexAbove(currentIndex) != QModelIndex()) {
				m_pHistoryTreeView->setCurrentIndex(m_pHistoryTreeView->indexAbove(currentIndex));
				emit callHistoryInfo(m_pHistoryTreeView->currentIndex());
				return true;
			}
			if (pKeyEvent->key() == Qt::Key_Down && m_pHistoryTreeView->indexBelow(currentIndex) != QModelIndex()) {
				m_pHistoryTreeView->setCurrentIndex(m_pHistoryTreeView->indexBelow(currentIndex));
				emit callHistoryInfo(m_pHistoryTreeView->currentIndex());
				return true;
			}
	}
	return QWidget::eventFilter(pObject, pEvent);
}

