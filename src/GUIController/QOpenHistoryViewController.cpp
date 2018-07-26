/*
 * QOpenHistoryViewController.cpp
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#include "QOpenHistoryViewController.h"
#include "Global/ApplicationSettings.h"
#include "GUIModel/QHistoryDatabaseModel.h"

QOpenHistoryViewController::QOpenHistoryViewController(QWindowMain* pMainWindow, QWindowMainController* pMainWindowController)
{
	m_pMainWindow = pMainWindow;
	m_pMainWindowController = pMainWindowController;
	m_pOpenHistoryView = pMainWindow->getOpenHistoryView();
	m_pHistoryDatabaseModel = new QHistoryDatabaseModel();

	m_pOpenHistoryView->getHistoryTreeView()->setModel(m_pHistoryDatabaseModel);
	initHistoryList();
	connect(m_pOpenHistoryView, SIGNAL(openHistorySQLiteDatabase(const QModelIndex&)), this, SLOT(openSQLiteHistoryFile(const QModelIndex&)));
	connect(m_pOpenHistoryView, SIGNAL(openHistoryInfo(const QModelIndex&)), this, SLOT(setHistoryInfo(const QModelIndex&)));
}

QOpenHistoryViewController::~QOpenHistoryViewController()
{
}

void QOpenHistoryViewController::setHistoryInfo(const QModelIndex& index)
{
	//TODO Switch between types
	QString qElidedText, szFName;
	QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(m_pHistoryDatabaseModel->itemFromIndex(index));
	DatabaseModel databaseModel = pItem->getDatabaseModel();

	int iWidth = 220;

	if (m_pOpenHistoryView->getHistoryInfoWidget()->width()>iWidth) {
		iWidth = m_pOpenHistoryView->getHistoryInfoWidget()->width()-10;
	}
	szFName = databaseModel.getDatabasePath().section('/', -1);
	qElidedText = databaseModel.getDatabasePath().section('/', 0, -2);

	QFontMetrics metrics(m_pOpenHistoryView->getHistoryPathLabel()->font());
	qElidedText = metrics.elidedText(qElidedText, Qt::ElideLeft, iWidth);

	m_pOpenHistoryView->getHistoryNameLabel()->setText(szFName);
	m_pOpenHistoryView->getHistoryPathLabel()->setText(qElidedText);
}

void QOpenHistoryViewController::initHistoryList()
{
	HistoryDatabaseList list = ApplicationSettings::getHistoryList();
	DatabaseModel database;

	m_pHistoryDatabaseModel->clear();
	for (int row = list.size() - 1 ; row >= 0 ; row--){
		database = list.at(row);
		QHistoryDatabaseItem *item = new QHistoryDatabaseItem(database);
		item->setEditable(false);
		m_pHistoryDatabaseModel->appendRow(item);
	}
}

void QOpenHistoryViewController::openSQLiteHistoryFile(const QModelIndex& index)
{
	QString szPath;
	QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(m_pHistoryDatabaseModel->itemFromIndex(index));
	DatabaseModel databaseModel = pItem->getDatabaseModel();
	szPath = databaseModel.getDatabasePath();

	m_pMainWindowController->callSQLiteFile(szPath);
}
