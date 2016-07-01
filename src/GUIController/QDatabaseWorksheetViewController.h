/*
 * QDatabaseWorksheetViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_

#include "GUI/QDatabaseWorksheetView.h"
#include "Database/DatabaseController.h"

#include <QDebug>
#include <QTextCursor>

class QDatabaseWorksheetView;

class QDatabaseWorksheetViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseWorksheetViewController();
	virtual ~QDatabaseWorksheetViewController();

	void init(QDatabaseWorksheetView* pDatabaseWorksheetView, QString& szFileName, DatabaseController* pDatabaseController);
	void showWorksheetQueryInformation();

public slots:
	void executeQuery();
	void reformatSqlText();
	void clearWorksheetText();

private:
	static void onDbLoadWorksheetQueryResults(const QList<QString>& pColumnNameList, const QList<QString>& pRowData, void* user_data);

private:
	QDatabaseWorksheetView* m_pDatabaseWorksheetView;
	QString m_szFileName;
	DatabaseController* m_pDatabaseController;
};

#endif /* SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_ */
