/*
 * DatabaseController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLER_H_
#define SRC_DATABASE_DATABASECONTROLLER_H_

#include <QSqlDatabase>

typedef void (*DbLoadTableCB)(const QString& szTable, void* user_data);
typedef void (*DbLoadTableDescription)(const QList<QString>& pColumnName, const QList<QString>& pRowData, void* user_data);
typedef void (*DbLoadTableData)(const QList<QString>& pColumnName, const QList<QString>& pRowData, void* user_data);
typedef void (*DbLoadTableCreationScript)(const QString& szCreationScriptString, void* user_data);
typedef void (*DbLoadWorksheetQueryResults)(const QList<QString>& pColumnName, const QList<QString>& pRowData, void* user_data);

class DatabaseController
{
public:
	DatabaseController(const QString& szFilename);
	virtual ~DatabaseController();

	virtual bool loadTables(DbLoadTableCB func, void* user_data);
	virtual bool loadSystemTables(DbLoadTableCB func, void* user_data);
	virtual bool loadViewsTables(DbLoadTableCB func, void* user_data);
	virtual bool loadTableDescription(const QString& szTable, DbLoadTableDescription func, void* user_data);
	virtual bool loadTableData(const QString& szTableName, const QString& szFilter, DbLoadTableData func, void* user_data);
	virtual bool loadTableCreationScript(const QString& szTableName, DbLoadTableCreationScript func, void* user_data);
	virtual bool loadWorksheetQueryResults(QString& szWorksheetQuery, DbLoadWorksheetQueryResults func, void* user_data);
	QString getQueryResultString() const;

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName) = 0;
	virtual QStringList loadTableDescriptionResult(const QSqlQuery query) = 0;
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery query) = 0;
	virtual QStringList listColumnNames(QString szTableName) = 0;


private:
	QString makeStringNumberOfRows(QSqlQuery query);
	QString makeQueryResultString(QSqlQuery query, QString& szQueryOutput);

protected:
	QString m_szFilename;
	QSqlDatabase m_db;
	QString m_szResultString;

	bool openDatabase();
	void closeDataBase();
};

#endif /* SRC_DATABASE_DATABASECONTROLLER_H_ */
