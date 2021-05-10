/*
 * ConfigDatabaseController.h
 *
 *  Created on: 15 avr. 2021
 *      Author: gsegard
 */

#ifndef SRC_CONTROLLER_CONFIGDATABASECONTROLLER_H_
#define SRC_CONTROLLER_CONFIGDATABASECONTROLLER_H_

#include <QJsonDocument>

#include "Model/ConfigDatabase.h"

class ConfigDatabaseController
{

public:
	ConfigDatabaseController();
	virtual ~ConfigDatabaseController();

public:
	bool initDatabasesList();

	bool saveDatabasesList();
	void addDatabase(const ConfigDatabase& configDatabase);
	void moveDatabaseFirstInList(const ConfigDatabase& configDatabase, int iIndex);

	bool initQueries(const QString& szName);
	void addQuery(const QString& szQuery, QStringList& listQueries);
	bool removeQuery(const QString& szQuery, QStringList& listQueries);
	void loadQueries(const QString& szName, QStringList& listQueries);
	bool saveQueries(const QString& szName, const QStringList& listQueries);

	QString getDatabaseName(const QString& szDatabaseIdentifier) const;
	const ConfigDatabaseList& getDatabaseList() const;
	int getNextID();

private:
	bool removeUnusedQueriesFiles();

	QJsonDocument parseToJsonDocument(const QString& szFile);
	bool writeToFile(const QString& szFilePath, const QByteArray& szText);

private:
	ConfigDatabaseList m_listConfigDatabase;
};

#endif /* SRC_CONTROLLER_CONFIGDATABASECONTROLLER_H_ */
