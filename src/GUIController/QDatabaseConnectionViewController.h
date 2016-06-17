/*
 * QDatabaseConnectionViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_

#include <QWidget>
#include <QStandardItemModel>

#include "Database/DatabaseController.h"

#include "GUI/QDatabaseConnectionView.h"

class QDatabaseConnectionView;

class QDatabaseConnectionViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseConnectionViewController(const QString& szFileName);
	virtual ~QDatabaseConnectionViewController();

	void init(QDatabaseConnectionView* pDatabaseConnectionView);

public slots:
	void openWorksheet();
	void openTableTab(const QModelIndex& index);
	void closeTab(const int& index);

	void updateTables();

private:
	static void onDbLoadTables(const QString& szTable, void* user_data);
	static void onDbLoadSystemTables(const QString& szTable, void* user_data);
	static void onDbLoadViewsTables(const QString& szTable, void* user_data);
	static void onDbLoadTableDescription(const QString& szName, const QString& szType, bool bNotNull, const QString& szDefaultValue, const QString& szPk, void* user_data);

private:
	QString m_szFileName;
	QDatabaseConnectionView* m_pDatabaseConnectionView;
	QString m_szTableName;
	QStandardItemModel* m_pListTableModel;
	DatabaseController* m_pDatabaseController;
};

#endif /* SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_ */
