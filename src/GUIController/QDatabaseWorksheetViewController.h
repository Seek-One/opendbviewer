/*
 * QDatabaseWorksheetViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_

#include "Database/DatabaseController.h"

#include "GUI/QDatabaseWorksheetView.h"

#include <QDebug>
#include <QTextCursor>
#include <QJsonDocument>

class QDatabaseWorksheetView;

class QDatabaseWorksheetViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseWorksheetViewController();
	virtual ~QDatabaseWorksheetViewController();

	void init(QDatabaseWorksheetView* pDatabaseWorksheetView, DatabaseController* pDatabaseController);
	void showWorksheetQueryInformation();

	void updateRequestHistory();
	void setRequestHistory();
	void setJsonDatabases();
	void addRequestHistoryToJson(QString szWorksheetQuery);

public slots:
	void executeQuery();
	void reformatSqlText();
	void clearWorksheetText();
	void exportDataWorksheet();
	void setExportButtonDisabled();
	void changeWorksheetTextFromHistory(QAction* action);

private:
	QDatabaseWorksheetView* m_pDatabaseWorksheetView;
	DatabaseController* m_pDatabaseController;
	QSqlHighlighterController* m_pSqlHighlighterController;
	QSqlDisplayQueryModel* m_pDatabaseDisplayModel;

	QString m_szDatabasesJsonPath;
	QString m_szDatabasesFilePath;
};

#endif /* SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_ */
