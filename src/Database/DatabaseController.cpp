/*
 * DatabaseController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QElapsedTimer>
#include <QSqlDriver>

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

bool DatabaseController::loadTableDescription(const QString& szTableName, DbLoadTableDescription func, void* user_data, DatabaseProcessHandler* pHandler)
{
	bool bRes;

	QStringList listRowHeader;
	QStringList listRowData;

	QElapsedTimer timerQuery;
	timerQuery.start();

	bRes = openDatabase();
	if(bRes){
		QString szQuery = loadTableDescriptionQuery(szTableName);

		int iResultCount = 0;

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

				iResultCount++;
			}

			// Tell all data are read
			func(listRowHeader, listRowData, DBQueryStepEnd, user_data);
		}else{
			qCritical("[DatabaseController] Cannot execute query for table description loading");
		}

		const auto& szResultString = makeQueryResultString(query, timerQuery.elapsed(), iResultCount);
		if (pHandler) {
			pHandler->notifyQueryResult(szQuery, bRes, iResultCount, szResultString);
		}

		closeDataBase();
	}else{
		qCritical("[DatabaseController] Cannot open database for table description loading");
	}

	return bRes;
}

bool DatabaseController::loadTableData(const QString& szTableName, const QString& szFilter, DatabaseProcessHandler* pHandler)
{
	bool bRes;

	QStringList listRowHeader;

	QSqlDisplayTableModel* pTableModel = pHandler->getTableModel();

	bRes = openDatabase();
	if(bRes){
		QElapsedTimer timerQuery;
		timerQuery.start();

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


		QString szQuery = pTableModel->query().executedQuery();

		int iResultCount = 0;
		if(pTableModel->query().driver()->hasFeature(QSqlDriver::QuerySize)) {
			iResultCount = pTableModel->query().size();
		}else {
			// Re-execute the request to count the number of rows
			QSqlQuery queryCount(szQuery, m_db);
			bRes = queryCount.exec();
			if (bRes) {
				while (queryCount.next()) {
					iResultCount++;
				}
			}
		}

		QString szResultString = makeQueryResultString(pTableModel->query(), timerQuery.elapsed(), iResultCount);
		if (pHandler) {
			pHandler->notifyQueryResult(szQuery, bRes, iResultCount, szResultString);
		}
	} else {
		qCritical("[DatabaseController] Cannot open database for table data loading");
	}

	if (pHandler) {
		pHandler->notifyQueriesFinished(bRes, true);
	}

	return bRes;
}

bool DatabaseController::processWorksheetQueryResults(const QString& szWorksheetQuery, DatabaseProcessHandler* pHandler)
{
	bool bRes;

	QStringList listRowHeader;
	QStringList listRowData;
	QString szRequest;

	QStringList listQueries = splitSQLQueries(szWorksheetQuery);

	bool bHasResult = false;

	if (!listQueries.isEmpty()) {
		bRes = openDatabase();
		if (bRes){
			for (const QString& szCurrentQuery : listQueries)
			{
				bHasResult = false;

				int iResultCount = 0;
				QString szResultString;

				QElapsedTimer timerQuery;
				timerQuery.start();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
				QRegularExpression regExp("\\s+");
#else
				QRegExp regExp("\\s+");
#endif
				szRequest = szCurrentQuery.section(regExp, 0, 0, QString::SectionSkipEmpty);
				if (szRequest.toLower() == "select") {
					auto* pQueryModel = new QSqlDisplayQueryModel();
					pQueryModel->setQuery(szCurrentQuery, m_db);
					bHasResult = true;
					if (pQueryModel->query().lastError().isValid()) {
						bRes = false;
						bHasResult = false;
					}
					if (pHandler) {
						pHandler->notifyQueryModel(pQueryModel);
					}

					const QSqlQuery& query = pQueryModel->query();
					const auto& iTimeElapsed = timerQuery.elapsed();

					// Compute rows count
					if (bRes){
						QSqlQuery queryCount(szCurrentQuery, m_db);
						bRes = queryCount.exec();
						if (bRes) {
							if (queryCount.driver()->hasFeature(QSqlDriver::QuerySize)) {
								iResultCount = queryCount.size();
							}else {
								while (queryCount.next()) {
									iResultCount++;
								}
							}
						}
					}

					szResultString = makeQueryResultString(query, iTimeElapsed, iResultCount);
				} else {
					QSqlQuery query(m_db);
					bRes = query.exec(szCurrentQuery);
					szResultString = makeQueryResultString(query, timerQuery.elapsed());
				}

				if (pHandler) {
					pHandler->notifyQueryResult(szCurrentQuery, bRes, iResultCount, szResultString);
				}

				if (!bRes) {
					break;
				}
			}

			closeDataBase();
		} else {
			qCritical("[DatabaseController] Cannot open database for new query loading");
		}
	}

	pHandler->notifyQueriesFinished(bRes, bHasResult);

	return bRes;
}

QStringList DatabaseController::splitSQLQueries(const QString& szSQLScript)
{
    QStringList listQueries;
    QString szCurrentQuery;
    bool bInSingleQuote = false;
    bool bInDoubleQuote = false;
    bool bInLineComment = false;
    bool bInBlockComment = false;

    for (int i = 0; i < szSQLScript.size(); ++i)
    {
        QChar c = szSQLScript[i];
        QChar next = (i + 1 < szSQLScript.size()) ? szSQLScript[i + 1] : QChar();

        // Gestion des commentaires de ligne (--)
        if (!bInSingleQuote && !bInDoubleQuote && !bInBlockComment && c == '-' && next == '-') {
            bInLineComment = true;
            szCurrentQuery += c;
            continue;
        }
        if (bInLineComment && c == '\n') {
            bInLineComment = false;
        }

        // Gestion des commentaires de bloc (/* ... */)
        if (!bInSingleQuote && !bInDoubleQuote && !bInLineComment && c == '/' && next == '*') {
            bInBlockComment = true;
            szCurrentQuery += c;
            ++i; // Sauter le '*'
            continue;
        }
        if (bInBlockComment && c == '*' && next == '/') {
            bInBlockComment = false;
            szCurrentQuery += c;
            ++i; // Sauter le '/'
            continue;
        }

        // Gestion des chaînes littérales
        if (!bInLineComment && !bInBlockComment) {
            if (c == '\'' && !bInDoubleQuote) {
                bInSingleQuote = !bInSingleQuote;
            } else if (c == '"' && !bInSingleQuote) {
                bInDoubleQuote = !bInDoubleQuote;
            }
        }

        // Si on est dans un commentaire ou une chaîne, on ajoute le caractère et on passe
        if (bInLineComment || bInBlockComment || bInSingleQuote || bInDoubleQuote) {
            szCurrentQuery += c;
            continue;
        }

    	// Handle semicolon (query separator)
        if (c == ';') {
            szCurrentQuery += c;
            listQueries << szCurrentQuery.trimmed();
            szCurrentQuery.clear();
        } else {
            szCurrentQuery += c;
        }
    }

    // Add last query if not empty
    if (!szCurrentQuery.trimmed().isEmpty()) {
        listQueries << szCurrentQuery.trimmed();
    }

    return listQueries;
}

bool DatabaseController::loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data, DatabaseProcessHandler* pHandler)
{
	bool bRes = true;

	QString szQuery = loadTableCreationScriptQuery(szTableName);

	if(!szQuery.isEmpty()){
		QString szTmp;

		QElapsedTimer timerQuery;
		timerQuery.start();

		// Run table creation script query
		QSqlQuery query(m_db);
		bRes = query.exec(szQuery);

		int iResultCount = 0;

		// Iterate results
		if (bRes) {
			while(query.next())
			{
				szTmp = makeTableCreationScriptQueryResult(query);
				func(szTmp, user_data);
				iResultCount++;
			}
		}

		const auto& szResultString = makeQueryResultString(query, timerQuery.elapsed(), iResultCount);
		if (pHandler) {
			pHandler->notifyQueryResult(szQuery, bRes, -1, szResultString);
		}
	} else {
		qCritical("[DatabaseController] Cannot open database for table creation script loading");
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

QString DatabaseController::getDatabaseName() const
{
	return m_db.databaseName();
}
