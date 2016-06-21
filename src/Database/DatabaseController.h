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
#include <QStandardItem>

typedef void (*DbLoadTableCB)(const QString& szTable, void* user_data);
typedef void (*DbLoadTableDescription)(const QString& szName, const QString& szType, bool bNotNull, const QString& szDefaultValue, const QString& szPk, void* user_data);
typedef void (*DbLoadTableData)(const QList<QString>& pColumnName, const QList<QString>& pRowData, void* user_data);

class DatabaseController
{
public:
	DatabaseController(const QString& szFilename);
	virtual ~DatabaseController();

	bool loadTables(DbLoadTableCB func, void* user_data);
	bool loadSystemTables(DbLoadTableCB func, void* user_data);
	bool loadViewsTables(DbLoadTableCB func, void* user_data);
	bool loadTableDescription(const QString& szTable, DbLoadTableDescription func, void* user_data);
	bool loadTableData(const QString& szTableName, DbLoadTableData func, void* user_data);

private:
	QString m_szFilename;
	QSqlDatabase m_db;
	QStringList m_szListColumnName;

	bool openDatabase();
	void closeDataBase();
};

#endif /* SRC_DATABASE_DATABASECONTROLLER_H_ */
