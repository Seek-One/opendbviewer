/*
 * DatabaseControllerMysql.cpp
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#include "Database/DatabaseControllerMysql.h"

DatabaseControllerMysql::DatabaseControllerMysql(DatabaseModel databaseModel) : DatabaseController(databaseModel)
{

	m_db = QSqlDatabase::addDatabase("QMYSQL", QString::number(g_iConnectionIdentifier++));
    m_db.setDatabaseName(databaseModel.getDatabaseName());
    m_db.setPort(databaseModel.getDatabasePort());
    m_db.setHostName(databaseModel.getDatabaseHost());
    m_db.setUserName(databaseModel.getDatabaseUsername());
    //TODO set the password for qsqldatabase instance
}

DatabaseControllerMysql::~DatabaseControllerMysql()
{
}

QString DatabaseControllerMysql::loadTableDescriptionQuery(const QString& szTableName)
{
	return QString("DESCRIBE %0;").arg(szTableName);
}

QStringList DatabaseControllerMysql::loadTableDescriptionResult(const QSqlQuery query)
{
	QStringList pRowData;
	QString szField = query.value(0).toString();
	QString szName = query.value(1).toString();
	QString szType = query.value(2).toString();
	QString szNotNull = query.value(3).toString();
	QString szDefaultValue = query.value(4).toString();
	QString szPk = query.value(5).toString();
	pRowData << szField << szName << szType << szNotNull << szDefaultValue << szPk;

	return pRowData;
}

QStringList DatabaseControllerMysql::loadTableDescriptionColumnNames(const QSqlQuery query)
{
	QStringList pColumnNames;
	int currentColumnNumber;
	//appending column names to columnNameList
	for (currentColumnNumber = 0; currentColumnNumber < query.record().count(); currentColumnNumber++)
	{
		QSqlField field = query.record().field(currentColumnNumber);
		pColumnNames << field.name();
	}
	return pColumnNames;
}

QStringList DatabaseControllerMysql::listColumnNames(const QString& szTableName)
{
	QStringList listColumnName;
	QSqlQuery tableInfoQuery(m_db);
	tableInfoQuery.exec(loadTableDescriptionQuery(szTableName));
	while (tableInfoQuery.next())
	   {
		QString szName = tableInfoQuery.value(0).toString();
		listColumnName += szName;
	   }
	return listColumnName;
}

QString DatabaseControllerMysql::loadTableCreationScriptQuery(const QString& szTableName)
{
	return QString("SHOW CREATE TABLE %0;").arg(szTableName);
}

QString DatabaseControllerMysql::makeTableCreationScriptQueryResult(const QSqlQuery query)
{
	return QString(query.value(1).toString());
}
