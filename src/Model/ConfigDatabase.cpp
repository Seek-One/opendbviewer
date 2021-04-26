/*
 * ConfigDatabase.cpp
 *
 *  Created on: 19 avr. 2021
 *      Author: gsegard
 */

#include "Model/ConfigDatabase.h"

#define MAX_ITEMS 30

ConfigDatabase::ConfigDatabase(const QString& szIdentifier, int iID)
{
	m_szDatabaseIdentifier = szIdentifier;
	m_szDatabaseName = "database_" + QString::number(iID);
	m_iDatabaseID = iID;
}

ConfigDatabase::ConfigDatabase(const ConfigDatabase& other)
{
	m_szDatabaseIdentifier = other.m_szDatabaseIdentifier;
	m_szDatabaseName = other.m_szDatabaseName;
	m_iDatabaseID = other.m_iDatabaseID;
}

ConfigDatabase::~ConfigDatabase() {

}

ConfigDatabase& ConfigDatabase::operator=(const ConfigDatabase& other)
{
	m_szDatabaseIdentifier = other.m_szDatabaseIdentifier;
	m_szDatabaseName = other.m_szDatabaseName;
	m_iDatabaseID = other.m_iDatabaseID;
	return *this;
}

const QString& ConfigDatabase::getDatabaseIdentifier() const
{
	return m_szDatabaseIdentifier;
}

const QString& ConfigDatabase::getDatabaseName() const
{
	return m_szDatabaseName;
}

int ConfigDatabase::getDatabaseID() const
{
	return m_iDatabaseID;
}

ConfigDatabaseList::ConfigDatabaseList() {

}

ConfigDatabaseList::~ConfigDatabaseList() {

}
