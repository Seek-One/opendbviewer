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
    //connect(pAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    //connect(pAction, SIGNAL(triggered()), this, SLOT(about()));

    newDatabaseConnection();
}


void QWindowMainController::newDatabaseConnection()
{
	qDebug("clicked");

	QDatabaseConnectionView* pConnectionView = new QDatabaseConnectionView(m_pMainWindow);
	m_pMainWindow->addDatabaseConnectionView(pConnectionView, tr("new tab"));

	//connect(pAction, SIGNAL(triggered()), this, SLOT(about()));
}

void QWindowMainController::about()
{
	QMessageBox::information(m_pMainWindow, tr("About..."), tr("Text"));
}
