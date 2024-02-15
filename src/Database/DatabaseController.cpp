/*
 * DatabaseController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QElapsedTimer>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

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
		qDebug("[DatabaseController] Error: %s", qPrintable(m_db.lastError().text()));
	}
	return bRet;
}

void DatabaseController::closeDataBase()
{
	m_db.close();
}

QSqlDatabase DatabaseController::getSqlDatabase()
{
	return m_db;
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

	QElapsedTimer timerQuery;
	timerQuery.start();

	bRes = openDatabase();
	if(bRes){
		QString szQuery = loadTableDescriptionQuery(szTableName);

		int iCount = 0;

		QSqlQuery query(m_db);
		bRes = query.exec(szQuery); // Loading the query according to the type of database used
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

		m_szResultString = makeQueryResultString(query, timerQuery.elapsed(), iCount);

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for table description loading");
	}

	return bRes;
}

bool DatabaseController::loadTableData(const QString& szTableName, const QString& szFilter, QSqlDisplayTableModel* pTableModel)
{
	bool bRes;

	QStringList listRowHeader;

	bRes = openDatabase();
	if(bRes){
		listRowHeader = listColumnNames(szTableName);

		pTableModel->setTable(szTableName);
		pTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

		if (!szFilter.isEmpty()) {
			pTableModel->setFilter(szFilter);
		}

		//Set the headers
		for (int i = 0; i < listRowHeader.size(); i++) {
			pTableModel->setHeaderData(i , Qt::Horizontal, listRowHeader.at(i), Qt::TextAlignmentRole);
		}
		bRes = pTableModel->select();

	} else {
		qCritical("[DatabaseController] Cannot open database for table data loading");
	}

	return bRes;
}

bool DatabaseController::loadWorksheetQueryResults(QString& szWorksheetQuery, QSqlDisplayQueryModel** ppQueryModel)
{
	bool bRes;

	QStringList listRowHeader;
	QStringList listRowData;
	QString szRequest;

	QElapsedTimer timerQuery;
	timerQuery.start();

	bRes = openDatabase();
	if(bRes && !szWorksheetQuery.isEmpty()){

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QRegularExpression regExp("\\s+");
#else
		QRegExp regExp("\\s+");
#endif
		szRequest = szWorksheetQuery.section(regExp, 0, 0, QString::SectionSkipEmpty);
		if (szRequest.toLower() == "select") {
			*ppQueryModel = new QSqlDisplayQueryModel();
			(*ppQueryModel)->setQuery(szWorksheetQuery, m_db);
			if ((*ppQueryModel)->query().lastError().isValid()) {
				bRes = false;
			}
			const QSqlQuery& query = (*ppQueryModel)->query();
			m_szResultString = makeQueryResultString(query, timerQuery.elapsed());
		} else {
			QSqlQuery query(m_db);
			query.exec(szWorksheetQuery);
			bRes = false; 	//No results to display in this case
			m_szResultString = makeQueryResultString(query, timerQuery.elapsed());
		}

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

QString DatabaseController::makeQueryResultString(const QSqlQuery& query, qint64 iTimeMS, int iNbRowsSelected)
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
		szResultString += tr("Query executed with error(s) (%0):").arg(query.lastError().text());
		szResultString += "\n";
	}else{
		szResultString += tr("Query executed successfully in %0 ms:").arg(QString::number(iTimeMS));
		// Write time
		szResultString += " ";
	}
	// Write number of rows
	szResultString += tr("%0 row(s) selected/affected").arg(QString::number(iNbRow));
	szResultString += "\n";
	// Write query
	if(!query.lastQuery().isEmpty()){
		szResultString += query.lastQuery() + "\n";
	}
	szResultString += "\n";

	return szResultString;
}

QString DatabaseController::getQueryResultString() const
{
	return m_szResultString;
}

QString DatabaseController::getDatabaseName() const
{
	return m_db.databaseName();
}
