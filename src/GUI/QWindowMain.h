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


class QWindowMain : public QMainWindow
{
public:
	QWindowMain();

private:
	void createMenu(); //Creates a menu bar
	void applyLayout();
	QWidget* makeDBTab();
	QTabWidget* makeWorksheetTab();
	QTabWidget* makeChannelTab();
	QTabWidget* makeDataTab();
};




#endif /* SRC_MAINWINDOW_H_ */
