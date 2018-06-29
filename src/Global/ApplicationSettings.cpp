/*
 * ApplicationSettings.cpp
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#include "ApplicationSettings.h"

QString ApplicationSettings::m_szCurrentExplorerPath;
bool ApplicationSettings::m_bWindowMaximized;

FavouriteDatabaseList ApplicationSettings::m_listFavourite;

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

const FavouriteDatabaseList ApplicationSettings::getFavouriteList()
{
	return m_listFavourite;
}

void ApplicationSettings::setFavouriteList(const FavouriteDatabaseList& databaseList)
{
	m_listFavourite = databaseList;
}

void ApplicationSettings::addFavouriteDatabase(const DatabaseModel& database)
{
	m_listFavourite.append(database);
}
