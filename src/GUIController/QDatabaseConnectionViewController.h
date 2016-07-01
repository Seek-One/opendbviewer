/*
 * QDatabaseConnectionViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_

#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerSqlite.h"
#include "GUI/QDatabaseConnectionView.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>

class QDatabaseConnectionView;

class QDatabaseConnectionViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseConnectionViewController(const QString& szFileName, DatabaseController* pDatabaseController);
	virtual ~QDatabaseConnectionViewController();

	void init(QDatabaseConnectionView* pDatabaseConnectionView, QStringList& szDatabaseInfoList);

public slots:
	void openWorksheet();
	void refreshList();
	void openTableTab(const QModelIndex& index);
	void closeTab(const int& index);

	void updateTables();

private:
	static void onDbLoadTables(const QString& szTable, void* user_data);
	static void onDbLoadSystemTables(const QString& szTable, void* user_data);
	static void onDbLoadViewsTables(const QString& szTable, void* user_data);

private:
	QString m_szFileName;
	QDatabaseConnectionView* m_pDatabaseConnectionView;
	QStandardItemModel* m_pListTableModel;
	DatabaseController* m_pDatabaseController;
};

#endif /* SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_ */
