/*
 * QWindowMainController.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include <QObject>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>

#include "Model/DatabaseModel.h"
#include "Database/DatabaseController.h"
#include "QOpenDatabaseViewController.h"
#include "QOpenHistoryViewController.h"

class QOpenDatabaseViewController;
class QOpenHistoryViewController;
class QWidget;
class QWindowMain;

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);
	void callInitHistoryList();

public slots:
	void closeDatabaseConnectionTab(const int& index);
	void about();

	void callHistoryDatabaseLoading(const DatabaseModel& databaseModel);
	void callSQLiteFile(const QString& szFileUrl);
	void dragEnterReceived(QDragEnterEvent* pEvent);
	void dragLeaveReceived(QDragLeaveEvent* pEvent);
	void dropReceived(QDropEvent* pEvent);

	void openViews();
	void openHist();
	void openExplorer();
	void openMenuConn();

private:
	QWindowMain* m_pMainWindow;
	QOpenDatabaseViewController* m_pOpenDatabaseViewController;
	QOpenHistoryViewController* m_pOpenHistoryViewController;
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
