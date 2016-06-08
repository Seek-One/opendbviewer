/*
 * QDatabaseTableView.cpp
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#include <QVBoxLayout>

#include "QDatabaseTableView.h"

QDatabaseTableView::QDatabaseTableView(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout *pTablePanelLayout = new QVBoxLayout();
	setLayout(pTablePanelLayout);

	QTabWidget *pTablePanelTabs = new QTabWidget(this);
	pTablePanelLayout->addWidget(pTablePanelTabs);

	QWidget *pTableTab1 = new QWidget;
	pTablePanelTabs->addTab(pTableTab1, "Structure");

	m_pTableTab2 = new QWidget;
	m_pTableTab2 = makeDataTab();
	pTablePanelTabs->addTab(m_pTableTab2, "Data");

	QWidget *pTableTab3 = new QWidget();
	pTablePanelTabs->addTab(pTableTab3, "Original SQL creation script");
}

QDatabaseTableView::~QDatabaseTableView()
{

}

QWidget* QDatabaseTableView::makeStructureTab()
{
	QVBoxLayout *pStructureLayout = new QVBoxLayout;
	QWidget *pTableTab1 = new QWidget;
	pTableTab1->setLayout(pStructureLayout);


	return pTableTab1;
}

QWidget* QDatabaseTableView::makeDataTab()
{
	QVBoxLayout *pDataLayout = new QVBoxLayout;
	QWidget *pTableTab2 = new QWidget;
	pTableTab2->setLayout(pDataLayout);

	QHBoxLayout *pHorizontalLayout = new QHBoxLayout;
	pDataLayout->addLayout(pHorizontalLayout);

	QPushButton *pRefreshButton = new QPushButton(tr("&Refresh"));
	pHorizontalLayout->addWidget(pRefreshButton);

	QLabel *filterLabel = new QLabel;
	filterLabel->setText(tr("Filter:"));
	pHorizontalLayout->addWidget(filterLabel);

	QLineEdit *pEntryFilter = new QLineEdit;
	pHorizontalLayout->addWidget(pEntryFilter);

	QPushButton *pClearButton = new QPushButton(tr("&Clear"));
	pHorizontalLayout->addWidget(pClearButton);

	QTabWidget *pQueryResults = new QTabWidget;
	pQueryResults->setTabPosition(QTabWidget::East);
	pDataLayout->addWidget(pQueryResults);

	QWidget *pQueryResultsTab1 = new QWidget;
	pQueryResults->addTab(pQueryResultsTab1, "Results");

	QWidget *PQueryResultsTab2 = new QWidget;
	pQueryResults->addTab(PQueryResultsTab2, "Console");

	return pTableTab2;
}



