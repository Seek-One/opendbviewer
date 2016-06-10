/*
 * QDatabaseConnectionViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_

#include <QWidget>

#include "GUI/QDatabaseConnectionView.h"

class QDatabaseConnectionView;

class QDatabaseConnectionViewController : public QWidget
{
	Q_OBJECT
public:
	QDatabaseConnectionViewController();
	virtual ~QDatabaseConnectionViewController();

	void init(QDatabaseConnectionView* pDatabaseConnectionView);

public slots:
	void openWorksheet();
	void closeTab(const int& index);

private:
	QDatabaseConnectionView* m_pDatabaseConnectionView;
};

#endif /* SRC_GUICONTROLLER_QDATABASECONNECTIONVIEWCONTROLLER_H_ */
