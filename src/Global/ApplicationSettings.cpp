/*
 * ApplicationSettings.cpp
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#include "ApplicationSettings.h"

QString ApplicationSettings::m_szCurrentExplorerPath;

const QString& ApplicationSettings::getCurrentExplorerPath()
{
	return m_szCurrentExplorerPath;
}

void ApplicationSettings::setCurrentExplorerPath(const QString& szCurrentExplorerPath)
{
	m_szCurrentExplorerPath = szCurrentExplorerPath;
}
