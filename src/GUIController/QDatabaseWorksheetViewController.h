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

class QDatabaseWorksheetView;

class QDatabaseWorksheetViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseWorksheetViewController();
	virtual ~QDatabaseWorksheetViewController();

	void init(QDatabaseWorksheetView* pDatabaseWorksheetView, DatabaseController* pDatabaseController);
	void showWorksheetQueryInformation();

public slots:
	void executeQuery();
	void reformatSqlText();
	void clearWorksheetText();
	void exportDataWorksheet();

private:
	QDatabaseWorksheetView* m_pDatabaseWorksheetView;
	DatabaseController* m_pDatabaseController;
	QSqlHighlighterController* m_pSqlHighlighterController;
	QSqlDisplayQueryModel* m_pDatabaseDisplayModel;

	bool m_bExportAvailable;
};

#endif /* SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_ */
