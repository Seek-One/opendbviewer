/*
 * QDatabaseConnectionView.h
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASECONNECTIONVIEW_H_
#define SRC_GUI_QDATABASECONNECTIONVIEW_H_

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QWidget>
#include <QToolBar>
#include <QPushButton>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QHeaderView>
#include <QTextEdit>
#include <QTableView>
#include <QApplication>
#include <QSplitter>
#include <QList>

#include "QDatabaseTableView.h"
#include "QDatabaseWorksheetView.h"

class QDatabaseConnectionView : public QWidget
{
public:
	QDatabaseConnectionView(QWidget* parent = 0);
	virtual ~QDatabaseConnectionView();

private:
	QWidget* makeWorksheetTab();
	QWidget* makeChannelTab();
	QWidget* makeStructureTable();
	QWidget* makeVerticalConnectionPannel();
	QToolBar* makeOptionButtonsToolBar();

private:
	QWidget *m_pTab2;

};

#endif /* SRC_GUI_QDATABASECONNECTIONVIEW_H_ */
