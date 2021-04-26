/*
 * QSettingManager.h
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_SETTINGS_QSETTINGSMANAGER_H_
#define SRC_SETTINGS_QSETTINGSMANAGER_H_

#include <QSettings>
#include <QDir>

#include "GUI/QDatabaseWorksheetView.h"

class QSettingsManager {
private:
	QSettingsManager();

public:
	virtual ~QSettingsManager();

public:
	static QSettingsManager& getInstance();

public:
	void loadConfigSettings();
	void saveConfigSettings();

	void loadDatabaseSettings();
	void saveDatabaseSettings();

	QString getDatabasesJsonDir() const;
	QString getDatabasesJsonFile() const;

private:
	QSettings* m_pConfigSettings;
	QSettings* m_pDatabaseSettings;

	QString m_szDatabasesFilePath;
	QDir m_dirDatabases;

	static QSettingsManager m_gInstance;
};

#endif /* SRC_SETTINGS_QSETTINGSMANAGER_H_ */
