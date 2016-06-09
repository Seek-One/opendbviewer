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

	//Creation of the connection tab widget
	QTabWidget *pPage1 = new QTabWidget(this);
	pMainLayout->addWidget(pPage1);

	QHBoxLayout *pDatabaseConnectionLayout = new QHBoxLayout(); //Creates a layout to be used for widgets in the connection tab

	QSplitter *pDatabaseConnectionSplitter = new QSplitter();
	pDatabaseConnectionSplitter->setChildrenCollapsible(false);
	pDatabaseConnectionLayout->addWidget(pDatabaseConnectionSplitter);

	pPage1->setLayout(pDatabaseConnectionLayout);

	QWidget *pVertConnectionPannel = new QWidget;
	pVertConnectionPannel = makeVerticalConnectionPannel();
	pDatabaseConnectionSplitter->addWidget(pVertConnectionPannel);

	QTabWidget *pTabsInConnection = new QTabWidget(pPage1);//Used to create a tab widget in the opened connection tab
	pTabsInConnection->setTabsClosable(true);

	//Creation of the first tab called Worksheet
	QWidget *pTab1 = makeWorksheetTab();
	pTabsInConnection->addTab(pTab1, "worksheet");

	//Creation of the second tab called Channels
	m_pTab2 = new QWidget;
	m_pTab2 = makeChannelTab();
	pTabsInConnection->addTab(m_pTab2, "channels");

	pDatabaseConnectionSplitter->addWidget(pTabsInConnection);
}

QDatabaseConnectionView::~QDatabaseConnectionView()
{

}

QWidget* QDatabaseConnectionView::makeWorksheetTab()
{
	QWidget *pTab1 = new QWidget();
	QVBoxLayout *pWorksheetTabLayout = new QVBoxLayout();
	pTab1->setLayout(pWorksheetTabLayout);

	QDatabaseWorksheetView* pWorksheetView = new QDatabaseWorksheetView(this);
	pWorksheetTabLayout->addWidget(pWorksheetView);

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

QWidget* QDatabaseConnectionView::makeStructureTable()
{
	QStandardItemModel *pStructureModel = new QStandardItemModel();
	QStandardItem *pStructureItem = new QStandardItem("Structure");
	pStructureModel->setHorizontalHeaderItem(0, pStructureItem);
	//QStandardItem *pTableItem = new QStandardItem("Tables");
	//pStructureModel->appendRow(pTableItem);

	//QStandardItem *pSystemTablesItem = new QStandardItem("System tables");
	//pStructureModel->appendRow(pSystemTablesItem);

	QTreeView *pStructureView = new QTreeView();
	pStructureView->setModel(pStructureModel);
	//pStructureView->header()->hide();

	return pStructureView;
}

QWidget* QDatabaseConnectionView::makeVerticalConnectionPannel()
{
	QWidget *pVertConnectionPannel = new QWidget;

	QVBoxLayout *pVertConnectionPannelLayout = new QVBoxLayout;
	pVertConnectionPannel->setLayout(pVertConnectionPannelLayout);

	QToolBar *pOptionButtonsToolbar = new QToolBar();
	pOptionButtonsToolbar = makeOptionButtonsToolBar();
	pVertConnectionPannelLayout->addWidget(pOptionButtonsToolbar);

	QWidget *pStructureView = new QWidget();
	pStructureView = makeStructureTable();
	pVertConnectionPannelLayout->addWidget(pStructureView);

	return pVertConnectionPannel;
}

QToolBar* QDatabaseConnectionView::makeOptionButtonsToolBar()
{
	QToolBar *pOptionButtonsToolbar = new QToolBar;

	QPushButton *pRefresh = new QPushButton("Refresh", this); //TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pRefresh);

	QPushButton *pNewWorksheet = new QPushButton("New", this);//TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pNewWorksheet);

	return pOptionButtonsToolbar;
}
