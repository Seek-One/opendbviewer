/*
 * DatabaseControllerSqlite.cpp
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#include "DatabaseControllerSqlite.h"

DatabaseControllerSqlite::DatabaseControllerSqlite(const DatabaseModel& databaseModel) : DatabaseController(databaseModel)
{
	m_db = QSqlDatabase::addDatabase("QSQLITE", databaseModel.getDatabaseName());
	m_db.setDatabaseName(databaseModel.getDatabasePath());
}

DatabaseControllerSqlite::~DatabaseControllerSqlite()
{

}

QString DatabaseControllerSqlite::loadTableDescriptionQuery(const QString& szTableName)
{
	return QString("PRAGMA table_info(%0);").arg(szTableName);
}

QStringList DatabaseControllerSqlite::loadTableDescriptionResult(const QSqlQuery query)
{
	QStringList listRowData;
	QString szNotNull;

	QString szName = query.value(1).toString();
	QString szType = query.value(2).toString();
	bool bNotNull = query.value(3).toBool();
	if(bNotNull == false)//bNotNull doesn't appear in the table, this is a workaround
		szNotNull = "false";
	else
		szNotNull = "true";//end of workaround
	QString szDefaultValue = query.value(4).toString();
	QString szPk = query.value(5).toString();
	listRowData << szName << szType << szNotNull << szDefaultValue << szPk;

	return listRowData;
}

QStringList DatabaseControllerSqlite::loadTableDescriptionColumnNames(const QSqlQuery query)
{
	QStringList listColumnName;
	listColumnName << "Field" << "Type" << "Not null" << "Default value" << "Primary key";
	return listColumnName;
}

QString DatabaseControllerSqlite::loadTableCreationScriptQuery(const QString& szTableName)
{
	return QString("SELECT sql FROM sqlite_master WHERE type = 'table' AND name = '"+szTableName+"';");
}

QString DatabaseControllerSqlite::makeTableCreationScriptQueryResult(const QSqlQuery query)
{
	return QString(query.value(0).toString());
}

QStringList DatabaseControllerSqlite::listColumnNames(const QString& szTableName)
{
	QStringList szListColumnName;
	szListColumnName << "rowid";//appending rowid to the list
	QSqlQuery tableInfoQuery(m_db);
	tableInfoQuery.exec(loadTableDescriptionQuery(szTableName));
	while (tableInfoQuery.next())
	   {
		QString szName = tableInfoQuery.value(1).toString();
		szListColumnName += szName;
	   }
	return szListColumnName;
}
