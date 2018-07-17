/*
 * MainWindow.h
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>
#include <QStackedWidget>

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

	QAction* getViewsAction() const;
	QAction* getHistAction() const;
	QAction* getExplorerAction() const;
	QAction* getNewConnAction() const;

	QAction* getSQLiteAction() const;
	QAction* getMySQLAction() const;
	QAction* getPostgreSQLAction() const;

	QOpenDatabaseView* getOpenDatabaseView() const;
	QTabWidget* getDatabaseConnectionTab() const;

	void addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle);
	void showViewsTab();
	void showHistoryTab();
	void showExplorerTab();
	void showNewConnMenuTab();
	void OpenSQLiteTab();
	void OpenMySQLTab();
	void OpenPostgreSQLTab();

protected:
	virtual void changeEvent(QEvent* pEvent);

private:
	void createMenu(); //Creates a menu bar
	void createToolbar(); //Creates main toolbar

	QMidClickClosableTabWidget *m_pDatabaseConnectionTab;
	QStackedWidget * m_pStackedMenuWidget;

	QWidget * m_pHistWidget;
	QWidget * m_pNewConnWidget;
	QWidget * m_pExplorerWidget;
	QWidget * m_pSQLiteWidget;
	QWidget * m_pMySQLWidget;
	QWidget * m_pPSQLWidget;

	QOpenDatabaseView* m_pOpenDatabaseView;
	QToolBar * m_pWindowToolBar;

	// List of actions
	QAction* m_pQuitAction;
	QAction* m_pAboutAction;

	QAction* m_pViewsAction;
	QAction* m_pHistAction;
	QAction* m_pExplorerAction;
	QAction* m_pNewConnAction;

	QAction* m_pSQLiteAction;
	QAction* m_pMySQLAction;
	QAction* m_pPostgreSQLAction;
};

#endif /* SRC_MAINWINDOW_H_ */
