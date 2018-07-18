/*
 * DatabaseModel.cpp
 *
 *  Created on: 25 juin 2018
 *      Author: cdegeorgi
 */

#include <Model/DatabaseModel.h>
#include <Global/ApplicationSettings.h>

DatabaseModel::DatabaseModel()
{
	m_DatabaseType = UnknownType;
	m_iDatabasePort = 0;
}

DatabaseModel::DatabaseModel(const QString& szName, DatabaseModel::DatabaseType type)
{
	m_szDatabaseName = szName;
	m_DatabaseType = type;
	m_iDatabasePort = 0;
}

DatabaseModel::~DatabaseModel()
{
}
const QString& DatabaseModel::getDatabaseName() const
{
	return m_szDatabaseName;
}

void DatabaseModel::setDatabaseName(const QString& szDatabaseName)
{
	m_szDatabaseName = szDatabaseName;
}

const QString& DatabaseModel::getDatabasePath() const
{
	return m_szDatabasePath;
}

void DatabaseModel::setDatabasePath(const QString& szDatabasePath)
{
	m_szDatabasePath = szDatabasePath;

}

int DatabaseModel::getDatabaseType() const
{
	return m_DatabaseType;
}

void DatabaseModel::setDatabaseType(DatabaseModel::DatabaseType DatabaseType)
{
	m_DatabaseType = DatabaseType;
}

const QString& DatabaseModel::getDatabaseHost() const
{
	return m_szDatabaseHost;
}

void DatabaseModel::setDatabaseHost(const QString& szDatabaseHost)
{
	m_szDatabaseHost = szDatabaseHost;
}

int DatabaseModel::getDatabasePort() const
{
	return m_iDatabasePort;
}

void DatabaseModel::setDatabasePort(int iDatabasePort)
{
	m_iDatabasePort = iDatabasePort;
}

const QString& DatabaseModel::getDatabaseUsername() const
{
	return m_szDatabaseUsername;
}

void DatabaseModel::setDatabaseUsername(const QString& szDatabaseUsername)
{
	m_szDatabaseUsername = szDatabaseUsername;
}

bool DatabaseModel::operator==(const DatabaseModel& other) const
{
	return (m_szDatabaseName == other.m_szDatabaseName &&
			m_szDatabaseHost == other.m_szDatabaseHost &&
			m_szDatabasePath == other.m_szDatabasePath &&
			m_DatabaseType == other.m_DatabaseType &&
			m_iDatabasePort == other.m_iDatabasePort &&
			m_szDatabaseUsername == other.m_szDatabaseUsername);
}

bool DatabaseModel::operator!=(const DatabaseModel& other) const
{
	return (m_szDatabaseName != other.m_szDatabaseName ||
			m_szDatabaseHost != other.m_szDatabaseHost ||
			m_szDatabasePath != other.m_szDatabasePath ||
			m_DatabaseType != other.m_DatabaseType ||
			m_iDatabasePort != other.m_iDatabasePort ||
			m_szDatabaseUsername != other.m_szDatabaseUsername);
}
