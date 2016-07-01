/*
 * QDatabaseSelectionViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionViewController.h"
#include "QDatabaseSelectionViewController.h"
#include "GUI/QDatabaseSelectionView.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QWindowMain.h"
#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerMysql.h"
#include "Database/DatabaseControllerSqlite.h"

QDatabaseSelectionViewController::QDatabaseSelectionViewController()
{
	m_pDatabaseSelectionView = NULL;
	m_pMainWindow = NULL;
	m_pDatabaseController = NULL;
	m_pDatabaseConnectionViewController = NULL;
}


QDatabaseSelectionViewController::~QDatabaseSelectionViewController()
{

}



void QDatabaseSelectionViewController::init(QWindowMain* pMainWindow, QDatabaseSelectionView* pDatabaseSelectionView)
{
	m_pMainWindow = pMainWindow;
	m_pDatabaseSelectionView = pDatabaseSelectionView;
	connect(m_pDatabaseSelectionView->getFileSelectionButton(), SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(m_pDatabaseSelectionView->getCancelButton(), SIGNAL(clicked()), this, SLOT(closeSelectionWindow()));
	connect(m_pDatabaseSelectionView->getOKButton(), SIGNAL(clicked()), this, SLOT(loadDatabase()));
}


void QDatabaseSelectionViewController::openFileDialog()
{
	m_fileName = QFileDialog::getOpenFileName(m_pDatabaseSelectionView, "Select a file", QString(), "Sqlite files (*.sqlite)");

	m_pDatabaseSelectionView->getFilePathField()->setText(m_fileName);
}

void QDatabaseSelectionViewController::closeSelectionWindow()
{
	m_pDatabaseSelectionView->close();
	m_pDatabaseSelectionView->~QDatabaseSelectionView();
}

void QDatabaseSelectionViewController::loadDatabase()
{
	//Creating a string to be used to set the tab name
	QString szTabFileName;
	QStringList szDatabaseInfoList;
	szDatabaseInfoList.clear(); //Makes sure the list is empty
	QDatabaseConnectionView* pConnectionView = new QDatabaseConnectionView(m_pMainWindow);

	//If the current tab is the sqlite tab and a file path is given
	if(m_pDatabaseSelectionView->getFileSelectionTabWidget()->currentIndex() == 0 && m_fileName.isEmpty() == false)//If not filename is provided, use the information from mysql tab to establish a connection
	{
		szTabFileName =	m_fileName.section('/', -1);//Get the last part of the file path to get the name for the tab
		m_pDatabaseController = new DatabaseControllerSqlite(m_fileName);
		m_pDatabaseConnectionViewController = new QDatabaseConnectionViewController(m_fileName, m_pDatabaseController);
		m_pDatabaseConnectionViewController->init(pConnectionView, szDatabaseInfoList);
	}
	//If the current tab is the mysql tab and databaseinfolist is not empty
	else if(m_pDatabaseSelectionView->getFileSelectionTabWidget()->currentIndex() == 1 && m_pDatabaseSelectionView->getHostField()->text().isEmpty() == false)
	{
		//Creating a list containing the user input information
		szDatabaseInfoList = makeDatabaseInfoList();
		m_pDatabaseController = new DatabaseControllerMysql(m_fileName, szDatabaseInfoList);

		//Testing the connection, if it fails, give a warning
		if(m_pDatabaseController->openDatabase() == false)
		{
			QMessageBox::warning(m_pDatabaseSelectionView, tr("Connection impossible"),tr("Unable to connect to database.\nPlease, make sure the information you have provided is correct."));
			return;
		}
		else
		{
			szTabFileName = m_pDatabaseSelectionView->getDatabaseField()->text();
			m_pDatabaseConnectionViewController = new QDatabaseConnectionViewController(m_fileName, m_pDatabaseController);
			m_pDatabaseConnectionViewController->init(pConnectionView, szDatabaseInfoList);
		}
	}
	//if no information is provided in either tab, open a message box asking to select a connection
	else
	{
		QMessageBox::warning(m_pDatabaseSelectionView, tr("No connection selected"),tr("Please enter the necessary information."));
		return;
	}

	//Adding DatabaseConnectionView to the main window
	m_pMainWindow->addDatabaseConnectionView(pConnectionView, szTabFileName);

	//Updating tables
	m_pDatabaseConnectionViewController->updateTables();

	connect(pConnectionView, SIGNAL(destroyed(QObject*)), m_pDatabaseConnectionViewController, SLOT(deleteLater()));

	closeSelectionWindow();
}

QString QDatabaseSelectionViewController::getFileName() const
{
	return m_fileName;
}

QStringList QDatabaseSelectionViewController::makeDatabaseInfoList()
{
	QStringList szDatabaseInfoList;
	szDatabaseInfoList << m_pDatabaseSelectionView->getHostField()->text();
	szDatabaseInfoList << m_pDatabaseSelectionView->getPortField()->text();
	szDatabaseInfoList << m_pDatabaseSelectionView->getUsernameField()->text();
	szDatabaseInfoList << m_pDatabaseSelectionView->getPasswordField()->text();
	szDatabaseInfoList << m_pDatabaseSelectionView->getDatabaseField()->text();

	return szDatabaseInfoList;
}
