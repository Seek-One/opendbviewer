/*
 * QWindowMainController.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include <QObject>

#include "QWindowMainController.h"

#include "GUI/QWindowMain.h"
#include "GUI/QDatabaseConnectionView.h"

class QWindowMain;

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);

public slots:
	void newDatabaseConnection();
	void about();
	void closeConnectionTab(const int& index);

private:
	QWindowMain* m_pMainWindow;
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
