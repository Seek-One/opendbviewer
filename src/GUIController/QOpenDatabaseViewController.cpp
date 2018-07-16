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
	m_bGoOn = true;
}

QOpenDatabaseViewController::~QOpenDatabaseViewController()
{

}

void QOpenDatabaseViewController::init(QWindowMain* pMainWindow, QOpenDatabaseView* pOpenDatabaseView)
{
	m_pMainWindow = pMainWindow;
	m_pOpenDatabaseView = pOpenDatabaseView;

	//Search in SQL Form
	connect(m_pOpenDatabaseView->getSQLiteFileSelectionButton(), SIGNAL(clicked()), this, SLOT(openFileDialog()));

	//Forms Validation buttons
	connect(m_pOpenDatabaseView, SIGNAL(clicked(DatabaseModel::DatabaseType)), this, SLOT(buttonClicked(DatabaseModel::DatabaseType)));

	//File Explorer Buttons
	connect(m_pOpenDatabaseView->getDropAreaWidget(), SIGNAL(fileDropped(const QString&)), this, SLOT(openSQLiteFile(const QString&)));
	connect(m_pOpenDatabaseView->getFileExplorerWidget(), SIGNAL(openSelectedFile(const QString&)), this, SLOT(openSQLiteFile(const QString&)));
	connect(m_pOpenDatabaseView->getFileExplorerWidget(), SIGNAL(openDatabase(const QString&)), this, SLOT(openSQLiteFile(const QString&)));
	connect(m_pOpenDatabaseView->getFileExplorerWidget()->getDropArea(), SIGNAL(fileDropped(const QString&)), this, SLOT(openSQLiteFile(const QString&)));

	//Favourites Databases Selection
	connect(m_pOpenDatabaseView, SIGNAL(openFavouriteSQLiteDatabase(const QString&)), this, SLOT(openSQLiteFile(const QString&)));

	//Main ToolBar Slots
	connect(m_pMainWindow->getViewsAction(), SIGNAL(triggered()), this, SLOT(openViews()));
	connect(m_pMainWindow->getFavAction(), SIGNAL(triggered()), this, SLOT(openFav()));
	connect(m_pMainWindow->getExplorerAction(), SIGNAL(triggered()), this, SLOT(openExplorer()));
	connect(m_pMainWindow->getNewConnAction(), SIGNAL(triggered()), this, SLOT(openMenuConn()));

	//Add Databases ToolBar Slots
	connect(m_pMainWindow->getSQLiteAction(), SIGNAL(triggered()), this, SLOT(openSQLite()));
	connect(m_pMainWindow->getMySQLAction(), SIGNAL(triggered()), this, SLOT(openMySQL()));
	connect(m_pMainWindow->getPostgreSQLAction(), SIGNAL(triggered()), this, SLOT(openPostgreSQL()));

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
	buttonClicked(DatabaseModel::SQLiteType);
}

void QOpenDatabaseViewController::closeOpenDatabaseDialog()
{
	m_pOpenDatabaseView->close();
}

void QOpenDatabaseViewController::buttonClicked(DatabaseModel::DatabaseType type) {
	m_bGoOn = true;
	QString szTabFileName;
	QString szErrorMsg;
	DatabaseModel database;

	switch(type){
	case DatabaseModel::SQLiteType: // SQLite
		m_bGoOn = !m_szFileUrl.isEmpty();
		if(m_bGoOn){
			szTabFileName =	m_szFileUrl.section('/', -1);//Get the last part of the file path to get the name for the tab
			database=selectedDatabase(type, szTabFileName);
		} else {
			szErrorMsg = tr("Please select a valid SQLite file");
		}
		break;
	case  DatabaseModel::MySQLType: // MySQL
		m_bGoOn = !m_pOpenDatabaseView->getMySQLHostField()->text().isEmpty() && !m_pOpenDatabaseView->getMySQLDatabaseField()->text().isEmpty();
		if(m_bGoOn){
			szTabFileName = m_pOpenDatabaseView->getMySQLDatabaseField()->text();
			database=selectedDatabase(type, szTabFileName);
		}else{
			szErrorMsg = tr("Please enter the necessary information.");
		}
		break;
	case DatabaseModel::PostgreSQLType: // PostgreSQL
		m_bGoOn = !m_pOpenDatabaseView->getPSQLHostField()->text().isEmpty() && !m_pOpenDatabaseView->getPSQLDatabaseField()->text().isEmpty();
		if(m_bGoOn){
			szTabFileName = m_pOpenDatabaseView->getPSQLDatabaseField()->text();
			database=selectedDatabase(type, szTabFileName);
		}else{
			szErrorMsg = tr("Please enter the necessary information.");
		}
		break;
	default:
		break;
	}
	loadDatabase(database);
}

