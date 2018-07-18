/*
 * ApplicationSettings.h
 *
 *  Created on: 20 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_GLOBAL_APPLICATIONSETTINGS_H_
#define SRC_GLOBAL_APPLICATIONSETTINGS_H_

#include <Model/HistoryDatabaseList.h>
#include <QString>
#include <QList>


class ApplicationSettings {
public:
	static const QString& getCurrentExplorerPath();
	static void setCurrentExplorerPath(const QString& szCurrentExplorerPath);

	static bool getWindowMaximized();
	static void setWindowMaximized(bool bWindowMaximized);

	static const HistoryDatabaseList getHistoryList();
	static void setHistoryList(const HistoryDatabaseList& databaseList);
	static void addHistoryDatabase(const DatabaseModel& database);

private:
	static QString m_szCurrentExplorerPath;
	static bool m_bWindowMaximized;

	static HistoryDatabaseList m_listHistory;
};

#endif /* SRC_GLOBAL_APPLICATIONSETTINGS_H_ */
