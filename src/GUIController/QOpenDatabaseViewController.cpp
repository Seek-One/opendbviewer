/*
 * QOpenDatabaseDialogController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QtCore>
#include <QtGui>
#include <QTreeView>

#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerMysql.h"
#include "Database/DatabaseControllerPostgreSQL.h"
#include "Database/DatabaseControllerSqlite.h"
#include "Global/ApplicationSettings.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QOpenDatabaseView.h"
#include "GUI/QWindowMain.h"
#include "QDatabaseConnectionViewController.h"
#include "QOpenDatabaseViewController.h"
#include "Settings/QSettingsManager.h"

QOpenDatabaseViewController::QOpenDatabaseViewController()
{
	m_pOpenDatabaseView = NULL;
	m_pMainWindow = NULL;
}

QOpenDatabaseViewController::~QOpenDatabaseViewController()
{

}

void QOpenDatabaseViewController::init(QWindowMain* pMainWindow, QOpenDatabaseView* pOpenDatabaseView)
{
	m_pMainWindow = pMainWindow;
	m_pOpenDatabaseView = pOpenDatabaseView;

	connect(m_pOpenDatabaseView->getSQLiteFileSelectionButton(), SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(m_pOpenDatabaseView->getOKButton(), SIGNAL(clicked()), this, SLOT(loadDatabase()));
	connect(m_pOpenDatabaseView->getMySQLConnectButton(), SIGNAL(clicked()), this, SLOT(loadDatabase()));
	connect(m_pOpenDatabaseView->getPostgreSQLConnectButton(), SIGNAL(clicked()), this, SLOT(loadDatabase()));

	connect(m_pOpenDatabaseView->getDropAreaWidget(), SIGNAL(fileDropped(const QString&)), this, SLOT(openSQLiteFile(const QString&)));
	connect(m_pOpenDatabaseView->getFileExplorerWidget(), SIGNAL(openSelectedFile(const QString&)), this, SLOT(openSQLiteFile(const QString&)));
	connect(m_pOpenDatabaseView->getFileExplorerWidget(), SIGNAL(openDatabase(const QString&)), this, SLOT(openSQLiteFile(const QString&)));
	connect(m_pOpenDatabaseView->getFileExplorerWidget()->getDropArea(), SIGNAL(fileDropped(const QString&)), this, SLOT(openSQLiteFile(const QString&)));

	connect(m_pOpenDatabaseView, SIGNAL(openFavouriteSQLiteDatabase(const QString&)), this, SLOT(openSQLiteFile(const QString&)));

	// Default values
	m_pOpenDatabaseView->getMySQLHostField()->setText("127.0.0.1");
	m_pOpenDatabaseView->getMySQLPortField()->setText("3306");
	m_pOpenDatabaseView->getMySQLUsernameField()->setText("root");
	m_pOpenDatabaseView->getMySQLDatabaseField()->setText("mysql");
	// Default values
	m_pOpenDatabaseView->getPSQLHostField()->setText("127.0.0.1");
	m_pOpenDatabaseView->getPSQLPortField()->setText("5432");
	m_pOpenDatabaseView->getPSQLUsernameField()->setText("postgres");

	initFavouriteList();
}

void QOpenDatabaseViewController::openFileDialog()
{
	m_szFileUrl = QFileDialog::getOpenFileName(m_pOpenDatabaseView, tr("Select a file"), QString(), tr("SQLite files (*.sqlite *.db)"));
	m_pOpenDatabaseView->getSQLiteFilePathField()->setText(m_szFileUrl);
}

void QOpenDatabaseViewController::openSQLiteFile(const QString& szFileUrl)
{
	m_szFileUrl = szFileUrl;
	loadDatabase();
}

void QOpenDatabaseViewController::closeOpenDatabaseDialog()
{
	m_pOpenDatabaseView->close();
}

void QOpenDatabaseViewController::loadDatabase()
{
	// Creating a string to be used to set the tab name
	QString szTabFileName;
	QStringList szDatabaseInfoList;
	int iDatabaseType;

	bool bGoOn = true;
	QString szErrorMsg;

	DatabaseController* pDatabaseController = NULL;
	QDatabaseConnectionView* pConnectionView = NULL;
	QDatabaseConnectionViewController* pDatabaseConnectionViewController = NULL;

	int iCurrentIndex = m_pOpenDatabaseView->getConnectionTypeTabWidget()->currentIndex();

	switch(iCurrentIndex){
	case FavouriteIndex:

	case ExplorerFileIndex:
	case SQLiteIndex: // SQLite
		iDatabaseType = 0;
		bGoOn = !m_szFileUrl.isEmpty();
		if(bGoOn){
			szTabFileName =	m_szFileUrl.section('/', -1);//Get the last part of the file path to get the name for the tab
			pDatabaseController = new DatabaseControllerSqlite(m_szFileUrl);
		}else{
			szErrorMsg = tr("Please select a valid SQLite file");
		}
		break;
	case MySQLIndex: // MySQL
		bGoOn = !m_pOpenDatabaseView->getMySQLHostField()->text().isEmpty() && !m_pOpenDatabaseView->getMySQLDatabaseField()->text().isEmpty();
		if(bGoOn){
			szTabFileName = m_pOpenDatabaseView->getMySQLDatabaseField()->text();
			szDatabaseInfoList = makeMySQLDatabaseInfoList();
			pDatabaseController = new DatabaseControllerMysql(m_szFileUrl, szDatabaseInfoList);
		}else{
			szErrorMsg = tr("Please enter the necessary information.");
		}
		break;
	case PostgreSQLIndex: // PostgreSQL
		bGoOn = !m_pOpenDatabaseView->getPSQLHostField()->text().isEmpty() && !m_pOpenDatabaseView->getPSQLDatabaseField()->text().isEmpty();
		if(bGoOn){
			szTabFileName = m_pOpenDatabaseView->getPSQLDatabaseField()->text();
			szDatabaseInfoList = makePostgreSQLDatabaseInfoList();
			pDatabaseController = new DatabaseControllerPostgreSQL(m_szFileUrl, szDatabaseInfoList);
		}else{
			szErrorMsg = tr("Please enter the necessary information.");
		}
		break;
	default:
		break;
	}

	// Testing the connection, if it fails, give a warning
	if(bGoOn){
		bGoOn = pDatabaseController->openDatabase();
		if(bGoOn){
			pDatabaseController->closeDataBase();
		}else{
			szErrorMsg = tr("Unable to connect to the database, please check the connection information.");
		}
	}

	DatabaseModel databaseModel;
	QString szDatabaseType = "sqlite";


	switch(iDatabaseType){
	case SQLiteType:
		databaseModel.setDatabaseName(szTabFileName);
		databaseModel.setDatabasePath(m_szFileUrl);
		databaseModel.setDatabaseType(szDatabaseType);

		ApplicationSettings::addFavouriteDatabase(databaseModel);
		QSettingsManager::getInstance().saveDatabaseSettings();
		initFavouriteList();

		break;
	}


	// Init the view and fill it
	if(bGoOn){
		pConnectionView = new QDatabaseConnectionView(m_pMainWindow);
		pDatabaseConnectionViewController = new QDatabaseConnectionViewController(m_szFileUrl, pDatabaseController);
		pDatabaseConnectionViewController->init(pConnectionView, szDatabaseInfoList);

		// Adding DatabaseConnectionView to the main window
		m_pMainWindow->addDatabaseConnectionView(pConnectionView, szTabFileName);

		// Updating tables
		if(!pDatabaseConnectionViewController->loadDatabaseTables()){
			QMessageBox::warning(m_pOpenDatabaseView, tr("Database problem"), tr("Problem while loading database tables"));
		}

		// Controller will be deleted when the view is destroyed
		connect(pConnectionView, SIGNAL(destroyed(QObject*)), pDatabaseConnectionViewController, SLOT(deleteLater()));
	}

	if(!bGoOn){
		if(pDatabaseController){
			delete pDatabaseController;
			pDatabaseController = NULL;
		}
		QMessageBox::critical(m_pOpenDatabaseView, tr("Connection error"), szErrorMsg);
	}
}

QString QOpenDatabaseViewController::getFileUrl() const
{
	return m_szFileUrl;
}

void QOpenDatabaseViewController::initFavouriteList()
{
	FavouriteDatabaseList list = ApplicationSettings::getFavouriteList();
	DatabaseModel database;

	m_pOpenDatabaseView->getFavouriteTreeWidget()->clear();
	for (int row = list.size() - 1 ; row >= 0 ; row--){
		database = list.at(row);
		QTreeWidgetItem *item = new QTreeWidgetItem(m_pOpenDatabaseView->getFavouriteTreeWidget());
		item->setText(0, database.getDatabasePath());
	}
}
QStringList QOpenDatabaseViewController::makeMySQLDatabaseInfoList()
{
	QStringList szDatabaseInfoList;

	szDatabaseInfoList << m_pOpenDatabaseView->getMySQLHostField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getMySQLPortField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getMySQLUsernameField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getMySQLPasswordField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getMySQLDatabaseField()->text();

	return szDatabaseInfoList;
}

QStringList QOpenDatabaseViewController::makePostgreSQLDatabaseInfoList()
{
	QStringList szDatabaseInfoList;

	szDatabaseInfoList << m_pOpenDatabaseView->getPSQLHostField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getPSQLPortField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getPSQLUsernameField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getPSQLPasswordField()->text();
	szDatabaseInfoList << m_pOpenDatabaseView->getPSQLDatabaseField()->text();

	return szDatabaseInfoList;
}
