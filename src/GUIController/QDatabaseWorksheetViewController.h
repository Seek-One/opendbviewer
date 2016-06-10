/*
 * QDatabaseWorksheetViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_

#include <GUI/QDatabaseWorksheetView.h>

class QDatabaseWorksheetView;

class QDatabaseWorksheetViewController
{
public:
	QDatabaseWorksheetViewController();
	virtual ~QDatabaseWorksheetViewController();

	void init(QDatabaseWorksheetView* pDatabaseWorksheetView);

private:
	QDatabaseWorksheetView* m_pDatabaseWorksheetView;
};

#endif /* SRC_GUICONTROLLER_QDATABASEWORKSHEETVIEWCONTROLLER_H_ */
