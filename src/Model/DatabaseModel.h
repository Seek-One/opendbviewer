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
		UnknownType = 0,
		SQLiteType,
		MySQLType,
		PostgreSQLType,
	};

public:
	DatabaseModel();
	DatabaseModel(const QString& szName, DatabaseModel::DatabaseType type);
	virtual ~DatabaseModel();

	const QString& getDatabaseName() const;
	void setDatabaseName(const QString& szDatabaseName);
	int getDatabaseType() const;
	void setDatabaseType(DatabaseModel::DatabaseType DatabaseType);
	const QString& getDatabasePath() const;
	void setDatabasePath(const QString& szDatabasePath);
	const QString& getDatabaseHost() const;
	void setDatabaseHost(const QString& szDatabaseHost);
	int getDatabasePort() const;
	void setDatabasePort(int iDatabasePort);
	const QString& getDatabaseUsername() const;
	void setDatabaseUsername(const QString& szDatabaseUsername);
	const QString& getDatabasePassword() const;
	void setDatabasePassword(const QString& szDatabasePassword);


	bool operator==(const DatabaseModel& other) const;
	bool operator!=(const DatabaseModel& other) const;
private:
	QString m_szDatabaseName;
	DatabaseModel::DatabaseType m_DatabaseType;
	QString m_szDatabasePath;
	QString m_szDatabaseHost;
	int m_iDatabasePort;
	QString m_szDatabaseUsername;
	QString m_szDatabasePassword;
};



#endif /* SRC_MODEL_DATABASEMODEL_H_ */
