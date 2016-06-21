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
#include <QStringList>

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
		bool bNotNull = tableInfoQuery.value(3).toInt();
		QString szDefaultValue = tableInfoQuery.value(4).toString();
		QString szPk = tableInfoQuery.value(5).toString();
		func(szName, szType, bNotNull, szDefaultValue, szPk, user_data);
		m_szListColumnName += szName;
    }

	tableInfoQuery.finish();
	closeDataBase();

	return true;
}

bool DatabaseController::loadTableData(const QString& szTableName, DbLoadTableData func, void* user_data)
{
	openDatabase();

	QList<QString> pRowData;
	QStringList pColumnName;
	QString columnNameString;
	//pColumnName << "rowid";

	QList<QString>::const_iterator iter = m_szListColumnName.begin();
	while(iter != m_szListColumnName.end())
	{
		//Adding the item we are on to the column name list
		pColumnName.append(*iter);
		iter++;
	}

	//Converting the list to a string
	columnNameString = pColumnName.join(", ");
	QSqlQuery tableDataQuery("SELECT rowid as rowid, "+columnNameString+" FROM "+szTableName+" ORDER BY rowid;");
	tableDataQuery.exec();

	//if there is no data to get, get both pColumnName and empty pRowData for setting the header, and position the query back to the first record
	if (tableDataQuery.next() == false)
		func(pColumnName, pRowData, user_data);
		tableDataQuery.previous();

	while(tableDataQuery.next())
	{
		int currentColumnNumber;
		for (currentColumnNumber = 0; currentColumnNumber <= pColumnName.size(); currentColumnNumber++)
		{
			pRowData << tableDataQuery.value(currentColumnNumber).toString();
		}
		func(pColumnName, pRowData, user_data);
		//Clearing pRowData to have an empty list when starting the while loop again
		pRowData.clear();
	}

		//tableDataQuery.finish();

	qDebug() << "Column name string: " << columnNameString;
	m_szListColumnName.clear();

	closeDataBase();

	return true;
}
