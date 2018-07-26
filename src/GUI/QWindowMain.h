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

#include "Widget/QFileExplorerWidget.h"
#include "Widget/QMidClickClosableTabWidget.h"

class QAction;
class QDatabaseConnectionView;
class QOpenDatabaseView;
class QOpenHistoryView;
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

	QTabWidget* getDatabaseConnectionTab() const;
	QOpenHistoryView* getOpenHistoryView() const;
	QFileExplorerWidget* getFileExplorerWidget() const;
	QOpenDatabaseView* getOpenDatabaseView() const;

	QWidget * makeExplorerTab(QWidget* pParent);
	QWidget* makeNoSelectionTab(QWidget* pParent);

	void addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle);
	void showViewsTab();
	void showHistoryTab();
	void showExplorerTab();
	void showMenuConnTab();

protected:
	virtual void changeEvent(QEvent* pEvent);

private:
	void createMenu(); //Creates a menu bar
	void createToolbar(); //Creates main toolbar

private:
	QStackedWidget * m_pStackedMenuWidget;
	QStackedWidget * m_pStackedDatabaseWidget;

	QMidClickClosableTabWidget *m_pDatabaseConnectionTab;

	QWidget * m_pExplorerWidget;
	QWidget * m_pNoSelectWidget;

	QOpenDatabaseView* m_pOpenDatabaseView;
	QOpenHistoryView* m_pOpenHistoryView;
	QToolBar * m_pWindowToolBar;

	// Explorer
	QFileExplorerWidget* m_pFileExplorerWidget;

	// List of actions
	QAction* m_pQuitAction;
	QAction* m_pAboutAction;

	QAction* m_pViewsAction;
	QAction* m_pHistAction;
	QAction* m_pExplorerAction;
	QAction* m_pNewConnAction;
};

#endif /* SRC_MAINWINDOW_H_ */
