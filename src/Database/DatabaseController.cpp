/*
 * DatabaseController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "DatabaseController.h"

int DatabaseController::g_iConnectionIdentifier = 0;

DatabaseController::DatabaseController(const DatabaseModel&  databaseModel)
{
	m_databaseModel = databaseModel;
}

DatabaseController::~DatabaseController()
{

}

bool DatabaseController::openDatabase()
{
	bool bRet = m_db.open();
	if(!bRet){
		qDebug(qPrintable("[DatabaseController] Error: " + m_db.lastError().text()));
	}
	return bRet;
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
	} else {
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

bool DatabaseController::loadTableData(const QString& szTableName, const QString& szFilter, QSqlTableModel** ppTableModel)
{
	bool bRes;

	QStringList listRowHeader;

	bRes = openDatabase();
	if(bRes){
		listRowHeader = listColumnNames(szTableName);
		*ppTableModel = new QSqlTableModel(NULL, m_db);

		(*ppTableModel)->setTable(szTableName);
		(*ppTableModel)->setEditStrategy(QSqlTableModel::OnFieldChange);

		if (!szFilter.isEmpty()) {
			(*ppTableModel)->setFilter(szFilter);
		}

		//Set the headers
		for (int i = 0; i < listRowHeader.size(); i++) {
			(*ppTableModel)->setHeaderData(i , Qt::Horizontal, listRowHeader.at(i), Qt::TextAlignmentRole);
		}
		bRes = (*ppTableModel)->select();

	} else {
		qCritical("[DatabaseController] Cannot open database for table data loading");
	}

	return bRes;
}

bool DatabaseController::loadWorksheetQueryResults(QString& szWorksheetQuery, QSqlTableModel** ppTableModel)
{
	bool bRes;

	QStringList listRowHeader;
	QStringList listRowData;

	bRes = openDatabase();
	if(bRes){
		QSqlQuery query(m_db);
		if (!szWorksheetQuery.isEmpty()) {
			bRes = query.exec(szWorksheetQuery);
			if (bRes) {
				*ppTableModel = new QSqlTableModel(NULL, m_db);

				int iCurrentColumn;
				//Set Headers
				for (iCurrentColumn = 0; iCurrentColumn < query.record().count(); iCurrentColumn++)
				{
					QSqlField field = query.record().field(iCurrentColumn);
					(*ppTableModel)->setHeaderData(iCurrentColumn , Qt::Horizontal, field.name());
				}
				bRes = (*ppTableModel)->select();
			}
		}

		m_szResultString = makeQueryResultString(query);

		closeDataBase();
	} else {
		qCritical("[DatabaseController] Cannot open database for new query loading");
	}

	return bRes;
}

bool DatabaseController::loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data)
{
	bool bRes = true;

	QString szQuery = loadTableCreationScriptQuery(szTableName);

	if(!szQuery.isEmpty()){
		if(bRes){

			QSqlQuery query(m_db);
			bRes = query.exec(szQuery);

			QString szTmp;

			while(query.next())
			{
				szTmp = makeTableCreationScriptQueryResult(query);
				func(szTmp, user_data);
			}

		} else {
			qCritical("[DatabaseController] Cannot open database for table creation script loading");
		}
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
	} else {
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
		szResultString += "   " + query.lastQuery() + "\n";
	}
	szResultString += "\n";

	return szResultString;
}

QString DatabaseController::getQueryResultString() const
{
	return m_szResultString;
}
