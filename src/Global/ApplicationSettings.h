/*
 * ApplicationSettings.h
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_GLOBAL_APPLICATIONSETTINGS_H_
#define SRC_GLOBAL_APPLICATIONSETTINGS_H_

#include <QString>
#include <QList>

#include "Model/FavouriteDatabaseList.h"

class ApplicationSettings {
public:
	static const QString& getCurrentExplorerPath();
	static void setCurrentExplorerPath(const QString& szCurrentExplorerPath);

	static bool getWindowMaximized();
	static void setWindowMaximized(bool bWindowMaximized);

	static const FavouriteDatabaseList getFavouriteList();
	static void setFavouriteList(const FavouriteDatabaseList& databaseList);
	static void addFavouriteDatabase(const DatabaseModel& database);

private:
	static QString m_szCurrentExplorerPath;
	static bool m_bWindowMaximized;

	static FavouriteDatabaseList m_listFavourite;
};

#endif /* SRC_GLOBAL_APPLICATIONSETTINGS_H_ */
