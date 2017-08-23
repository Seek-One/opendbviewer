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
	if(openDatabase()){

		QList<QString> szTableList = m_db.tables();

		QList<QString>::const_iterator iter = szTableList.begin();
		while(iter != szTableList.end())
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

		QList<QString> szTableList = m_db.tables(QSql::SystemTables);

		QList<QString>::const_iterator iter = szTableList.begin();
		while(iter != szTableList.end())
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
	if(openDatabase())
	{
		QList<QString> szTableList = m_db.tables(QSql::Views);
		QList<QString>::const_iterator iter = szTableList.begin();
		while(iter != szTableList.end())
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
	QSqlQuery tableInfoQuery(m_db);
	tableInfoQuery.exec(loadTableDescriptionQuery(szTableName));//loading the query according to the type of database used
	QStringList pColumnName = loadTableDescriptionColumnNames(tableInfoQuery);

	QStringList pRowData;

	while (tableInfoQuery.next())
    {
		pRowData = loadTableDescriptionResult(tableInfoQuery); //Loading results depending on type of database
		func(pColumnName, pRowData, user_data);
		pRowData.clear();//Clearing pRowData to have an empty list when starting the while loop again
    }

	QString szQueryOutput("Query executed successfully");
	m_szResultString = makeQueryResultString(tableInfoQuery, szQueryOutput);
	tableInfoQuery.finish();

	closeDataBase();

	return true;
}

bool DatabaseController::loadTableData(const QString& szTableName, const QString& szFilter, DbLoadTableData func, void* user_data)
{
	openDatabase();

	QList<QString> pRowData;

	//Get the list of column names from the table
	QStringList pColumnName = listColumnNames(szTableName);

	//Creating a string from a list
	QString columnNamesString = pColumnName.join(", ");

	//Using the string in the query
	QSqlQuery tableDataQuery(m_db);
	QString szQuery = "SELECT "+columnNamesString+" FROM "+szTableName;
	QString szQueryOutput;
	try
		{
		if(szFilter.isEmpty() == false)//If there is no filter, execute query
		{
			szQuery += " WHERE "+szFilter;
		}
		tableDataQuery.exec(szQuery);
		if(tableDataQuery.exec(szQuery) == false) //If the query is not right, throw
			throw QString("Query executed with error");
		else
			szQueryOutput = "Query executed successfully";
	}
	catch(QString& szErrorString)
	{
		szQueryOutput = "Query executed with error(s)";
	}

	/*if there is no data to get, get both pColumnName and empty pRowData for setting the header,
	 * and set the position back to the first record*/
	if (tableDataQuery.next() == false){
		func(pColumnName, pRowData, user_data);
	}
	tableDataQuery.previous();

	while(tableDataQuery.next())
	{
		int currentColumnNumber;
		for (currentColumnNumber = 0; currentColumnNumber <= (pColumnName.size() - 1); currentColumnNumber++)
		{
			pRowData << tableDataQuery.value(currentColumnNumber).toString();
		}
		func(pColumnName, pRowData, user_data);
		//Clearing pRowData to have an empty list when starting the while loop again
		pRowData.clear();
	}
	m_szResultString = makeQueryResultString(tableDataQuery, szQueryOutput);
	tableDataQuery.finish();
	closeDataBase();

	return true;
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
		//Creates a query from the data given in the worksheet text edit
		QSqlQuery worksheetQuery(m_db);

		//Creating a string giving information about the success or failure of query
		QString szQueryOutput;

		bRes = worksheetQuery.exec(szWorksheetQuery);
		if(bRes){
			QList<QString> pRowData;
			QList<QString> pColumnNameList;

			int currentColumnNumber;
			//appending column names to columnNameList
			for (currentColumnNumber = 0; currentColumnNumber < worksheetQuery.record().count(); currentColumnNumber++)
			{
				QSqlField field = worksheetQuery.record().field(currentColumnNumber);
				pColumnNameList << field.name();
			}

			while(worksheetQuery.next())
			{
				int currentColumnNumber;
				for (currentColumnNumber = 0; currentColumnNumber < worksheetQuery.record().count(); currentColumnNumber++)
				{
					pRowData << worksheetQuery.value(currentColumnNumber).toString();
				}

				func(pColumnNameList, pRowData, user_data);
				//Clearing pRowData to have an empty list when starting the while loop again
				pRowData.clear();
			}
			szQueryOutput = ("Query executed successfully");
			m_szResultString = makeQueryResultString(worksheetQuery, szQueryOutput);
		}else{
			szQueryOutput = "Query executed with error(s): %s" + worksheetQuery.lastError().text();
			m_szResultString = makeQueryResultString(worksheetQuery, szQueryOutput);
		}

		closeDataBase();
	}

	return bRes;
}

QString DatabaseController::makeQueryResultString(QSqlQuery query, QString& szQueryOutput)
{
	QString szResultString("");//Creates an empty string
	QString szNumberOfRows = makeStringNumberOfRows(query);//Gets the number of lines in the query and converts it to string
	QTime time;
	//Creating the result string with query information
	if(query.lastQuery().isEmpty())//If the query is empty, do not append it to szResultString
		szResultString.append(time.currentTime().toString()+"=> "+szQueryOutput+": "+szNumberOfRows+" row(s) selected/affected\n\n");
	else
		szResultString.append(time.currentTime().toString()+"=> "+szQueryOutput+": "+szNumberOfRows+" row(s) selected/affected \n   "+query.lastQuery()+"\n\n");

	return szResultString;
}

QString DatabaseController::makeStringNumberOfRows(QSqlQuery query)
{
	//Gets the number of rows as numRowsAffected() does not seem to be working
	int numberOfRows = 0;

		if(query.last())
		{
		    numberOfRows =  query.at() + 1;
		    query.first();
		    query.previous();
		}
	//Converts the number to a string
	QString szNumberOfRows = QString::number(numberOfRows);

	return szNumberOfRows;
}

QString DatabaseController::getQueryResultString() const
{
	return m_szResultString;
}
