/*
 * ApplicationSettings.h
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_GLOBAL_APPLICATIONSETTINGS_H_
#define SRC_GLOBAL_APPLICATIONSETTINGS_H_

#include <QString>

class ApplicationSettings {
public:
	static const QString& getCurrentExplorerPath();
	static void setCurrentExplorerPath(const QString& szCurrentExplorerPath);

private:
	static QString m_szCurrentExplorerPath;
};

#endif /* SRC_GLOBAL_APPLICATIONSETTINGS_H_ */
