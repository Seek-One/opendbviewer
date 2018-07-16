/*
 * DatabaseController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "DatabaseController.h"

DatabaseController::DatabaseController(const DatabaseModel&  databaseModel)
{
	m_szFilename = databaseModel.getDatabaseName();
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
		QStringList listItem = m_db.tables(QSql::Tables);
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

	QStringList listRowHeader;
	QStringList listRowData;


	bRes = openDatabase();
	if(bRes){
		QString szQuery = loadTableDescriptionQuery(szTableName);

		int iCount = 0;

		QSqlQuery query(m_db);
		bRes = query.exec(szQuery);//loading the query according to the type of database used
		if(bRes){
			// Get headers
			listRowHeader = loadTableDescriptionColumnNames(query);
			func(listRowHeader, listRowData, DBQueryStepStart, user_data);

			// Get rows
			while (query.next())
			{
				listRowData = loadTableDescriptionResult(query);
				func(listRowHeader, listRowData, DBQueryStepRow, user_data);
				listRowData.clear();

				iCount++;
			}

			// Tell all data are read
			func(listRowHeader, listRowData, DBQueryStepEnd, user_data);
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

	QStringList listRowHeader;
	QStringList listRowData;

	bRes = openDatabase();
	if(bRes){
		// Get the list of column names from the table
		listRowHeader = listColumnNames(szTableName);
		func(listRowHeader, listRowData, DBQueryStepStart, user_data);

		// Select with all column
		QString szHeaders = listRowHeader.join(", ");
		QString szQuery = "SELECT "+szHeaders+" FROM "+szTableName;
		// Add filter if any
		if(szFilter.isEmpty() == false){
			szQuery += " WHERE "+szFilter;
		}

		int iCount = 0;

		int iColumnCount = listRowHeader.size();
		int col;

		// Execute the query
		QSqlQuery query(m_db);
		bRes = query.exec(szQuery);
		if(bRes){
			QVariant variant;

			// Get rows
			while(query.next())
			{
				for (col=0; col<iColumnCount; col++)
				{
					variant = query.value(col);
					listRowData << variant.toString();
				}
				func(listRowHeader, listRowData, DBQueryStepRow, user_data);
				listRowData.clear();
				iCount++;
			}
		}

		func(listRowHeader, listRowData, DBQueryStepEnd, user_data);

		m_szResultString = makeQueryResultString(query, iCount);

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for table description loading");
	}

	return bRes;
}

bool DatabaseController::loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data)
{
	bool bRes = true;

	QString szQuery = loadTableCreationScriptQuery(szTableName);

	if(!szQuery.isEmpty()){
		bRes = openDatabase();
		if(bRes){

			QSqlQuery query(m_db);
			bRes = query.exec(szQuery);

			QString szTmp;

			while(query.next())
			{
				szTmp = makeTableCreationScriptQueryResult(query);
				func(szTmp, user_data);
			}

			closeDataBase();
		}else{
			qCritical("[DatabaseController] Cannot open database for table creation script loading");
		}
	}

	return bRes;
}

bool DatabaseController::loadWorksheetQueryResults(QString& szWorksheetQuery, DbLoadWorksheetQueryResults func, void* user_data)
{
	bool bRes;

	QStringList listRowHeader;
	QStringList listRowData;

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

			func(listRowHeader, listRowData, DBQueryStepStart, user_data);

			while(query.next())
			{
				int currentColumnNumber;
				for (currentColumnNumber = 0; currentColumnNumber < query.record().count(); currentColumnNumber++)
				{
					variant = query.value(currentColumnNumber);
					listRowData << variant.toString();
				}

				func(listRowHeader, listRowData, DBQueryStepRow, user_data);
				//Clearing pRowData to have an empty list when starting the while loop again
				listRowData.clear();
				iCount++;
			}

			func(listRowHeader, listRowData, DBQueryStepEnd, user_data);
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
