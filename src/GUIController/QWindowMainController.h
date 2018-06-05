/*
 * QWindowMainController.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include <QObject>

class QWidget;
class QWindowMain;

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);

public slots:
	void openNewDatabaseConnection();
	void closeDatabaseConnectionTab(const int& index);

	void about();

private:
	QWindowMain* m_pMainWindow;

	QWidget* makeTab();
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */