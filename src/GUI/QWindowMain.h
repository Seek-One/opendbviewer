/*
 * MainWindow.h
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>
#include "Widget/QMidClickClosableTabWidget.h"

class QAction;
class QDatabaseConnectionView;
class QOpenDatabaseView;
class QTabWidget;

class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	QWindowMain(QWidget* parent = 0);
	virtual ~QWindowMain();

	QAction* getQuitAction() const;
	QAction* getAboutAction() const;
	QOpenDatabaseView* getOpenDatabaseView() const;
	QTabWidget* getDatabaseConnectionTab() const;

	void addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle);

protected:
	virtual void changeEvent(QEvent* pEvent);

private:
	void createMenu(); //Creates a menu bar

	QMidClickClosableTabWidget *m_pDatabaseConnectionTab;
	QOpenDatabaseView* m_pOpenDatabaseView;

	// List of actions
	QAction* m_pQuitAction;
	QAction* m_pAboutAction;
};

#endif /* SRC_MAINWINDOW_H_ */
