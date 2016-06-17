/*
 * QDatabaseTableView.cpp
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#include "QDatabaseTableView.h"
#include <QSqlQueryModel>
#include <QDebug>

QDatabaseTableView::QDatabaseTableView(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout *pTablePanelLayout = new QVBoxLayout();
	setLayout(pTablePanelLayout);

	//Adds a tab widget
	QTabWidget *pTablePanelTabs = new QTabWidget(this);
	pTablePanelLayout->addWidget(pTablePanelTabs);

	//Adds the first tab "Structure" to the widget
	QWidget *pTableTab1 = new QWidget();
	pTableTab1 = makeStructureTab();
	pTablePanelTabs->addTab(pTableTab1, "Structure");

	//Adds the second tab "Data" to the widget
	QWidget *pTableTab2 = new QWidget();
	pTableTab2 = makeDataTab();
	pTablePanelTabs->addTab(pTableTab2, "Data");

	//Adds the third tab "SQL creation script" to the widget
	QWidget *pTableTab3 = new QWidget();
	pTableTab3 = makeCreationScriptTab();
	pTablePanelTabs->addTab(pTableTab3, "Original SQL creation script");
}

QDatabaseTableView::~QDatabaseTableView()
{

}

QWidget* QDatabaseTableView::makeStructureTab()
{
	//Sets the layout for the structure tab
	QVBoxLayout *pStructureLayout = new QVBoxLayout();
	QWidget *pTableTab1 = new QWidget;
	pTableTab1->setLayout(pStructureLayout);

	m_pStructureTable = new QTableView();
	pStructureLayout->addWidget(m_pStructureTable);
	m_pStructureModel = new QStandardItemModel();

	//Setting the header
	QStandardItem *pHeader1 = new QStandardItem(tr("Field"));
	m_pStructureModel->setHorizontalHeaderItem(0, pHeader1);
	QStandardItem *pHeader2 = new QStandardItem(tr("Type"));
	m_pStructureModel->setHorizontalHeaderItem(1, pHeader2);
	QStandardItem *pHeader3 = new QStandardItem(tr("Not null"));
	m_pStructureModel->setHorizontalHeaderItem(2, pHeader3);
	QStandardItem *pHeader4 = new QStandardItem(tr("Default value"));
	m_pStructureModel->setHorizontalHeaderItem(3, pHeader4);
	QStandardItem *pHeader5 = new QStandardItem(tr("Primary key"));
	m_pStructureModel->setHorizontalHeaderItem(4, pHeader5);

	//Setting the model
	m_pStructureTable->setModel(m_pStructureModel);

	return pTableTab1;
}

QWidget* QDatabaseTableView::makeDataTab()
{
	//Sets the layout for the data tab
	QVBoxLayout *pDataLayout = new QVBoxLayout();
	QWidget *pTableTab2 = new QWidget();
	pTableTab2->setLayout(pDataLayout);

	QHBoxLayout *pHorizontalLayout = new QHBoxLayout();
	pDataLayout->addLayout(pHorizontalLayout);

	QPushButton *pRefreshButton = new QPushButton(tr("Refresh"));
	pHorizontalLayout->addWidget(pRefreshButton);

	QLabel *filterLabel = new QLabel();
	filterLabel->setText(tr("Filter:"));
	pHorizontalLayout->addWidget(filterLabel);

	QLineEdit *pEntryFilter = new QLineEdit();
	pHorizontalLayout->addWidget(pEntryFilter);

	QPushButton *pClearButton = new QPushButton(tr("Clear"));
	pHorizontalLayout->addWidget(pClearButton);

	QTabWidget *pQueryResults = new QTabWidget();
	pQueryResults->setTabPosition(QTabWidget::East);
	pDataLayout->addWidget(pQueryResults);

	QWidget *pQueryResultsTab1 = new QWidget();
	pQueryResults->addTab(pQueryResultsTab1, "Results");

	QVBoxLayout *pResultsLayout = new QVBoxLayout();
	pQueryResultsTab1->setLayout(pResultsLayout);

	QTableView *pResultsTable = new QTableView();
	pResultsLayout->addWidget(pResultsTable); //TODO Improve table

	QWidget *pQueryResultsTab2 = new QWidget();
	pQueryResults->addTab(pQueryResultsTab2, "Console");

	QVBoxLayout *pConsoleLayout = new QVBoxLayout();
	pQueryResultsTab2->setLayout(pConsoleLayout);

	QTextEdit *pConsoleTextView = new QTextEdit();
	pConsoleLayout->addWidget(pConsoleTextView);

	return pTableTab2;
}

QWidget* QDatabaseTableView::makeCreationScriptTab()
{
	//Sets the layout for the creation script tab
	QVBoxLayout *pCreationScriptLayout = new QVBoxLayout();
	QWidget *pTableTab3 = new QWidget();
	pTableTab3->setLayout(pCreationScriptLayout);

	QTextEdit *pCreationScriptTextView = new QTextEdit();
	pCreationScriptLayout->addWidget(pCreationScriptTextView);

	return pTableTab3;
}

QStandardItemModel* QDatabaseTableView::getStructureModel() const
{
	return m_pStructureModel;
}


QTableView* QDatabaseTableView::getStructureTable() const
{
	return m_pStructureTable;
}

