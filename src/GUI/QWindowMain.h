/*
 * MainWindow.h
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QApplication>
#include <QMessageBox>

#include "QDatabaseTableView.h"
#include "QDatabaseConnectionView.h"
#include "QDatabaseWorksheetView.h"

class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	QWindowMain(QWidget* parent = 0);
	virtual ~QWindowMain();

	QAction* getNewConnectionAction() const;


	void addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle);

private:
	void createMenu(); //Creates a menu bar

	QTabWidget *m_pDatabaseConnectionTab;

	// List of actions
	QAction* m_pNewConnectionAction;
};




#endif /* SRC_MAINWINDOW_H_ */
