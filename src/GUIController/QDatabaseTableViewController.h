/*
 * QDatabaseTableViewController.h
 *
 *  Created on: 14 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_

#include <GUI/QDatabaseTableView.h>
#include "Database/DatabaseController.h"

class QDatabaseTableView;

class QDatabaseTableViewController
{
public:
	QDatabaseTableViewController();
	virtual ~QDatabaseTableViewController();

	void init(QDatabaseTableView* pDatabaseTableView, QString& szTableName, DatabaseController* pDatabaseController);
private:
	static void onDbLoadTableDescription(const QString& szName, const QString& szType, bool bNotNull, const QString& szDefaultValue, const QString& szPk, void* user_data);
	static void onDbLoadTableData(const QList<QString>& pColumnName, const QList<QString>& pRowData, void* user_data);
private:
	QDatabaseTableView* m_pDatabaseTableView;
	QString m_szTableName;
	DatabaseController* m_pDatabaseController;
	QString m_szFieldName;
};

#endif /* SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_ */
