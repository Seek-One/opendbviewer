/*
 * DatabaseController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLER_H_
#define SRC_DATABASE_DATABASECONTROLLER_H_

#include <QSqlTableModel>
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
#include "GUIModel/QSqlDisplayQueryModel.h"
#include "GUIModel/QSqlDisplayTableModel.h"

class DatabaseController : public QObject
{
	Q_OBJECT
public:
	DatabaseController(const DatabaseModel&  databaseModel);
	virtual ~DatabaseController();

	virtual QString getDatabaseName() const;

	bool openDatabase();
	void closeDataBase();

	QSqlDatabase getSqlDatabase();

	bool loadTables(DbLoadTableCB func, void* user_data);
	bool loadSystemTables(DbLoadTableCB func, void* user_data);
	bool loadViewsTables(DbLoadTableCB func, void* user_data);

	bool loadTableData(const QString& szTableName, const QString& szFilter, QSqlDisplayTableModel* pTableModel);
	bool loadTableDescription(const QString& szTable, DbLoadTableDescription func, void* user_data);

	bool loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data);
	bool loadWorksheetQueryResults(QString& szWorksheetQuery, QSqlDisplayQueryModel** ppQueryModel);

	QString getQueryResultString() const;

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName) = 0;
	virtual QStringList loadTableDescriptionResult(const QSqlQuery query) = 0;
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery query) = 0;
	virtual QStringList listColumnNames(const QString& szTableName) = 0;
	virtual QString loadTableCreationScriptQuery(const QString& szTableName) = 0;
	virtual QString makeTableCreationScriptQueryResult(const QSqlQuery query) = 0;

private:
	QString makeQueryResultString(const QSqlQuery& query, qint64 iTimeMS, int iNbRowsSelected = 0);


protected:
	DatabaseModel m_databaseModel;
	QSqlDatabase m_db;
	QString m_szResultString;

	static int g_iConnectionIdentifier;
};

#endif /* SRC_DATABASE_DATABASECONTROLLER_H_ */
