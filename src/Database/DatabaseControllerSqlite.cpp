/*
 * DatabaseControllerSqlite.cpp
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#include "DatabaseControllerSqlite.h"
#include "DatabaseController.h"
#include <QSqlDatabase>

DatabaseControllerSqlite::DatabaseControllerSqlite(const QString& szFileName) : DatabaseController(szFileName)
{
}

DatabaseControllerSqlite::~DatabaseControllerSqlite()
{

}

QString DatabaseControllerSqlite::loadTableDescriptionQuery(const QString& szTableName)
{
	return QString("PRAGMA table_info(%0);").arg(szTableName);
}

