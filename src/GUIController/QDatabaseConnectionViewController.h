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
	void closeTab(const int& index);

	void updateTables();

private:
	static void onDbLoadTable(const QString& szTable, void* user_data);

private:
	QString m_szFileName;
	QDatabaseConnectionView* m_pDatabaseConnectionView;

	QStandardItemModel* m_pListTableModel;
};

#endif /* SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_ */
