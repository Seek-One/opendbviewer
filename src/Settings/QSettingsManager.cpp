/*
 * QSettingManager.cpp
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#include <QDir>

#include "Global/ApplicationSettings.h"
#include "Model/DatabaseModel.h"
#include "QSettingsManager.h"
#include "version.h"

QSettingsManager QSettingsManager::m_gInstance;

static const QString CONFIG_GROUP_GLOBAL = "global";
static const QString CONFIG_KEY_CURRENT_EXPLORER_PATH = "current_explorer_path";
static const QString CONFIG_KEY_WINDOW_MAXIMIZED = "window_maximized"; // "true" | "false"

static const QString DATABASE_GROUP_GLOBAL = "global";
static const QString DATABASE_KEY_DATABASE_COUNT = "database_count";
static const QString DATABASE_GROUP_DATABASE_ITER = "database_";
static const QString DATABASE_KEY_DATABASE_NAME = "name";
static const QString DATABASE_KEY_DATABASE_PATH = "path";
static const QString DATABASE_KEY_DATABASE_TYPE = "type";

QSettingsManager::QSettingsManager()
{
	m_pConfigSettings = new QSettings(QDir::homePath() + "/" + APPLICATION_PACKAGE_NAME + "/config.ini", QSettings::IniFormat);
	m_pDatabaseSettings = new QSettings(QDir::homePath() + "/" + APPLICATION_PACKAGE_NAME + "/database.ini", QSettings::IniFormat);
}

QSettingsManager::~QSettingsManager()
{
	if(m_pConfigSettings){
		delete m_pConfigSettings;
		m_pConfigSettings = NULL;
	}

	if(m_pDatabaseSettings){
		delete m_pDatabaseSettings;
		m_pDatabaseSettings = NULL;
	}
}

QSettingsManager& QSettingsManager::getInstance()
{
	return m_gInstance;
}

void QSettingsManager::loadConfigSettings()
{
	if(m_pConfigSettings){
		QVariant value = m_pConfigSettings->value(CONFIG_GROUP_GLOBAL + "/" + CONFIG_KEY_CURRENT_EXPLORER_PATH, QDir::homePath());
		if(!value.isNull()){
			ApplicationSettings::setCurrentExplorerPath(value.toString());
		}
		QVariant maximizedWindowValue = m_pConfigSettings->value(CONFIG_GROUP_GLOBAL + "/" + CONFIG_KEY_WINDOW_MAXIMIZED, false);
		if(!maximizedWindowValue.isNull()){
			ApplicationSettings::setWindowMaximized(maximizedWindowValue.toBool());
		}

	}
}

void QSettingsManager::saveConfigSettings()
{
	if(m_pConfigSettings){
		m_pConfigSettings->beginGroup(CONFIG_GROUP_GLOBAL);
		m_pConfigSettings->setValue(CONFIG_KEY_CURRENT_EXPLORER_PATH, ApplicationSettings::getCurrentExplorerPath());
		m_pConfigSettings->setValue(CONFIG_KEY_WINDOW_MAXIMIZED, ApplicationSettings::getWindowMaximized());
		m_pConfigSettings->endGroup();
	}
}

void QSettingsManager::loadDatabaseSettings()
{
	if(m_pDatabaseSettings){
		FavouriteDatabaseList favouriteList;

		int iDatabaseCount = 0;
		QVariant databaseCount = m_pDatabaseSettings->value(DATABASE_GROUP_GLOBAL + "/" + DATABASE_KEY_DATABASE_COUNT, 0);
		if(!databaseCount.isNull()){
			iDatabaseCount = databaseCount.toInt();
		}

		QString szGroup;
		QString szName;
		QString szPath;
		QString szType;
		QVariant databaseName;
		QVariant databasePath;
		QVariant databaseType;

		for(int i = 0; i < iDatabaseCount; ++i){
			szGroup = DATABASE_GROUP_DATABASE_ITER + QString::number(i);

			m_pDatabaseSettings->beginGroup(szGroup);
			databaseName = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_NAME, "");
			databasePath = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_PATH, "");
			databaseType = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_TYPE, "");
			m_pDatabaseSettings->endGroup();

			if(!databaseName.isNull()){
				szName = databaseName.toString();
			}
			if(!databasePath.isNull()){
				szPath = databasePath.toString();
			}
			if(!databaseType.isNull()){
				szType = databaseType.toString();
			}

			DatabaseModel databaseModel;
			databaseModel.setDatabaseName(szName);
			databaseModel.setDatabasePath(szPath);
			databaseModel.setDatabaseType(szType);
			favouriteList.append(databaseModel);
		}
		ApplicationSettings::setFavouriteList(favouriteList);
	}
}

void QSettingsManager::saveDatabaseSettings()
{
	if(m_pDatabaseSettings){
		FavouriteDatabaseList favouriteList = ApplicationSettings::getFavouriteList();
		DatabaseModel database;

		m_pDatabaseSettings->setValue(DATABASE_GROUP_GLOBAL + "/" + DATABASE_KEY_DATABASE_COUNT, favouriteList.size());

		for(int i = 0 ; i < favouriteList.size() ; i++){
			database = favouriteList.at(i);
			m_pDatabaseSettings->beginGroup(DATABASE_GROUP_DATABASE_ITER + QString::number(i) );
			m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_NAME, database.getDatabaseName());
			m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_PATH, database.getDatabasePath());
			m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_TYPE, database.getDatabaseType());
			m_pDatabaseSettings->endGroup();
		}
	}
}
