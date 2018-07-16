/*
 * DatabaseControllerPostgresSQL.h
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLERPOSTGRESQL_H_
#define SRC_DATABASE_DATABASECONTROLLERPOSTGRESQL_H_

#include "DatabaseController.h"

#include <QStringList>

class DatabaseControllerPostgreSQL : public DatabaseController
{
public:
	//DatabaseControllerPostgreSQL(const QString &szFileName, const QStringList& szDatabaseInfoList);
	DatabaseControllerPostgreSQL(DatabaseModel databaseModel);
	virtual ~DatabaseControllerPostgreSQL();

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName);
	virtual QStringList loadTableDescriptionResult(const QSqlQuery query);
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery query);
	virtual QStringList listColumnNames(const QString& szTableName);
	virtual QString loadTableCreationScriptQuery(const QString& szTableName);
	virtual QString makeTableCreationScriptQueryResult(const QSqlQuery query);

private:
	//void splitDatabaseInfoList(QStringList& szDatabaseInfoList);
	void splitDatabaseInfoList(DatabaseModel databaseModel);

private:
	QString m_szHostName;
	int m_port;
	QString m_szUsername;
	QString m_szPassword;
	QString m_szDatabaseName;
	QStringList m_szDatabaseInfoList;
};

#endif /* SRC_DATABASE_DATABASECONTROLLERPOSTGRESQL_H_ */
