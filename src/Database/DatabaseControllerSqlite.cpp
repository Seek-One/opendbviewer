/*
 * DatabaseControllerSqlite.cpp
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#include "DatabaseControllerSqlite.h"
#include "DatabaseController.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QSqlField>
#include <QSqlRecord>


DatabaseControllerSqlite::DatabaseControllerSqlite(const QString& szFileName) : DatabaseController(szFileName)
{
	m_db = QSqlDatabase::addDatabase("QSQLITE", szFileName);
	m_db.setDatabaseName(szFileName);
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
	QStringList pRowData;
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
	pRowData << szName << szType << szNotNull << szDefaultValue << szPk;

	return pRowData;
}

QStringList DatabaseControllerSqlite::loadTableDescriptionColumnNames(const QSqlQuery query)
{
	QStringList pColumnNames;
	pColumnNames << "Field" << "Type" << "Not null" << "Default value" << "Primary key";
	return pColumnNames;
}

QStringList DatabaseControllerSqlite::listColumnNames(QString szTableName)
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
