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
	DatabaseModel();
	DatabaseModel(const QString& szName, const QString& szType);
	virtual ~DatabaseModel();

	const QString& getDatabaseName() const;
	void setDatabaseName(const QString& szDatabaseName);
	const QString& getDatabasePath() const;
	void setDatabasePath(const QString& szDatabasePath);
	const QString& getDatabaseType() const;
	void setDatabaseType(const QString& szDatabaseType);
	const QString& getDatabaseHost() const;
	void setDatabaseHost(const QString& szDatabaseHost);
	const QString& getDatabasePort() const;
	void setDatabasePort(const QString& szDatabasePort);

	bool operator==(const DatabaseModel& other) const;
	bool operator!=(const DatabaseModel& other) const;
private:
	QString m_szDatabaseName;
	QString m_szDatabasePath;
	QString m_szDatabaseType;
	QString m_szDatabaseHost;
	QString m_szDatabasePort;
};



#endif /* SRC_MODEL_DATABASEMODEL_H_ */
