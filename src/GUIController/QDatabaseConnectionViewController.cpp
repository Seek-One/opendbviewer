/*
 * QDatabaseConnectionViewController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include <QStandardItem>

#include "QDatabaseConnectionViewController.h"

#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "QDatabaseWorksheetViewController.h"
#include "Database/DatabaseController.h"

QDatabaseConnectionViewController::QDatabaseConnectionViewController(const QString& szFileName)
{
	m_szFileName = szFileName;
	m_pDatabaseConnectionView = NULL;

	m_pListTableModel = new QStandardItemModel();
}

QDatabaseConnectionViewController::~QDatabaseConnectionViewController()
{

}

void QDatabaseConnectionViewController::init(QDatabaseConnectionView* pDatabaseConnectionView)
{
	m_pDatabaseConnectionView = pDatabaseConnectionView;

	connect(m_pDatabaseConnectionView->getNewWorksheetButton(), SIGNAL(clicked()), this, SLOT(openWorksheet()));
	connect(m_pDatabaseConnectionView->getTabsInConnection(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	m_pDatabaseConnectionView->setTablesModel(m_pListTableModel);

	openWorksheet();
}

void QDatabaseConnectionViewController::openWorksheet()
{
	QDatabaseWorksheetView* pDatabaseWorksheetView = new QDatabaseWorksheetView(m_pDatabaseConnectionView);
	m_pDatabaseConnectionView->addWorksheetView(pDatabaseWorksheetView, tr("worksheet"));

	QDatabaseWorksheetViewController* pDatabaseWorksheetViewController = new QDatabaseWorksheetViewController;
	pDatabaseWorksheetViewController->init(pDatabaseWorksheetView);
}

void QDatabaseConnectionViewController::closeTab(const int& index)
{
	if(m_pDatabaseConnectionView->getTabsInConnection()->count() < 2)
	{
		return;
	}

	QWidget* tabItem = m_pDatabaseConnectionView->getTabsInConnection()->widget(index);
	m_pDatabaseConnectionView->getTabsInConnection()->removeTab(index);

	delete(tabItem);
}


void QDatabaseConnectionViewController::updateTables()
{
	DatabaseController* pDatabaseController = new DatabaseController(m_szFileName);
	pDatabaseController->loadTables(onDbLoadTable, this);
}

void QDatabaseConnectionViewController::onDbLoadTable(const QString& szTable, void* user_data)
{
	QDatabaseConnectionViewController* pController = (QDatabaseConnectionViewController*)(user_data);

	QStandardItem *pTableItem = new QStandardItem(szTable);
	pController->m_pListTableModel->appendRow(pTableItem);
	pTableItem->setEditable(false);

}
