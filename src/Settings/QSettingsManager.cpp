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
static const QString DATABASE_KEY_DATABASE_USER = "username";
static const QString DATABASE_KEY_DATABASE_HOST = "host";
static const QString DATABASE_KEY_DATABASE_PORT = "port";

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
		HistoryDatabaseList historyList;

		int iDatabaseCount = 0;
		QVariant databaseCount = m_pDatabaseSettings->value(DATABASE_GROUP_GLOBAL + "/" + DATABASE_KEY_DATABASE_COUNT, 0);
		if(!databaseCount.isNull()){
			iDatabaseCount = databaseCount.toInt();
		}

		QString szGroup;
		QString szName;
		DatabaseModel::DatabaseType type;
		QString szPath;
		QString szHost;
		int iPort;
		QString szUsername;

		QVariant databaseName;
		QVariant databasePath;
		QVariant databaseType;
		QVariant databaseHost;
		QVariant databasePort;
		QVariant databaseUsername;

		for(int i = 0; i < iDatabaseCount; ++i){
			szGroup = DATABASE_GROUP_DATABASE_ITER + QString::number(i);

			m_pDatabaseSettings->beginGroup(szGroup);
			databaseName = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_NAME, "");
			databaseType = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_TYPE, "");
			databasePath = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_PATH, "");
			databaseHost = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_HOST, "");
			databasePort = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_PORT, "");
			databaseUsername = m_pDatabaseSettings->value(DATABASE_KEY_DATABASE_USER, "");
			m_pDatabaseSettings->endGroup();

			DatabaseModel databaseModel;
			if(!databaseName.isNull()){
				szName = databaseName.toString();
			}
			if(!databaseType.isNull()){
				type = (DatabaseModel::DatabaseType)databaseType.toInt();
			} else {
				type = DatabaseModel::UnknownType;
			}
			if(!databasePath.isNull()){
				szPath = databasePath.toString();
			}
			if(!databaseHost.isNull()){
				szHost = databaseHost.toString();
			}
			if(!databasePort.isNull()){
				iPort = databasePort.toInt();
			}
			if(!databaseUsername.isNull()){
				szUsername = databaseUsername.toString();
			}

			databaseModel.setDatabaseName(szName);
			databaseModel.setDatabaseType(type);

			switch(type) {
			case DatabaseModel::SQLiteType:
				databaseModel.setDatabasePath(szPath);
				break;
			case DatabaseModel::MySQLType:
				databaseModel.setDatabaseHost(szHost);
				databaseModel.setDatabasePort(iPort);
				databaseModel.setDatabaseUsername(szUsername);
				break;
			case DatabaseModel::PostgreSQLType:
				databaseModel.setDatabaseHost(szHost);
				databaseModel.setDatabasePort(iPort);
				databaseModel.setDatabaseUsername(szUsername);
				break;
			default:
				break;
			}
			historyList.append(databaseModel);
		}
		ApplicationSettings::setHistoryList(historyList);
	}
}

void QSettingsManager::saveDatabaseSettings()
{
	if(m_pDatabaseSettings){
		HistoryDatabaseList historyList = ApplicationSettings::getHistoryList();
		DatabaseModel database;
		DatabaseModel::DatabaseType type;


		m_pDatabaseSettings->setValue(DATABASE_GROUP_GLOBAL + "/" + DATABASE_KEY_DATABASE_COUNT, historyList.size());

		for(int i = 0 ; i < historyList.size() ; i++){
			database = historyList.at(i);
			type = database.getDatabaseType();
			m_pDatabaseSettings->beginGroup(DATABASE_GROUP_DATABASE_ITER + QString::number(i) );
			m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_NAME, database.getDatabaseName());
			m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_TYPE, database.getDatabaseType());

			switch(type) {
			case DatabaseModel::SQLiteType:
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_PATH, database.getDatabasePath());
				break;
			case DatabaseModel::MySQLType:
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_HOST, database.getDatabaseHost());
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_USER, database.getDatabaseUsername());
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_PORT, database.getDatabasePort());
				break;
			case DatabaseModel::PostgreSQLType:
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_HOST, database.getDatabaseHost());
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_USER, database.getDatabaseUsername());
				m_pDatabaseSettings->setValue(DATABASE_KEY_DATABASE_PORT, database.getDatabasePort());
				break;
			default:
				break;
			}
			m_pDatabaseSettings->endGroup();
		}
	}
}
