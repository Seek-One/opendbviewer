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
#include <QSqlQuery>
#include <QString>
#include <QSqlRecord>
#include <QStandardItem>

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

	return true;
}

bool DatabaseController::loadSystemTables(DbLoadTableCB func, void* user_data)
{
	if(openDatabase()){

		QList<QString> m_tableList = m_db.tables(QSql::SystemTables);
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

	return true;
}

bool DatabaseController::loadViewsTables(DbLoadTableCB func, void* user_data)
{
	if(openDatabase()){

			QList<QString> tableList = m_db.tables(QSql::Views);
			QList<QString>::const_iterator iter = tableList.begin();
			while(iter != tableList.end())
			{
				if(func){
					func(*iter, user_data);
				}
				iter++;
			}

			closeDataBase();
		}
	return true;
}

bool DatabaseController::loadTableDescription(const QString& szTableName, DbLoadTableDescription func, void* user_data)
{
	openDatabase();
	QSqlQuery tableInfoQuery("PRAGMA table_info("+szTableName+");");
	tableInfoQuery.exec();

	while (tableInfoQuery.next())
    {
		QString szName = tableInfoQuery.value(1).toString();
		QString szType = tableInfoQuery.value(2).toString();
		bool bNotNull = tableInfoQuery.value(3).toBool();
		QString szDefaultValue = tableInfoQuery.value(4).toString();
		QString szPk = tableInfoQuery.value(5).toString();
		func(szName, szType, bNotNull, szDefaultValue, szPk, user_data);
    }

	tableInfoQuery.finish();
	closeDataBase();

	return true;
}

