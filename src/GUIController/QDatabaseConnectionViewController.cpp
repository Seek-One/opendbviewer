/*
 * QDatabaseConnectionViewController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionViewController.h"
#include "QDatabaseTableViewController.h"
#include "QDatabaseWorksheetViewController.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "GUI/QDatabaseTableView.h"
#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerSqlite.h"
#include "Database/DatabaseControllerMysql.h"

QDatabaseConnectionViewController::QDatabaseConnectionViewController(const QString& szFileName, DatabaseController* pDatabaseController)
{
	m_szFileName = szFileName;
	m_pDatabaseConnectionView = NULL;
	m_pDatabaseController = pDatabaseController;
	m_pListTableModel = new QStandardItemModel();
}

QDatabaseConnectionViewController::~QDatabaseConnectionViewController()
{

}

void QDatabaseConnectionViewController::init(QDatabaseConnectionView* pDatabaseConnectionView, QStringList& szDatabaseInfoList)
{
	m_pDatabaseConnectionView = pDatabaseConnectionView;

	connect(m_pDatabaseConnectionView->getNewWorksheetButton(), SIGNAL(clicked()), this, SLOT(openNewWorksheet()));
	connect(m_pDatabaseConnectionView->getRefreshTableListButton(), SIGNAL(clicked()), this, SLOT(refreshDatabaseTables()));
	connect(m_pDatabaseConnectionView->getTabsInConnection(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(m_pDatabaseConnectionView->getTableTreeView(), SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(openTableTab(const QModelIndex&)));

	m_pDatabaseConnectionView->setTablesModel(m_pListTableModel);

	openNewWorksheet();
}

void QDatabaseConnectionViewController::openNewWorksheet()
{
	QDatabaseWorksheetView* pDatabaseWorksheetView = new QDatabaseWorksheetView(m_pDatabaseConnectionView);
	m_pDatabaseConnectionView->addWorksheetView(pDatabaseWorksheetView, tr("Worksheet"));

	QDatabaseWorksheetViewController* pDatabaseWorksheetViewController = new QDatabaseWorksheetViewController;
	pDatabaseWorksheetViewController->init(pDatabaseWorksheetView, m_szFileName, m_pDatabaseController);
}

void QDatabaseConnectionViewController::refreshDatabaseTables()
{
	bool bRes;

	m_pDatabaseConnectionView->getTableItem()->removeRows(0, m_pDatabaseConnectionView->getTableItem()->rowCount());
	m_pDatabaseConnectionView->getStructureTableItem()->removeRows(0, m_pDatabaseConnectionView->getStructureTableItem()->rowCount());
	m_pDatabaseConnectionView->getViewTableItem()->removeRows(0, m_pDatabaseConnectionView->getViewTableItem()->rowCount());

	bRes = loadDatabaseTables();
	if(!bRes){
		QMessageBox::warning(m_pDatabaseConnectionView, tr("Database problem"), tr("Problem while loading database tables"));
	}
}

void QDatabaseConnectionViewController::openTableTab(const QModelIndex& index)
{
	if (index == m_pDatabaseConnectionView->getTableItem()->index() || index == m_pDatabaseConnectionView->getStructureTableItem()->index() || index == m_pDatabaseConnectionView->getViewTableItem()->index())
	{
		// Prevents the tab from opening if the user clicks on the "tables", "system tables" or "views" item
		return;
	}

	QStandardItem *pTableItem = m_pListTableModel->itemFromIndex(index);
	QString szTableName = pTableItem->text();

	int iDatabaseTableViewIdx = 0;
	QDatabaseTableView* pDatabaseTableView = new QDatabaseTableView();
	QDatabaseTableViewController* pDatabaseTableViewController = new QDatabaseTableViewController();
	pDatabaseTableViewController->init(pDatabaseTableView, szTableName, m_pDatabaseController);


	iDatabaseTableViewIdx = m_pDatabaseConnectionView->addTableView(pDatabaseTableView, szTableName);
	m_pDatabaseConnectionView->switchCurrentTableView(iDatabaseTableViewIdx);

	if(!pDatabaseTableViewController->loadDatabaseTableInfos()){
		QMessageBox::warning(m_pDatabaseConnectionView, tr("Database problem"), tr("Problem while loading database table informations"));
	}

	// Controller will be deleted when the view is destroyed
	connect(pDatabaseTableView, SIGNAL(destroyed(QObject*)), pDatabaseTableViewController, SLOT(deleteLater()));

}

void QDatabaseConnectionViewController::closeTab(const int& index)
{
	QWidget* tabItem = m_pDatabaseConnectionView->getTabsInConnection()->widget(index);
	m_pDatabaseConnectionView->getTabsInConnection()->removeTab(index);

	delete(tabItem);
}

bool QDatabaseConnectionViewController::loadDatabaseTables()
{
	bool bRes;

	bRes = m_pDatabaseController->loadTables(onDbLoadTables, this);
	bRes = m_pDatabaseController->loadSystemTables(onDbLoadSystemTables, this) && bRes;
	bRes = m_pDatabaseController->loadViewsTables(onDbLoadViewsTables, this) && bRes;

	return bRes;
}

void QDatabaseConnectionViewController::onDbLoadTables(const QString& szTable, void* user_data)
{
	QDatabaseConnectionViewController* pController = (QDatabaseConnectionViewController*)(user_data);
	QStandardItem *pTableItem = new QStandardItem(szTable);
	pController->m_pDatabaseConnectionView->getTableItem()->appendRow(pTableItem);
	pTableItem->setEditable(false);
	pTableItem->setSelectable(true);
}

void QDatabaseConnectionViewController::onDbLoadSystemTables(const QString& szTable, void* user_data)
{
	QDatabaseConnectionViewController* pController = (QDatabaseConnectionViewController*)(user_data);

	QStandardItem *pStructureTableItem = new QStandardItem(szTable);
	pController->m_pDatabaseConnectionView->getStructureTableItem()->appendRow(pStructureTableItem);
	pStructureTableItem->setEditable(false);
	pStructureTableItem->setSelectable(true);
}

void QDatabaseConnectionViewController::onDbLoadViewsTables(const QString& szTable, void* user_data)
{
	QDatabaseConnectionViewController* pController = (QDatabaseConnectionViewController*)(user_data);
	QStandardItem *pViewTableItem = new QStandardItem(szTable);
	pController->m_pDatabaseConnectionView->getViewTableItem()->appendRow(pViewTableItem);
	pViewTableItem->setEditable(false);
	pViewTableItem->setSelectable(true);
}
