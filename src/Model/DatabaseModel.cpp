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

const QString& DatabaseModel::getDatabaseType() const
{
	return m_szDatabaseType;
}

void DatabaseModel::setDatabaseType(const QString& szDatabaseType)
{
	m_szDatabaseType = szDatabaseType;
}

const QString& DatabaseModel::getDatabaseHost() const
{
	return m_szDatabaseHost;
}

void DatabaseModel::setDatabaseHost(const QString& szDatabaseHost)
{
	m_szDatabaseHost = szDatabaseHost;
}

const QString& DatabaseModel::getDatabasePort() const
{
	return m_szDatabasePort;
}

void DatabaseModel::setDatabasePort(const QString& szDatabasePort)
{
	m_szDatabasePort = szDatabasePort;
}
