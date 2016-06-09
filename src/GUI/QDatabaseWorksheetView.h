/*
 * QDatabaseWorksheetView.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASEWORKSHEETVIEW_H_
#define SRC_GUI_QDATABASEWORKSHEETVIEW_H_

#include <QPushButton>
#include <QToolBar>
#include <QTextEdit>
#include <QTableView>

#include "QDatabaseTableView.h"

class QDatabaseWorksheetView : public QWidget
{
public:
	QDatabaseWorksheetView(QWidget* parent = 0);
	virtual ~QDatabaseWorksheetView();

private:
	QToolBar* makeWorksheetToolbar();

};

#endif /* SRC_GUI_QDATABASEWORKSHEETVIEW_H_ */
