/*
 * QOpenHistoryView.cpp
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QHeaderView>

#include "QOpenHistoryView.h"

QOpenHistoryView::QOpenHistoryView(QWidget* pParent)
	: QWidget(pParent)
{
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);
	pMainLayout->setContentsMargins(0,0,0,0);
	pMainLayout->setSpacing(0);

	m_pHistoryTreeView = new QTreeView(this);
	m_pHistoryNameLabel = new QLabel();
	m_pHistoryPathLabel = new QLabel();
	m_pHistoryHostLabel = new QLabel();
	m_pHistoryPortLabel = new QLabel();
	m_pHistoryUsernameLabel = new QLabel();

	m_pHistoryTreeView->setHeaderHidden(true);
	m_pHistoryTreeView->header()->setDefaultAlignment(Qt::AlignCenter);
	pMainLayout->addWidget(m_pHistoryTreeView);

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
	//set the color of the background without side effect on border

	pFormLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
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

