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

#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QDebug>

QDatabaseSelectionViewController::QDatabaseSelectionViewController()
{
	m_pDatabaseSelectionView = NULL;
	m_pMainWindow = NULL;
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

	if(m_pDatabaseSelectionView->getFileSelectionTabWidget()->currentIndex() == 0 && m_fileName.isEmpty() == false)//If not filename is provided, use the information from mysql tab to establish a connection
	//If the current tab is the sqlite tab and a file path is given
	{
		szTabFileName =	m_fileName.section('/', -1);//Get the last part of the file path to get the name for the tab
	}
	else if(m_pDatabaseSelectionView->getFileSelectionTabWidget()->currentIndex() == 1 && m_pDatabaseSelectionView->getHostField()->text().isEmpty() == false)
	//If the current tab is the mysql tab and databaseinfolist is not empty
	{
		//Creating a list containing the user input information
		szDatabaseInfoList = makeDatabaseInfoList();
			if(szDatabaseInfoList.contains(""))//If the user has not entered all the information
			{
				QMessageBox::warning(m_pDatabaseSelectionView, tr("Information missing"),tr("Some information is missing.\nPlease, make sure you have provided the necessary information."));
				return;
			}
			else
				szTabFileName = m_pDatabaseSelectionView->getDatabaseField()->text(); //Setting the tab name to the database name
	}
	else
	//otherwise, open a message box asking to select a connection
	{
		QMessageBox::warning(m_pDatabaseSelectionView, tr("No connection selected"),tr("Please select a connection."));
		return;
	}

	QDatabaseConnectionViewController* pDatabaseConnectionViewController = new QDatabaseConnectionViewController(m_fileName);
	pDatabaseConnectionViewController->init(pConnectionView, szDatabaseInfoList);

	//Adding DatabaseConnectionView to the main window
	m_pMainWindow->addDatabaseConnectionView(pConnectionView, szTabFileName);

	//Updating tables
	pDatabaseConnectionViewController->updateTables();

	connect(pConnectionView, SIGNAL(destroyed(QObject*)), pDatabaseConnectionViewController, SLOT(deleteLater()));

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
