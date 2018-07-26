/*
 * QWindowMainController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */


#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTabWidget>
#include <QVBoxLayout>

#include "GUI/QAboutDialog.h"
#include "GUI/QWindowMain.h"
#include "GUI/QOpenHistoryView.h"
#include "GUI/QDatabaseConnectionView.h"
#include "QDatabaseConnectionViewController.h"
#include "QWindowMainController.h"
#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerMysql.h"
#include "Database/DatabaseControllerPostgreSQL.h"
#include "Database/DatabaseControllerSqlite.h"
#include "Global/ApplicationSettings.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
	m_pOpenDatabaseViewController = NULL;
	m_pOpenHistoryViewController = NULL;
}

QWindowMainController::~QWindowMainController()
{
	if(m_pOpenDatabaseViewController)
	{
		delete m_pOpenDatabaseViewController;
		m_pOpenDatabaseViewController = NULL;
	}
	if(m_pOpenHistoryViewController)
	{
		delete m_pOpenHistoryViewController;
		m_pOpenHistoryViewController = NULL;
	}
}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;
	m_pOpenDatabaseViewController = new QOpenDatabaseViewController(pMainWindow, this);
	m_pOpenHistoryViewController = new QOpenHistoryViewController(pMainWindow, this);

    connect(m_pMainWindow->getDatabaseConnectionTab(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseConnectionTab(int)));
    connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
    connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));

	//File Explorer Buttons
	connect(m_pMainWindow->getFileExplorerWidget(), SIGNAL(openSelectedFile(const QString&)), this, SLOT(callSQLiteFile(const QString&)));
	connect(m_pMainWindow->getFileExplorerWidget(), SIGNAL(openDatabase(const QString&)), this, SLOT(callSQLiteFile(const QString&)));
	connect(m_pMainWindow->getFileExplorerWidget()->getDropArea(), SIGNAL(fileDropped(const QString&)), this, SLOT(callSQLiteFile(const QString&)));

	//Main ToolBar Slots
	connect(m_pMainWindow->getViewsAction(), SIGNAL(triggered()), this, SLOT(openViews()));
	connect(m_pMainWindow->getHistAction(), SIGNAL(triggered()), this, SLOT(openHist()));
	connect(m_pMainWindow->getExplorerAction(), SIGNAL(triggered()), this, SLOT(openExplorer()));
	connect(m_pMainWindow->getNewConnAction(), SIGNAL(triggered()), this, SLOT(openMenuConn()));
}

void QWindowMainController::closeDatabaseConnectionTab(const int& index)
{
	QWidget* pTabItem = m_pMainWindow->getDatabaseConnectionTab()->widget(index);
	if(pTabItem){
		m_pMainWindow->getDatabaseConnectionTab()->removeTab(index);
		delete pTabItem;
	}
}

void QWindowMainController::about()
{
	QAboutDialog aboutDialog(m_pMainWindow);
	aboutDialog.exec();
}

void QWindowMainController::openViews()
{
	m_pMainWindow->showViewsTab();
}

void QWindowMainController::openHist()
{
	m_pMainWindow->showHistoryTab();
}

void QWindowMainController::openExplorer()
{
	m_pMainWindow->showExplorerTab();
}

void QWindowMainController::openMenuConn()
{
	m_pMainWindow->showMenuConnTab();
}

void QWindowMainController::callSQLiteFile(const QString& szFileUrl)
{
	m_pOpenDatabaseViewController->openSQLiteFile(szFileUrl);
}

void QWindowMainController::callInitHistoryList() {
	m_pOpenHistoryViewController->initHistoryList();
}
