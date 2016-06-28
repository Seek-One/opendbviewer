/*
 * DatabaseControllerSqlite.h
 *
 *  Created on: 27 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLERSQLITE_H_
#define SRC_DATABASE_DATABASECONTROLLERSQLITE_H_

#include "DatabaseController.h"
#include <QString>


class DatabaseControllerSqlite : public DatabaseController
{
public:
	DatabaseControllerSqlite(const QString& szFilename);
	virtual ~DatabaseControllerSqlite();

protected:
	virtual QString loadTableDescriptionQuery(const QString& szTableName);
};

#endif /* SRC_DATABASE_DATABASECONTROLLERSQLITE_H_ */
