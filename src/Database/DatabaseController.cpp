/*
 * DatabaseController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "Database/DatabaseController.h"
#include "GUIController/QDatabaseSelectionViewController.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUIController/QDatabaseConnectionViewController.h"

#include <QSqlDatabase>
#include <QListView>
#include <QDebug>
#include <QSqlError>

DatabaseController::DatabaseController(const QString& szFilename)
{
	m_szFilename = szFilename;

	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(m_szFilename);

}

DatabaseController::~DatabaseController()
{

}

bool DatabaseController::openDatabase()
{
	return m_db.open();
}

void DatabaseController::closeDataBase()
{
	m_db.close();
}

bool DatabaseController::loadTables(DbLoadTableCB func, void* user_data)
{
	qDebug("Test");

	if(openDatabase()){

		QList<QString> m_tableList = m_db.tables();
		QList<QString>::const_iterator iter = m_tableList.begin();
		while(iter != m_tableList.end())
		{
			if(func){
				func(*iter, user_data);
			}
			iter++;
		}

		closeDataBase();
	}

	/*

	//Transforms QList<QString> to QStandardItems and adds them to the table
	QList<QString>::iterator iterator = m_tableList.begin();
	while(iterator != m_tableList.end())
	{
		QString listItemString;
		iterator->append(&listItemString);
		QStandardItem *pListItem = new QStandardItem(listItemString);
		QDatabaseConnectionView *pConnectionView = new QDatabaseConnectionView;
		pConnectionView->getTableItem()->appendRow(pListItem);
		iterator++;
	}*/

	return true;
}

QList<QString> DatabaseController::getTableList() const
{
	return m_tableList;
}
