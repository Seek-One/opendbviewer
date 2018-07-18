/*
 * DatabaseControllerPostgreSQL.cpp
 *
 *  Created on: 15 sept 2017
 *      Author: ebeuque
 */
#include <QSqlDatabase>

#include "DatabaseControllerPostgreSQL.h"

DatabaseControllerPostgreSQL::DatabaseControllerPostgreSQL(DatabaseModel databaseModel) : DatabaseController(databaseModel)
{
	m_db = QSqlDatabase::addDatabase("QPSQL", QString::number(g_iConnectionIdentifier++));
    m_db.setDatabaseName(databaseModel.getDatabaseName());
    m_db.setPort(databaseModel.getDatabasePort());
    m_db.setHostName(databaseModel.getDatabaseHost());
    m_db.setUserName(databaseModel.getDatabaseUsername());
    //TODO set the password for qsqldatabase instance
}

DatabaseControllerPostgreSQL::~DatabaseControllerPostgreSQL()
{
}

QString DatabaseControllerPostgreSQL::loadTableDescriptionQuery(const QString& szTableName)
{
	return QString("SELECT column_name, data_type, character_maximum_length, is_nullable, column_default FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '%0';").arg(szTableName);
}

QStringList DatabaseControllerPostgreSQL::loadTableDescriptionResult(const QSqlQuery query)
{
	QStringList pRowData;
	QString szColName = query.value(0).toString();
	QString szType = query.value(1).toString();
	QString szMaxLen = query.value(2).toString();
	QString szNullable = query.value(2).toString();
	QString szDefaultValue = query.value(4).toString();
	//QString szPk = query.value(5).toString();
	pRowData << szColName << szType << szMaxLen << szNullable << szDefaultValue;

	return pRowData;
}

QStringList DatabaseControllerPostgreSQL::loadTableDescriptionColumnNames(const QSqlQuery query)
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

QStringList DatabaseControllerPostgreSQL::listColumnNames(const QString& szTableName)
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

QString DatabaseControllerPostgreSQL::loadTableCreationScriptQuery(const QString& szTableName)
{
	return QString();
}

QString DatabaseControllerPostgreSQL::makeTableCreationScriptQueryResult(const QSqlQuery query)
{
	return QString(query.value(1).toString());
}
