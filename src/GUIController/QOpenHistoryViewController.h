/*
 * QOpenHistoryViewController.h
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUICONTROLLER_QOPENHISTORYVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QOPENHISTORYVIEWCONTROLLER_H_

#include "GUI/QOpenHistoryView.h"
#include "GUI/QWindowMain.h"
#include "GUIModel/QHistoryDatabaseItem.h"
#include "GUIModel/QHistoryDatabaseModel.h"
#include "QWindowMainController.h"
#include "Model/HistoryDatabaseList.h"

class QWindowMain;
class QWindowMainController;
class QOpenHistoryView;

class QOpenHistoryViewController : public QObject
{
	Q_OBJECT
public:
	QOpenHistoryViewController(QWindowMain* pMainWindow, QWindowMainController* pMainWindowController);
	virtual ~QOpenHistoryViewController();
	void initHistoryList();

public slots:
	void openSQLiteHistoryFile(const QModelIndex& index);
	void setHistoryInfo(const QModelIndex& index);

private:
	QWindowMain * m_pMainWindow;
	QWindowMainController * m_pMainWindowController;
	QOpenHistoryView * m_pOpenHistoryView;
	QHistoryDatabaseModel * m_pHistoryDatabaseModel;

};

#endif /* SRC_GUICONTROLLER_QOPENHISTORYVIEWCONTROLLER_H_ */
