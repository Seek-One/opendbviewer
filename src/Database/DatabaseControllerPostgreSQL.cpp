/*
 * DatabaseControllerPostgresSQL.cpp
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#include "Database/DatabaseControllerPostgresSQL.h"

DatabaseControllerPostgresSQL::DatabaseControllerPostgresSQL(const QString &szFileName, const QStringList& szDatabaseInfoList) : DatabaseController(szFileName)
{
	m_szDatabaseInfoList = szDatabaseInfoList;
	splitDatabaseInfoList(m_szDatabaseInfoList);

	m_db = QSqlDatabase::addDatabase("QMYSQL", m_szDatabaseName);
    m_db.setHostName(m_szHostName);
    m_db.setPort(m_port);
    m_db.setDatabaseName(m_szDatabaseName);
    m_db.setUserName(m_szUsername);
    m_db.setPassword(m_szPassword);
}

DatabaseControllerPostgresSQL::~DatabaseControllerPostgresSQL()
{
}

QString DatabaseControllerPostgresSQL::loadTableDescriptionQuery(const QString& szTableName)
{
	return QString("\d %0;").arg(szTableName);
}

QStringList DatabaseControllerPostgresSQL::loadTableDescriptionResult(const QSqlQuery query)
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

QStringList DatabaseControllerPostgresSQL::loadTableDescriptionColumnNames(const QSqlQuery query)
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

QStringList DatabaseControllerPostgresSQL::listColumnNames(const QString& szTableName)
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

QString DatabaseControllerPostgresSQL::loadTableCreationScriptQuery(const QString& szTableName)
{
	return QString("SHOW CREATE TABLE %0;").arg(szTableName);
}

QString DatabaseControllerPostgresSQL::makeTableCreationScriptQueryResult(const QSqlQuery query)
{
	return QString(query.value(1).toString());
}

void DatabaseControllerPostgresSQL::splitDatabaseInfoList(QStringList& szDatabaseInfoList)
{
	m_szHostName = szDatabaseInfoList.value(0);
	m_port = szDatabaseInfoList.value(1).toInt();
	m_szUsername = szDatabaseInfoList.value(2);
	m_szPassword = szDatabaseInfoList.value(3);
	m_szDatabaseName = szDatabaseInfoList.value(4);
}
