/*
 * ApplicationSettings.cpp
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#include "ApplicationSettings.h"

QString ApplicationSettings::m_szCurrentExplorerPath;
bool ApplicationSettings::m_bWindowMaximized;

HistoryDatabaseList ApplicationSettings::m_listHistory;

ConfigDatabaseController* ApplicationSettings::m_pConfigDatabaseController = NULL;

const QString& ApplicationSettings::getCurrentExplorerPath()
{
	return m_szCurrentExplorerPath;
}

void ApplicationSettings::setCurrentExplorerPath(const QString& szCurrentExplorerPath)
{
	m_szCurrentExplorerPath = szCurrentExplorerPath;
}

bool ApplicationSettings::getWindowMaximized()
{
	return m_bWindowMaximized;
}

void ApplicationSettings::setWindowMaximized(bool bWindowMaximized)
{
	m_bWindowMaximized = bWindowMaximized;
}

const HistoryDatabaseList ApplicationSettings::getHistoryList()
{
	return m_listHistory;
}

void ApplicationSettings::setHistoryList(const HistoryDatabaseList& databaseList)
{
	m_listHistory = databaseList;
}

void ApplicationSettings::addHistoryDatabase(const DatabaseModel& database)
{
	m_listHistory.append(database);
}

ConfigDatabaseController* ApplicationSettings::getConfigDatabaseController()
{
	return m_pConfigDatabaseController;
}

void ApplicationSettings::setConfigDatabaseController(ConfigDatabaseController* pConfigDatabaseController)
{
	m_pConfigDatabaseController = pConfigDatabaseController;
}
