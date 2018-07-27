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
	HTMLDelegate* delegate = new HTMLDelegate();

	m_pOpenHistoryView->getHistoryTreeView()->setModel(m_pHistoryDatabaseModel);
	m_pOpenHistoryView->getHistoryTreeView()->setItemDelegate(delegate);
	initHistoryList();
	connect(m_pOpenHistoryView, SIGNAL(openHistorySQLiteDatabase(const QModelIndex&)), this, SLOT(openSQLiteHistoryFile(const QModelIndex&)));
	connect(m_pOpenHistoryView, SIGNAL(openHistoryInfo(const QModelIndex&)), this, SLOT(getHistoryInfo(const QModelIndex&)));
	connect(m_pOpenHistoryView, SIGNAL(improveDesign(int)), this, SLOT(resizeHistory(int)));
}

QOpenHistoryViewController::~QOpenHistoryViewController()
{
}

void QOpenHistoryViewController::getHistoryInfo(const QModelIndex& index)
{
	QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(m_pHistoryDatabaseModel->itemFromIndex(index));
	m_databaseModel = pItem->getDatabaseModel();


	//TODO Switch between types
	setHistoryInfo();
}

void QOpenHistoryViewController::setHistoryInfo()
{
	QString qText, qElidedText, szFName;
	szFName = m_databaseModel.getDatabasePath().section('/', -1);
	qText = m_databaseModel.getDatabasePath().section('/', 0, -2);

	QFontMetrics metrics(m_pOpenHistoryView->getHistoryInfoWidget()->font());
	qElidedText = metrics.elidedText(qText, Qt::ElideMiddle, m_iWindowWidth);

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

void QOpenHistoryViewController::resizeHistory(int iWidth)
{
	QString qText, qElidedText;
	m_iWindowWidth = iWidth;
	m_pHistoryDatabaseModel->obtainInfoDesign(iWidth, m_pOpenHistoryView->getHistoryTreeView()->font());
	setHistoryInfo();
}