DatabaseModel QOpenDatabaseViewController::selectedDatabase(int iType, QString szTabFileName) {
	DatabaseModel databaseModel;
	switch(iType){
	case DatabaseModel::SQLiteType:
		databaseModel.setDatabaseName(szTabFileName);
		databaseModel.setDatabasePath(m_szFileUrl);
		databaseModel.setDatabaseType(iType);

		ApplicationSettings::addFavouriteDatabase(databaseModel);
		QSettingsManager::getInstance().saveDatabaseSettings();
		initFavouriteList();

		break;
	/*case DatabaseModel::MySQLType:
		szDatabaseType= "mysql";
		databaseModel.setDatabaseName(szTabFileName);
		databaseModel.setDatabasePath(m_szFileUrl);
		databaseModel.setDatabaseType(szDatabaseType);
		MySQLDatabaseInfo(databaseModel);

		ApplicationSettings::addFavouriteDatabase(databaseModel);
		QSettingsManager::getInstance().saveDatabaseSettings();
		initFavouriteList();
		break;
	case DatabaseModel::PostgreSQLType:
		szDatabaseType= "postgresql";
		databaseModel.setDatabaseName(szTabFileName);
		databaseModel.setDatabasePath(m_szFileUrl);
		databaseModel.setDatabaseType(szDatabaseType);
		PostgreSQLDatabaseInfo(databaseModel);

		ApplicationSettings::addFavouriteDatabase(databaseModel);
		QSettingsManager::getInstance().saveDatabaseSettings();
		initFavouriteList();
		break;*/
	default:
		break;
	}
	return databaseModel;
}


void QOpenDatabaseViewController::loadDatabase(const DatabaseModel&  databaseModel)
{
	QString szErrorMsg;

	DatabaseController * dbControl = NULL;
	QDatabaseConnectionView* pConnectionView = NULL;
	QDatabaseConnectionViewController* pDatabaseConnectionViewController = NULL;

	switch(databaseModel.getDatabaseType()) {
	case DatabaseModel::SQLiteType:
		dbControl = new DatabaseControllerSqlite(databaseModel);
		break;
	case DatabaseModel::MySQLType:
		dbControl = new DatabaseControllerMysql(databaseModel);
		break;
	case DatabaseModel::PostgreSQLType:
		dbControl = new DatabaseControllerPostgreSQL(databaseModel);
		break;
	}

	// Testing the connection, if it pass load the selected database else give a warning
	if(m_bGoOn){
		m_bGoOn = dbControl->openDatabase();
		if(m_bGoOn){
			dbControl->closeDataBase();
		} else {
			szErrorMsg = tr("Unable to connect to the database, please check the connection information.");
		}
	}
	// Init the view and fill it
	if(m_bGoOn){
		pConnectionView = new QDatabaseConnectionView(m_pMainWindow);
		pDatabaseConnectionViewController = new QDatabaseConnectionViewController(m_szFileUrl, dbControl);
		pDatabaseConnectionViewController->init(pConnectionView);

		// Adding DatabaseConnectionView to the main window
		m_pMainWindow->addDatabaseConnectionView(pConnectionView, databaseModel.getDatabaseName());


		// Updating tables
		if(!pDatabaseConnectionViewController->loadDatabaseTables()){
			QMessageBox::warning(m_pOpenDatabaseView, tr("Database problem"), tr("Problem while loading database tables"));
		}

		// Controller will be deleted when the view is destroyed
		connect(pConnectionView, SIGNAL(destroyed(QObject*)), pDatabaseConnectionViewController, SLOT(deleteLater()));
	}

	if(!m_bGoOn){
		if(dbControl){
			delete dbControl;
			dbControl = NULL;
		}
		QMessageBox::critical(m_pOpenDatabaseView, tr("Connection error"), szErrorMsg);
	}
}

void QOpenDatabaseViewController::MySQLDatabaseInfo(DatabaseModel * database)
{
	bool ok = true;
	database->setDatabaseName(m_pOpenDatabaseView->getMySQLDatabaseField()->text());
	database->setDatabaseHost(m_pOpenDatabaseView->getMySQLHostField()->text());
	database->setDatabasePort(m_pOpenDatabaseView->getMySQLPortField()->text().toInt(&ok, 10));
	database->setDatabaseUsername(m_pOpenDatabaseView->getMySQLUsernameField()->text());
	//m_pOpenDatabaseView->getMySQLPasswordField()->text()); //TODO Modify DatabaseModel to accept the password?
}

void QOpenDatabaseViewController::PostgreSQLInfo(DatabaseModel * database)
{
	bool ok = true;
	database->setDatabaseName(m_pOpenDatabaseView->getPSQLDatabaseField()->text());
	database->setDatabaseHost(m_pOpenDatabaseView->getPSQLHostField()->text());
	database->setDatabasePort(m_pOpenDatabaseView->getPSQLPortField()->text().toInt(&ok, 10));
	database->setDatabaseUsername(m_pOpenDatabaseView->getPSQLUsernameField()->text());
	//m_pOpenDatabaseView->getPSQLPasswordField()->text()); //TODO Modify DatabaseModel to accept the password?
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

void QOpenDatabaseViewController::openViews() {
	m_pMainWindow->showViewsTab();
}

void QOpenDatabaseViewController::openFav() {
	m_pMainWindow->addFavouritesDatabasesTab();
}

void QOpenDatabaseViewController::openExplorer() {
	m_pMainWindow->addExplorerTab();
}

void QOpenDatabaseViewController::openMenuConn() {
	m_pMainWindow->addNewConnMenuTab();
}

void QOpenDatabaseViewController::openSQLite() {
	m_pMainWindow->OpenSQLiteTab();
}

void QOpenDatabaseViewController::openMySQL() {
	m_pMainWindow->OpenMySQLTab();
}

void QOpenDatabaseViewController::openPostgreSQL() {
	m_pMainWindow->OpenPostgreSQLTab();
}
