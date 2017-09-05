/*
 * QDatabaseTableViewController.h
 *
 *  Created on: 14 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_

#include "GUI/QDatabaseTableView.h"
#include "Database/DatabaseController.h"
#include <QObject>
#include <QTextCursor>
#include <QDebug>

class QDatabaseTableView;

class QDatabaseTableViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseTableViewController();
	virtual ~QDatabaseTableViewController();

	void init(QDatabaseTableView* pDatabaseTableView, QString& szTableName, DatabaseController* pDatabaseController);
	void showQueryInformation();
	QList<QStandardItem*> makeStandardItemListFromStringList(const QList<QString>& szStringList);
public slots:
	void updateView();
	void clearFilterField();

private:
	static void onDbLoadTableDescription(const QList<QString>& listRowHeader, const QList<QString>& listRowData, void* user_data);
	static void onDbLoadTableData(const QList<QString>& listRowHeader, const QList<QString>& listRowData, void* user_data);
	static void onDbLoadTableCreationScript(const QString& szCreationScriptString, void* user_data);

private:
	QDatabaseTableView* m_pDatabaseTableView;
	QString m_szTableName;
	DatabaseController* m_pDatabaseController;
	QString m_szFieldName;
};

#endif /* SRC_GUICONTROLLER_QDATABASETABLEVIEWCONTROLLER_H_ */
