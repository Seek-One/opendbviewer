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

class DatabaseController;
class QDatabaseConnectionViewController;
class QOpenDatabaseView;
class QWindowMain;

class QOpenDatabaseViewController : public QObject
{
	Q_OBJECT
public:
	enum TabIndex {
		FavouriteIndex = 0,
		ExplorerFileIndex,
		SQLiteIndex,
		MySQLIndex,
		PostgreSQLIndex,
	};

	enum DatabaseType {
		SQLiteType = 0,
		MySQLType,
		PostgreSQLType,
	};

public:
	QOpenDatabaseViewController();
	virtual ~QOpenDatabaseViewController();

	void init(QWindowMain* pMainWindow, QOpenDatabaseView* pOpenDatabaseView);
	QString getFileUrl() const;
	void initFavouriteList();

private:
	QStringList makeMySQLDatabaseInfoList();
	QStringList makePostgreSQLDatabaseInfoList();

public slots:
	void openFileDialog();
	void openSQLiteFile(const QString& szFileUrl);
	void closeOpenDatabaseDialog();
	void loadDatabase();

private:
	QOpenDatabaseView* m_pOpenDatabaseView;
	QString m_szFileUrl;
	QWindowMain* m_pMainWindow;
};

#endif /* SRC_GUICONTROLLER_QOPENDATABASEVIEWCONTROLLER_H_ */
