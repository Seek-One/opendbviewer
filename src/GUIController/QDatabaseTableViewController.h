/*
 * QDatabaseTableViewController.h
 *
 *  Created on: 14 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_

#include <QWidget>
#include <QList>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

#include "Database/DatabaseCallback.h"
#include "GUIModel/QSqlDisplayTableModel.h"

class QStandardItem;
class QDatabaseTableView;
class DatabaseController;

class QDatabaseTableViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseTableViewController();
	virtual ~QDatabaseTableViewController();

	void init(QDatabaseTableView* pDatabaseTableView, QString& szTableName, DatabaseController* pDatabaseController);
	void showQueryInformation();

public:
	bool loadDatabaseTableInfos();
	bool loadDatabaseTableData();

public slots:
	void updateTableData();
	void clearFilterField();

private:
	static void onDbLoadTableDescription(const QStringList& listRowHeader, const QStringList& listRowData, DatabaseQueryStep step, void* user_data);
	static void onDbLoadTableCreationScript(const QString& szCreationScriptString, void* user_data);

	QList<QStandardItem*> makeStandardItemListFromStringList(const QList<QString>& szStringList);

private:
	QDatabaseTableView* m_pDatabaseTableView;
	QString m_szTableName;
	DatabaseController* m_pDatabaseController;
	QSqlDisplayTableModel* m_pDatabaseTableModel;
};

#endif /* SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_ */
