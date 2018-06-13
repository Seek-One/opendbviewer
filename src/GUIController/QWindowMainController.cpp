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

#include "Database/DatabaseController.h"
#include "GUI/QAboutDialog.h"
#include "GUI/QWindowMain.h"
#include "GUI/QDatabaseConnectionView.h"
#include "QDatabaseConnectionViewController.h"
#include "QOpenDatabaseViewController.h"
#include "QWindowMainController.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
	m_pOpenDatabaseViewController = NULL;
}

QWindowMainController::~QWindowMainController()
{
	if(m_pOpenDatabaseViewController)
	{
		delete m_pOpenDatabaseViewController;
		m_pOpenDatabaseViewController = NULL;
	}
}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;
	m_pOpenDatabaseViewController = new QOpenDatabaseViewController();

	m_pOpenDatabaseViewController->init(pMainWindow, pMainWindow->getOpenDatabaseView());

    connect(m_pMainWindow->getDatabaseConnectionTab(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseConnectionTab(int)));
    connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
    connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));
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
