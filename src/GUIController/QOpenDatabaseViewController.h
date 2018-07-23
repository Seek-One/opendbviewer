/*
 * QOpenDatabaseViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QOPENDATABASEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QOPENDATABASEVIEWCONTROLLER_H_

#include <QObject>
#include <QString>

#include "Model/DatabaseModel.h"
#include "Database/DatabaseController.h"

class QDatabaseConnectionViewController;
class QOpenDatabaseView;
class QWindowMain;

class QOpenDatabaseViewController : public QObject
{
	Q_OBJECT
public:
	QOpenDatabaseViewController();
	virtual ~QOpenDatabaseViewController();

	void init(QWindowMain* pMainWindow, QOpenDatabaseView* pOpenDatabaseView);
	QString getFileUrl() const;
	void initHistoryList();
	DatabaseModel selectDatabase(DatabaseModel::DatabaseType type);
	void loadDatabase(const DatabaseModel&  databaseModel);

private:
	void recoverMySQLDatabaseInfo(DatabaseModel * database);
	void recoverPostgreSQLInfo(DatabaseModel * database);

public slots:
	void prepareConnection(DatabaseModel::DatabaseType type);
	void openFileDialog();
	void openSQLiteFile(const QString& szFileUrl);
	void setHistoryInfo(const QString& szPath);

	void closeOpenDatabaseDialog();
	void openViews();
	void openHist();
	void openExplorer();
	void openMenuConn();
	void openSQLite();
	void openMySQL();
	void openPostgreSQL();

private:
	QOpenDatabaseView* m_pOpenDatabaseView;
	QString m_szFileUrl;
	QWindowMain* m_pMainWindow;
};

#endif /* SRC_GUICONTROLLER_QOPENDATABASEVIEWCONTROLLER_H_ */
