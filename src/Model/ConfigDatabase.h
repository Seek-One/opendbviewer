/*
 * ConfigDatabase.h
 *
 *  Created on: 19 avr. 2021
 *      Author: gsegard
 */

#ifndef SRC_MODEL_CONFIGDATABASE_H_
#define SRC_MODEL_CONFIGDATABASE_H_

#include <QList>
#include <QString>

class ConfigDatabase {
public:
	ConfigDatabase(const QString& szIdentifier, int iID);
	ConfigDatabase(const ConfigDatabase& other);
	virtual ~ConfigDatabase();

	const QString& getDatabaseIdentifier() const;
	const QString& getDatabaseName() const;
	int getDatabaseID() const;

	ConfigDatabase& operator=(const ConfigDatabase& other);

private:
	QString m_szDatabaseIdentifier;
	QString m_szDatabaseName;
	int m_iDatabaseID;
};


class ConfigDatabaseList : public QList<ConfigDatabase> {
public:
	ConfigDatabaseList();
	virtual ~ConfigDatabaseList();
};

#endif /* SRC_MODEL_CONFIGDATABASE_H_ */
