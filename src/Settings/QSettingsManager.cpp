/*
 * QSettingManager.cpp
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#include <QDir>

#include "Global/ApplicationSettings.h"
#include "QSettingsManager.h"
#include "version.h"

QSettingsManager QSettingsManager::m_gInstance;

static const QString KEY_GROUP_GENERAL = "global";

static const QString KEY_CURRENT_EXPLORER_PATH = "current_explorer_path";

QSettingsManager::QSettingsManager()
{
	m_pSettingsCurrentExplorerPath = new QSettings(QDir::homePath() + "/" + APPLICATION_PACKAGE_NAME + "/config.ini", QSettings::IniFormat);
}

QSettingsManager::~QSettingsManager()
{
	if(m_pSettingsCurrentExplorerPath){
		delete m_pSettingsCurrentExplorerPath;
		m_pSettingsCurrentExplorerPath = NULL;
	}
}

QSettingsManager& QSettingsManager::getInstance()
{
	return m_gInstance;
}

void QSettingsManager::loadSettings()
{
	if(m_pSettingsCurrentExplorerPath){
		QVariant value = m_pSettingsCurrentExplorerPath->value(KEY_GROUP_GENERAL + "/" + KEY_CURRENT_EXPLORER_PATH, QDir::homePath());
		if(!value.isNull()){
			ApplicationSettings::setCurrentExplorerPath(value.toString());
		}
	}
}

void QSettingsManager::saveSettings()
{
	if(m_pSettingsCurrentExplorerPath){
		m_pSettingsCurrentExplorerPath->beginGroup(KEY_GROUP_GENERAL);
		m_pSettingsCurrentExplorerPath->setValue(KEY_CURRENT_EXPLORER_PATH, ApplicationSettings::getCurrentExplorerPath());
		m_pSettingsCurrentExplorerPath->endGroup();
	}
}
