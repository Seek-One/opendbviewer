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

#include "Database/DatabaseController.h"
#include "QWindowMainController.h"

class QWindowMainController;
class QDatabaseConnectionViewController;
class QOpenDatabaseView;
class QWindowMain;

class QOpenDatabaseViewController : public QObject
{
	Q_OBJECT
public:
	QOpenDatabaseViewController(QWindowMain* pMainWindow, QWindowMainController* pMainWindowController);
	virtual ~QOpenDatabaseViewController();

	QString getFileUrl() const;
	DatabaseModel selectDatabase(DatabaseModel::DatabaseType type);
	void loadDatabase(const DatabaseModel&  databaseModel);

private:
	void recoverMySQLDatabaseInfo(DatabaseModel * database);
	void recoverPostgreSQLInfo(DatabaseModel * database);

public slots:
	void openFileDialog();
	void closeOpenDatabaseDialog();

	void prepareConnection(DatabaseModel::DatabaseType type);
	void openSQLiteFile(const QString& szFileUrl);

	void openSQLite();
	void openMySQL();
	void openPostgreSQL();

private:
	QWindowMain* m_pMainWindow;
	QWindowMainController* m_pMainWindowController;
	QOpenDatabaseView* m_pOpenDatabaseView;
	QString m_szFileUrl;
};

#endif /* SRC_GUICONTROLLER_QOPENDATABASEVIEWCONTROLLER_H_ */
