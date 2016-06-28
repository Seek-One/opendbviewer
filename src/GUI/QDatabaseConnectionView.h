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
#include <QAction>

#include "QDatabaseTableView.h"
#include "QDatabaseWorksheetView.h"

class QDatabaseConnectionView : public QWidget
{
public:
	QDatabaseConnectionView(QWidget* parent = 0);
	virtual ~QDatabaseConnectionView();

	QPushButton* getNewWorksheetButton() const;
	QAction* getNewWorksheetAction() const;
	QPushButton* getRefreshTableListButton() const;
	QTabWidget* getTabsInConnection() const;
	QWidget* getWorksheetTab() const;
	QTreeView *getTableTreeView() const;
	QStandardItem* getTableItem() const;
	QStandardItem* getStructureTableItem() const;
	QStandardItem* getViewTableItem() const;
	void addWorksheetView(QDatabaseWorksheetView* pDatabaseWorksheetView, const QString& szTitle);
	void addTableView(QDatabaseTableView* pDatabaseTableView, const QString& szTableTitle);

	void setTablesModel(QStandardItemModel* pModel);

private:
	QWidget* makeWorksheetTab();
	QWidget* makeVerticalConnectionPanel();
	QToolBar* makeOptionButtonsToolBar();

private:
	QWidget *m_pTab1;
	QPushButton *m_pNewWorksheetButton;
	QPushButton *m_pRefreshTableListButton;
	QAction* m_pNewWorksheetAction;
	QTabWidget *m_pTabsInConnection;
	QTreeView *m_pTableTreeView;
	QStandardItem* m_pTableItem;
	QStandardItem* m_pStructureTableItem;
	QStandardItem* m_pViewItem;
};

#endif /* SRC_GUI_QDATABASECONNECTIONVIEW_H_ */
