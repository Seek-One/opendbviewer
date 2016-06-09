/*
 * QDatabaseTableView.cpp
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#include "QDatabaseTableView.h"

QDatabaseTableView::QDatabaseTableView(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout *pTablePanelLayout = new QVBoxLayout();
	setLayout(pTablePanelLayout);

	QTabWidget *pTablePanelTabs = new QTabWidget(this);
	pTablePanelLayout->addWidget(pTablePanelTabs);

	QWidget *pTableTab1 = new QWidget;
	pTableTab1 = makeStructureTab();
	pTablePanelTabs->addTab(pTableTab1, "Structure");

	QWidget *pTableTab2 = new QWidget;
	pTableTab2 = makeDataTab();
	pTablePanelTabs->addTab(pTableTab2, "Data");

	QWidget *pTableTab3 = new QWidget();
	pTableTab3 = makeCreationScriptTab();
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

	QTableView *pStructureTable = new QTableView;
	pStructureLayout->addWidget(pStructureTable);//TODO Improve table

	return pTableTab1;
}

QWidget* QDatabaseTableView::makeDataTab()
{
	QVBoxLayout *pDataLayout = new QVBoxLayout;
	QWidget *pTableTab2 = new QWidget;
	pTableTab2->setLayout(pDataLayout);

	QHBoxLayout *pHorizontalLayout = new QHBoxLayout;
	pDataLayout->addLayout(pHorizontalLayout);

	QPushButton *pRefreshButton = new QPushButton(tr("Refresh"));
	pHorizontalLayout->addWidget(pRefreshButton);

	QLabel *filterLabel = new QLabel;
	filterLabel->setText(tr("Filter:"));
	pHorizontalLayout->addWidget(filterLabel);

	QLineEdit *pEntryFilter = new QLineEdit;
	pHorizontalLayout->addWidget(pEntryFilter);

	QPushButton *pClearButton = new QPushButton(tr("Clear"));
	pHorizontalLayout->addWidget(pClearButton);

	QTabWidget *pQueryResults = new QTabWidget;
	pQueryResults->setTabPosition(QTabWidget::East);
	pDataLayout->addWidget(pQueryResults);

	QWidget *pQueryResultsTab1 = new QWidget;
	pQueryResults->addTab(pQueryResultsTab1, "Results");

	QVBoxLayout *pResultsLayout = new QVBoxLayout;
	pQueryResultsTab1->setLayout(pResultsLayout);

	QTableView *pResultsTable = new QTableView;
	pResultsLayout->addWidget(pResultsTable); //TODO Improve table

	QWidget *pQueryResultsTab2 = new QWidget;
	pQueryResults->addTab(pQueryResultsTab2, "Console");

	QVBoxLayout *pConsoleLayout = new QVBoxLayout;
	pQueryResultsTab2->setLayout(pConsoleLayout);

	QTextEdit *pConsoleTextView = new QTextEdit;
	pConsoleLayout->addWidget(pConsoleTextView);

	return pTableTab2;
}

QWidget* QDatabaseTableView::makeCreationScriptTab()
{
	QVBoxLayout *pCreationScriptLayout = new QVBoxLayout;
	QWidget *pTableTab3 = new QWidget;
	pTableTab3->setLayout(pCreationScriptLayout);

	QTextEdit *pCreationScriptTextView = new QTextEdit;
	pCreationScriptLayout->addWidget(pCreationScriptTextView);

	return pTableTab3;
}
