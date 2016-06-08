/*
 * QDatabaseConnectionView.cpp
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionView.h"

QDatabaseConnectionView::QDatabaseConnectionView(QWidget* parent)
		: QWidget(parent)
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	QTabWidget *pPage1 = new QTabWidget(this);
	//pPage1->setTabsClosable(true);
	pMainLayout->addWidget(pPage1);

	//Creation of other layouts within the main layout
	QHBoxLayout *pDatabaseConnectionLayout = new QHBoxLayout(); //Creates a layout to be used for widgets in the file tab
	pPage1->setLayout(pDatabaseConnectionLayout);

	QVBoxLayout *pSecDatabaseConnectionLayout = new QVBoxLayout(); //Layout to be used within fileLayout to allow positionning of the widgets
	pDatabaseConnectionLayout->addLayout(pSecDatabaseConnectionLayout);

	QToolBar *pOptionButtonsToolbar = new QToolBar();
	pSecDatabaseConnectionLayout->addWidget(pOptionButtonsToolbar);

	QTabWidget *pTabsInConnection = new QTabWidget(pPage1);//Used to create tabs in the opened file tab
	QPushButton *pRefresh = new QPushButton("refresh", pTabsInConnection); //TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pRefresh);

	QPushButton *pAddFile = new QPushButton("add", pTabsInConnection);//TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pAddFile);

	//Creation of the "structure" Model
	QStandardItemModel *pStructureModel = new QStandardItemModel();
	QStandardItem *pTableItem = new QStandardItem("Tables");
	pStructureModel->appendRow(pTableItem);

	QStandardItem *pSystemTablesItem = new QStandardItem("System tables");
	pStructureModel->appendRow(pSystemTablesItem);

	//Creation of the view to be applied to the "structure" model
	QTreeView *pStructureView = new QTreeView();
	pStructureView->setModel(pStructureModel);
	pStructureView->header()->hide();
	pSecDatabaseConnectionLayout->addWidget(pStructureView);

	//Creation of the first tab called Worksheet
	QWidget *pTab1 = makeWorksheetTab();
	pTabsInConnection->addTab(pTab1, "worksheet");

	//Creation of the second tab called Channels
	m_pTab2 = new QWidget;
	m_pTab2 = makeChannelTab();
	pTabsInConnection->addTab(m_pTab2, "channels");

	pDatabaseConnectionLayout->addWidget(pTabsInConnection);
}

QDatabaseConnectionView::~QDatabaseConnectionView()
{

}

QWidget* QDatabaseConnectionView::makeWorksheetTab()
{
	QWidget *pTab1 = new QWidget();//Creates the first tab

	QVBoxLayout *pWorksheetTabLayout = new QVBoxLayout();
	QToolBar *pWorksheetToolbar = new QToolBar();
	pWorksheetTabLayout->addWidget(pWorksheetToolbar);

	QPushButton *pButton1 = new QPushButton("button 1", pTab1);
	pWorksheetToolbar->addWidget(pButton1);

	QPushButton *pButton2 = new QPushButton("button2", pTab1);
	pWorksheetToolbar->addWidget(pButton2);

	QPushButton *pButton3 = new QPushButton("button3", pTab1);
	pWorksheetToolbar->addWidget(pButton3);

	QTextEdit *pWorksheetTextEdit = new QTextEdit();
	pWorksheetTabLayout->addWidget(pWorksheetTextEdit);

	//Creation of the worksheet view
    QTableView *pWorksheetTableView = new QTableView();
    pWorksheetTableView->horizontalHeader()->hide();
	pWorksheetTabLayout->addWidget(pWorksheetTableView);

	pTab1->setLayout(pWorksheetTabLayout);

	return pTab1;
}


QWidget* QDatabaseConnectionView::makeChannelTab()
{
	QWidget *pTab2 = new QWidget();
	QVBoxLayout *pChannelTabLayout = new QVBoxLayout;//Layout to be used in the Channel tab
	pTab2->setLayout(pChannelTabLayout);

	QDatabaseTableView* tableView = new QDatabaseTableView(this);
	pChannelTabLayout->addWidget(tableView);

	return pTab2;
}

