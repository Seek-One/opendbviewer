/*
 * DatabaseController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerSqlite.h"
#include "GUIController/QDatabaseSelectionViewController.h"
#include "GUIController/QDatabaseConnectionViewController.h"
#include "GUI/QDatabaseConnectionView.h"

DatabaseController::DatabaseController(const QString& szFilename)
{
	m_szFilename = szFilename;
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
	bool bRes;

	bRes = openDatabase();
	if(bRes){
		QStringList listItem = m_db.tables();

		// Sort by name
		listItem.sort();

		QList<QString>::const_iterator iter = listItem.begin();
		while(iter != listItem.end())
		{
			if(func){
				func(*iter, user_data);
			}
			iter++;
		}

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for table loading");
	}

	return bRes;
}

bool DatabaseController::loadSystemTables(DbLoadTableCB func, void* user_data)
{
	bool bRes;

	bRes = openDatabase();
	if(bRes){
		QStringList listItem = m_db.tables(QSql::SystemTables);

		// Sort by name
		listItem.sort();

		QList<QString>::const_iterator iter = listItem.begin();
		while(iter != listItem.end())
		{
			if(func){
				func(*iter, user_data);
			}
			iter++;
		}

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for system table loading");
	}

	return bRes;
}

bool DatabaseController::loadViewsTables(DbLoadTableCB func, void* user_data)
{
	bool bRes;

	bRes = openDatabase();
	if(bRes){
		QStringList listItem = m_db.tables(QSql::Views);

		// Sort by name
		listItem.sort();

		QList<QString>::const_iterator iter = listItem.begin();
		while(iter != listItem.end())
		{
			if(func){
				func(*iter, user_data);
			}
			iter++;
		}

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for views loading");
	}

	return bRes;
}

bool DatabaseController::loadTableDescription(const QString& szTableName, DbLoadTableDescription func, void* user_data)
{
	bool bRes;

	bRes = openDatabase();
	if(bRes){
		QString szQuery = loadTableDescriptionQuery(szTableName);

		int iCount = 0;

		QSqlQuery query(m_db);
		bRes = query.exec(szQuery);//loading the query according to the type of database used
		if(bRes){
			QStringList listRowHeader = loadTableDescriptionColumnNames(query);
			QStringList listRowData;

			while (query.next())
			{
				listRowData = loadTableDescriptionResult(query); //Loading results depending on type of database
				func(listRowHeader, listRowData, user_data);
				listRowData.clear();//Clearing pRowData to have an empty list when starting the while loop again

				iCount++;
			}
		}else{
			qCritical("[DatabaseController] Cannot execute query for table description loading");
		}

		QString szQueryOutput("Query executed successfully");
		m_szResultString = makeQueryResultString(query, iCount);

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for table description loading");
	}

	return bRes;
}

bool DatabaseController::loadTableData(const QString& szTableName, const QString& szFilter, DbLoadTableData func, void* user_data)
{
	bool bRes;

	bRes = openDatabase();
	if(bRes){
		// Get the list of column names from the table
		QStringList listRowHeader = listColumnNames(szTableName);
		QStringList listRowData;

		// Select with all column
		QString szHeaders = listRowHeader.join(", ");
		QString szQuery = "SELECT "+szHeaders+" FROM "+szTableName;
		// Add filter if any
		if(szFilter.isEmpty() == false){
			szQuery += " WHERE "+szFilter;
		}

		int iCount = 0;

		int iColumnCount;

		// Execute the query
		QSqlQuery query(m_db);
		bRes = query.exec(szQuery);
		if(bRes){
			QVariant variant;

			while(query.next())
			{
				for (iColumnCount = 0; iColumnCount <= (listRowHeader.size() - 1); iColumnCount++)
				{
					variant = query.value(iColumnCount);
					listRowData << variant.toString();
				}
				func(listRowHeader, listRowData, user_data);
				listRowData.clear();
				iCount++;
			}

			if(iCount == 0){
				func(listRowHeader, listRowData, user_data);
			}
		}

		m_szResultString = makeQueryResultString(query, iCount);

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for table description loading");
	}

	return bRes;
}

bool DatabaseController::loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data)
{
	openDatabase();

	QString szCreationScriptString;

	QSqlQuery tableCreationScriptQuery(m_db);
	tableCreationScriptQuery.exec(loadTableCreationScriptQuery(szTableName));//Loading the query according to the database

	while(tableCreationScriptQuery.next())
	{
		szCreationScriptString = makeTableCreationScriptQueryResult(tableCreationScriptQuery);//Getting the result according to the database
		func(szCreationScriptString, user_data);
	}

	closeDataBase();

	return true;
}

bool DatabaseController::loadWorksheetQueryResults(QString& szWorksheetQuery, DbLoadWorksheetQueryResults func, void* user_data)
{
	bool bRes = false;
	bRes = openDatabase();
	if(bRes){

		int iCount = 0;

		QSqlQuery query(m_db);
		bRes = query.exec(szWorksheetQuery);
		if(bRes){
			QVariant variant;

			QStringList listRowHeader;
			QStringList listRowData;

			int currentColumnNumber;
			//appending column names to columnNameList
			for (currentColumnNumber = 0; currentColumnNumber < query.record().count(); currentColumnNumber++)
			{
				QSqlField field = query.record().field(currentColumnNumber);
				listRowHeader << field.name();
			}

			while(query.next())
			{
				int currentColumnNumber;
				for (currentColumnNumber = 0; currentColumnNumber < query.record().count(); currentColumnNumber++)
				{
					variant = query.value(currentColumnNumber);
					listRowData << variant.toString();
				}

				func(listRowHeader, listRowData, user_data);
				//Clearing pRowData to have an empty list when starting the while loop again
				listRowData.clear();
				iCount++;
			}
		}
		m_szResultString = makeQueryResultString(query, iCount);

		closeDataBase();
	}

	return bRes;
}

QString DatabaseController::makeQueryResultString(const QSqlQuery& query, int iNbRowsSelected)
{
	QString szResultString;
	QTime time;

	int iNbRow = 0;
	if(query.isSelect()){
		iNbRow = query.size();
		if(iNbRow == -1){
			iNbRow = iNbRowsSelected;
		}
	}else{
		iNbRow = query.numRowsAffected();
	}

	// Write the time
	szResultString += time.currentTime().toString()+" => ";
	// Write sql error
	if(query.lastError().isValid()){
		szResultString += "Query executed with error(s) (" + query.lastError().text() + "): ";
	}else{
		szResultString += "Query executed successfully: ";
	}
	// Write number of rows
	szResultString += QString::number(iNbRow)+" row(s) selected/affected\n";
	// Write query
	if(!query.lastQuery().isEmpty()){
		szResultString+="   "+query.lastQuery()+"\n";
	}
	szResultString+="\n";

	return szResultString;
}

QString DatabaseController::getQueryResultString() const
{
	return m_szResultString;
}
