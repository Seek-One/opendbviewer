/*
 * QDatabaseConnectionView.cpp
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionView.h"
#include "GUIController/QWindowMainController.h"
#include "Database/DatabaseController.h"

#include <QHeaderView>

QDatabaseConnectionView::QDatabaseConnectionView(QWidget* parent)
		: QWidget(parent)
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	//Creation of the connection tab widget
	QTabWidget *pPage1 = new QTabWidget(this);
	pMainLayout->addWidget(pPage1);

	QHBoxLayout *pDatabaseConnectionLayout = new QHBoxLayout(); //Creates a layout to be used for widgets in the connection tab
	pPage1->setLayout(pDatabaseConnectionLayout);

	QSplitter *pDatabaseConnectionSplitter = new QSplitter();
	pDatabaseConnectionSplitter->setChildrenCollapsible(false);
	pDatabaseConnectionLayout->addWidget(pDatabaseConnectionSplitter);

	QWidget *pVertConnectionPanel = new QWidget;
	pVertConnectionPanel = makeVerticalConnectionPanel();
	pVertConnectionPanel->setMaximumWidth(300);
	pDatabaseConnectionSplitter->addWidget(pVertConnectionPanel);

	m_pTabsInConnection = new QTabWidget();//Used to create a tab widget in the opened connection tab
	m_pTabsInConnection->setTabsClosable(true);
	m_pTabsInConnection->setMinimumSize(600, 400);

	pDatabaseConnectionSplitter->addWidget(m_pTabsInConnection);
}

QDatabaseConnectionView::~QDatabaseConnectionView()
{

}

void QDatabaseConnectionView::addWorksheetView(QDatabaseWorksheetView* pDatabaseWorksheetView, const QString& szTitle)
{
	m_pTabsInConnection->addTab(pDatabaseWorksheetView, szTitle);
}

void QDatabaseConnectionView::addTableView(QDatabaseTableView* pDatabaseTableView, const QString& szTableTitle)
{
	m_pTabsInConnection->addTab(pDatabaseTableView, szTableTitle);
}

QWidget* QDatabaseConnectionView::makeWorksheetTab()
{
	m_pTab1 = new QWidget();
	QVBoxLayout *pWorksheetTabLayout = new QVBoxLayout();
	m_pTab1->setLayout(pWorksheetTabLayout);

	QDatabaseWorksheetView* pWorksheetView = new QDatabaseWorksheetView(this);
	pWorksheetTabLayout->addWidget(pWorksheetView);

	return m_pTab1;
}


QWidget* QDatabaseConnectionView::makeVerticalConnectionPanel()
{
	QWidget *pVertConnectionPanel = new QWidget;

	QVBoxLayout *pVertConnectionPanelLayout = new QVBoxLayout;
	pVertConnectionPanel->setLayout(pVertConnectionPanelLayout);

	QToolBar *pOptionButtonsToolbar = new QToolBar();
	pOptionButtonsToolbar = makeOptionButtonsToolBar();
	pVertConnectionPanelLayout->addWidget(pOptionButtonsToolbar);

	m_pTableTreeView = new QTreeView();
	pVertConnectionPanelLayout->addWidget(m_pTableTreeView);

	return pVertConnectionPanel;
}

QToolBar* QDatabaseConnectionView::makeOptionButtonsToolBar()
{
	QToolBar *pOptionButtonsToolbar = new QToolBar;

	m_pRefreshTableListButton = new QPushButton("Refresh", this); //TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(m_pRefreshTableListButton);

	m_pNewWorksheetButton= new QPushButton("New", this);//TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(m_pNewWorksheetButton);

	return pOptionButtonsToolbar;
}

void QDatabaseConnectionView::setTablesModel(QStandardItemModel* pModel)
{
	m_pTableTreeView->setModel(pModel);
	QStandardItem *pHeader = new QStandardItem("Structure");
	pModel->setHorizontalHeaderItem(0, pHeader);
	m_pTableTreeView->expandAll();

	//Creates the "tables" item in the tree view
	m_pTableItem = new QStandardItem("Tables");
	m_pTableItem->setEditable(false);
	pModel->appendRow(m_pTableItem);

	//Creates the "structure tables" item in the tree view
	m_pStructureTableItem = new QStandardItem("Structure tables");
	m_pStructureTableItem->setEditable(false);
	pModel->appendRow(m_pStructureTableItem);

	//Creates the "views" item in the tree view
	m_pViewItem = new QStandardItem("Views");
	m_pViewItem->setEditable(false);
	pModel->appendRow(m_pViewItem);
	m_pTableTreeView->expandAll();
}

QPushButton* QDatabaseConnectionView::getNewWorksheetButton() const
{
	return m_pNewWorksheetButton;
}

QPushButton* QDatabaseConnectionView::getRefreshTableListButton() const
{
	return m_pRefreshTableListButton;
}

QAction* QDatabaseConnectionView::getNewWorksheetAction() const
{
	return m_pNewWorksheetAction;
}

QTabWidget* QDatabaseConnectionView::getTabsInConnection() const
{
	return m_pTabsInConnection;
}

QWidget* QDatabaseConnectionView::getWorksheetTab() const
{
	return m_pTab1;
}

QTreeView* QDatabaseConnectionView::getTableTreeView() const
{
	return m_pTableTreeView;
}

QStandardItem* QDatabaseConnectionView::getTableItem() const
{
	return m_pTableItem;
}

QStandardItem* QDatabaseConnectionView::getStructureTableItem() const
{
	return m_pStructureTableItem;
}

QStandardItem* QDatabaseConnectionView::getViewTableItem() const
{
	return m_pViewItem;
}
