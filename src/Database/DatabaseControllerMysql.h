/*
 * DatabaseControllerMysql.h
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLERMYSQL_H_
#define SRC_DATABASE_DATABASECONTROLLERMYSQL_H_

#include "DatabaseController.h"

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QDebug>

class DatabaseControllerMysql : public DatabaseController
{
public:
	DatabaseControllerMysql(DatabaseModel databaseModel);
	virtual ~DatabaseControllerMysql();

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName);
	virtual QStringList loadTableDescriptionResult(const QSqlQuery& query);
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery& query);
	virtual QStringList listColumnNames(const QString& szTableName);
	virtual QString loadTableCreationScriptQuery(const QString& szTableName);
	virtual QString makeTableCreationScriptQueryResult(const QSqlQuery& query);
};

#endif /* SRC_DATABASE_DATABASECONTROLLERMYSQL_H_ */
