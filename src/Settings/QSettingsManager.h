/*
 * QSettingManager.h
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_SETTINGS_QSETTINGSMANAGER_H_
#define SRC_SETTINGS_QSETTINGSMANAGER_H_

#include <QSettings>
#include <QJsonDocument>

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

	QString getStringDatabaseId(QString szDatabaseIdentifier) const;
	QString getDatabasesJsonDir() const;
	QString getDatabasesJson() const;

public:
	static bool writeToFile(QString szFilePath, QByteArray szText);
	static QJsonDocument parseToJsonDocument(QString szFile);

private:
	QSettings* m_pConfigSettings;
	QSettings* m_pDatabaseSettings;
	QSettings* m_pDatabasesJsonSettings;
	QDir m_szDatabasesJsonDir;

	static QSettingsManager m_gInstance;
};

#endif /* SRC_SETTINGS_QSETTINGSMANAGER_H_ */
