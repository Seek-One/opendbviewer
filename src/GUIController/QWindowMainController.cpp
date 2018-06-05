/*
 * QWindowMainController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */


#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

#include "Database/DatabaseController.h"

#include "GUI/QAboutDialog.h"
#include "GUI/QWindowMain.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QOpenDatabaseDialog.h"

#include "QWindowMainController.h"

#include "QDatabaseConnectionViewController.h"
#include "QOpenDatabaseDialogController.h"

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

    connect(m_pMainWindow->getNewConnectionAction(), SIGNAL(triggered()), this, SLOT(openNewDatabaseConnection()));
    connect(m_pMainWindow->getDatabaseConnectionTab(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseConnectionTab(int)));

    connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
    connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));

    // At startup we launch the database connection dialog
    openNewDatabaseConnection();
}

void QWindowMainController::openNewDatabaseConnection()
{
	QOpenDatabaseDialog dbOpenDialog(m_pMainWindow);

	QOpenDatabaseDialogController dbOpenDialogController;
	dbOpenDialogController.init(m_pMainWindow, &dbOpenDialog);
	dbOpenDialog.exec();
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