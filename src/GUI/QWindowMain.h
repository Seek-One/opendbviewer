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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include <QApplication>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include <QTreeView>
#include <QToolBar>
#include <QTextEdit>

#include "QDatabaseTableView.h"
#include "QDatabaseConnectionView.h"

class QWindowMain : public QMainWindow
{
	Q_OBJECT
public:
	QWindowMain(QWidget* parent = 0);
	virtual ~QWindowMain();

public slots:
	void newDatabaseConnection();

private:
	void createMenu(); //Creates a menu bar


	QTabWidget *m_pDatabaseConnectionTab;
};




#endif /* SRC_MAINWINDOW_H_ */
