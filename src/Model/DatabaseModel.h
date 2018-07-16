/*
 * DatabaseModel.h
 *
 *  Created on: 25 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_MODEL_DATABASEMODEL_H_
#define SRC_MODEL_DATABASEMODEL_H_

#include <QString>

class DatabaseModel {
public:
	enum DatabaseType {
		SQLiteType = 0,
		MySQLType,
		PostgreSQLType,
	};

public:
	DatabaseModel();
	DatabaseModel(const QString& szName, int iType);
	virtual ~DatabaseModel();

	const QString& getDatabaseName() const;
	void setDatabaseName(const QString& szDatabaseName);
	const QString& getDatabasePath() const;
	void setDatabasePath(const QString& szDatabasePath);
	const QString& getDatabaseHost() const;
	void setDatabaseHost(const QString& szDatabaseHost);
	const QString& getDatabaseUsername() const;
	void setDatabaseUsername(const QString& szDatabaseUsername);
	int getDatabaseType() const;
	void setDatabaseType(int iDatabaseType);
	int getDatabasePort() const;
	void setDatabasePort(int iDatabasePort);

	bool operator==(const DatabaseModel& other) const;
	bool operator!=(const DatabaseModel& other) const;
private:
	int m_iDatabaseType;
	int m_iDatabasePort;
	QString m_szDatabaseName;
	QString m_szDatabasePath;
	QString m_szDatabaseHost;
	QString m_szDatabaseUsername;
};



#endif /* SRC_MODEL_DATABASEMODEL_H_ */
