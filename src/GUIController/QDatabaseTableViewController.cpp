/*
 * QDatabaseTableViewController.cpp
 *
 *  Created on: 14 juin 2016
 *      Author: echopin
 */

#include <GUIController/QDatabaseTableViewController.h>
#include <GUI/QDatabaseTableView.h>

QDatabaseTableViewController::QDatabaseTableViewController()
{
	m_pDatabaseTableView = NULL;
	m_szTableName = "";
	m_pDatabaseController = NULL;
}

QDatabaseTableViewController::~QDatabaseTableViewController()
{

}

void QDatabaseTableViewController::init(QDatabaseTableView* pDatabaseTableView, QString& szTableName, DatabaseController* pDatabaseController)
{
	m_pDatabaseTableView = pDatabaseTableView;
	m_szTableName = szTableName;
	m_pDatabaseController = pDatabaseController;

	m_pDatabaseController->loadTableDescription(m_szTableName, onDbLoadTableDescription, this);
}


void QDatabaseTableViewController::onDbLoadTableDescription(const QString& szName, const QString& szType, bool bNotNull, const QString& szDefaultValue, const QString& szPk, void* user_data)
{
	QDatabaseTableViewController* pDatabaseTableViewController = (QDatabaseTableViewController*)(user_data);

	//Creates the items that are to go into the different columns
	QStandardItem *pNameItem = new QStandardItem(szName);
	pNameItem->setEditable(false);
	QStandardItem *pTypeItem = new QStandardItem(szType);
	pTypeItem->setEditable(false);

	//Workaround for bNotNull not being displayed in table
	QString bNotNullString;
	if (bNotNull == false)
	{
		bNotNullString = "false";
	}
	else
	{
		bNotNullString = "true";
	}
	QStandardItem *pNotNullItem = new QStandardItem(bNotNullString);
	pNotNullItem->setEditable(false);
	QStandardItem *pDefaultValue = new QStandardItem(szDefaultValue);
	pDefaultValue->setEditable(false);
	QStandardItem *pPkItem = new QStandardItem(szPk);
	pPkItem->setEditable(false);

	//Creates a list of QStandardItems to be given to the structure model
	QList<QStandardItem*> tableItemList;
	tableItemList << pNameItem << pTypeItem << pNotNullItem << pDefaultValue << pPkItem;

	//Adds a row to the table
	pDatabaseTableViewController->m_pDatabaseTableView->getStructureModel()->appendRow(tableItemList);

}
