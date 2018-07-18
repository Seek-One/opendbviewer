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
	DatabaseControllerPostgreSQL(DatabaseModel databaseModel);
	virtual ~DatabaseControllerPostgreSQL();

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName);
	virtual QStringList loadTableDescriptionResult(const QSqlQuery query);
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery query);
	virtual QStringList listColumnNames(const QString& szTableName);
	virtual QString loadTableCreationScriptQuery(const QString& szTableName);
	virtual QString makeTableCreationScriptQueryResult(const QSqlQuery query);
};

#endif /* SRC_DATABASE_DATABASECONTROLLERPOSTGRESQL_H_ */
