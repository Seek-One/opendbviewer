/*
 * QWindowMainController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include <QMessageBox>

#include "QWindowMainController.h"

#include "GUI/QWindowMain.h"
#include "GUI/QDatabaseConnectionView.h"

#include "QDatabaseConnectionViewController.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
}

QWindowMainController::~QWindowMainController()
{

}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;

    connect(m_pMainWindow->getNewConnectionAction(), SIGNAL(triggered()), this, SLOT(newDatabaseConnection()));
    connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
    connect(m_pMainWindow->getDatabaseConnectionTab(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeConnectionTab(int)));
    newDatabaseConnection();
}

void QWindowMainController::newDatabaseConnection()
{
	QDatabaseConnectionView* pConnectionView = new QDatabaseConnectionView(m_pMainWindow);
	m_pMainWindow->addDatabaseConnectionView(pConnectionView, tr("new tab"));

	QDatabaseConnectionViewController* pDatabaseConnectionViewController = new QDatabaseConnectionViewController();
	pDatabaseConnectionViewController->init(pConnectionView);

	connect(pConnectionView, SIGNAL(destroyed(QObject*)), pDatabaseConnectionViewController, SLOT(deleteLater()));
}

void QWindowMainController::about()
{
	QMessageBox::information(m_pMainWindow, tr("About..."), tr("Text"));
}

void QWindowMainController::closeConnectionTab(const int& index)
{
	if(m_pMainWindow->getDatabaseConnectionTab()->count() == 1)
	{
		return;
	}

	QWidget* tabItem = m_pMainWindow->getDatabaseConnectionTab()->widget(index);
	m_pMainWindow->getDatabaseConnectionTab()->removeTab(index);

	delete(tabItem);
}
