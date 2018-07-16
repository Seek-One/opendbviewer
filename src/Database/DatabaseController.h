/*
 * DatabaseController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLER_H_
#define SRC_DATABASE_DATABASECONTROLLER_H_

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QDebug>

#include "DatabaseCallback.h"

class DatabaseController
{
public:
	//DatabaseController(const QString& szFilename);
	DatabaseController(const DatabaseModel&  databaseModel);
	virtual ~DatabaseController();

	bool openDatabase();
	void closeDataBase();

	bool loadTables(DbLoadTableCB func, void* user_data);
	bool loadSystemTables(DbLoadTableCB func, void* user_data);
	bool loadViewsTables(DbLoadTableCB func, void* user_data);
	bool loadTableDescription(const QString& szTable, DbLoadTableDescription func, void* user_data);
	bool loadTableData(const QString& szTableName, const QString& szFilter, DbLoadTableData func, void* user_data);
	bool loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data);
	bool loadWorksheetQueryResults(QString& szWorksheetQuery, DbLoadWorksheetQueryResults func, void* user_data);

	QString getQueryResultString() const;

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName) = 0;
	virtual QStringList loadTableDescriptionResult(const QSqlQuery query) = 0;
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery query) = 0;
	virtual QStringList listColumnNames(const QString& szTableName) = 0;
	virtual QString loadTableCreationScriptQuery(const QString& szTableName) = 0;
	virtual QString makeTableCreationScriptQueryResult(const QSqlQuery query) = 0;


private:
	QString makeQueryResultString(const QSqlQuery& query, int iNbRowsSelected);


protected:
	QString m_szFilename;
	QSqlDatabase m_db;
	QString m_szResultString;

};

#endif /* SRC_DATABASE_DATABASECONTROLLER_H_ */
