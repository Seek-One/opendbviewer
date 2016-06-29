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

class DatabaseControllerMysql : public DatabaseController
{
public:
	DatabaseControllerMysql(const QString& szFileName, const QStringList& szDatabaseInfoList);
	virtual ~DatabaseControllerMysql();

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName);
	virtual QStringList loadTableDescriptionResult(const QSqlQuery query);
	virtual QStringList loadTableDescriptionColumnNames(const QSqlQuery query);
	virtual QStringList listColumnNames(QString szTableName);

private:
	void splitDatabaseInfoList(QStringList& szDatabaseInfoList);

private:
	QString m_szHostName;
	int m_port;
	QString m_szUsername;
	QString m_szPassword;
	QString m_szDatabaseName;
	QStringList m_szDatabaseInfoList;
};

#endif /* SRC_DATABASE_DATABASECONTROLLERMYSQL_H_ */
