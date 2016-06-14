/*
 * DatabaseController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_DATABASE_DATABASECONTROLLER_H_
#define SRC_DATABASE_DATABASECONTROLLER_H_

#include <QSqlDatabase>
#include <QListView>

typedef void (*DbLoadTableCB)(const QString& szTable, void* user_data);

class DatabaseController
{
public:
	DatabaseController(const QString& szFilename);
	virtual ~DatabaseController();

	QList<QString> getTableList() const;
	bool loadTables(DbLoadTableCB func, void* user_data);

private:
	QString m_szFilename;
	QSqlDatabase m_db;

	QList<QString> m_tableList;

	bool openDatabase();
	void closeDataBase();
};

#endif /* SRC_DATABASE_DATABASECONTROLLER_H_ */
