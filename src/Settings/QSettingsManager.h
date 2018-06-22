/*
 * QSettingManager.h
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_SETTINGS_QSETTINGSMANAGER_H_
#define SRC_SETTINGS_QSETTINGSMANAGER_H_

#include <QSettings>

class QSettingsManager {
private:
	QSettingsManager();

public:
	virtual ~QSettingsManager();

public:
	static QSettingsManager& getInstance();

public:
	void loadSettings();
	void saveSettings();

private:
	QSettings* m_pSettingsCurrentExplorerPath;

	static QSettingsManager m_gInstance;
};

#endif /* SRC_SETTINGS_QSETTINGSMANAGER_H_ */
