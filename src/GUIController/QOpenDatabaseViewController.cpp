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

QOpenDatabaseViewController::QOpenDatabaseViewController(QWindowMain* pMainWindow, QWindowMainController* pMainWindowController)
{
	m_pMainWindow = pMainWindow;
	m_pMainWindowController = pMainWindowController;
	m_pOpenDatabaseView = pMainWindow->getOpenDatabaseView();

	//Search in SQL Form
	connect(m_pOpenDatabaseView->getSQLiteFileSelectionButton(), SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(m_pOpenDatabaseView->getDropAreaWidget(), SIGNAL(fileDropped(const QString&)), this, SLOT(openSQLiteFile(const QString&)));

	//Forms Validation buttons
	connect(m_pOpenDatabaseView, SIGNAL(clicked(DatabaseModel::DatabaseType)), this, SLOT(prepareConnection(DatabaseModel::DatabaseType)));

	//Slots for buttons in Add Connection Menu
	connect(m_pOpenDatabaseView->getSQLiteSelection(), SIGNAL(clicked()), this, SLOT(openSQLite()));
	connect(m_pOpenDatabaseView->getMySQLSelection(), SIGNAL(clicked()), this, SLOT(openMySQL()));
	connect(m_pOpenDatabaseView->getPostgreSQLSelection(), SIGNAL(clicked()), this, SLOT(openPostgreSQL()));

	// Default values
	m_pOpenDatabaseView->getMySQLHostField()->setText("127.0.0.1");
	m_pOpenDatabaseView->getMySQLPortField()->setText("3306");
	// Default values
	m_pOpenDatabaseView->getPSQLHostField()->setText("127.0.0.1");
	m_pOpenDatabaseView->getPSQLPortField()->setText("5432");
}

QOpenDatabaseViewController::~QOpenDatabaseViewController()
{

}

void QOpenDatabaseViewController::openFileDialog()
{
	m_szFileUrl = QFileDialog::getOpenFileName(m_pOpenDatabaseView, tr("Select a file"), QString(), tr("SQLite files (*.sqlite *.db)"));
	m_pOpenDatabaseView->getSQLiteFilePathField()->setText(m_szFileUrl);
}

void QOpenDatabaseViewController::closeOpenDatabaseDialog()
{
	m_pOpenDatabaseView->close();
}

void QOpenDatabaseViewController::recoverMySQLDatabaseInfo(DatabaseModel * database)
{
	database->setDatabaseName(m_pOpenDatabaseView->getMySQLDatabaseField()->text());
	database->setDatabaseHost(m_pOpenDatabaseView->getMySQLHostField()->text());
	database->setDatabasePort(m_pOpenDatabaseView->getMySQLPortField()->text().toInt());
	database->setDatabaseUsername(m_pOpenDatabaseView->getMySQLUsernameField()->text());
	database->setDatabasePassword(m_pOpenDatabaseView->getMySQLPasswordField()->text());
}

void QOpenDatabaseViewController::recoverPostgreSQLInfo(DatabaseModel * database)
{
	database->setDatabaseName(m_pOpenDatabaseView->getPSQLDatabaseField()->text());
	database->setDatabaseHost(m_pOpenDatabaseView->getPSQLHostField()->text());
	database->setDatabasePort(m_pOpenDatabaseView->getPSQLPortField()->text().toInt());
	database->setDatabaseUsername(m_pOpenDatabaseView->getPSQLUsernameField()->text());
	database->setDatabasePassword(m_pOpenDatabaseView->getPSQLPasswordField()->text());
}


QString QOpenDatabaseViewController::getFileUrl() const
{
	return m_szFileUrl;
}

void QOpenDatabaseViewController::openSQLiteFile(const QString& szFileUrl)
{
	m_szFileUrl = szFileUrl;
	prepareConnection(DatabaseModel::SQLiteType);
}

void QOpenDatabaseViewController::prepareConnection(DatabaseModel::DatabaseType type) {
	bool bGoOn = true;
	QString szErrorMsg;
	DatabaseModel database;

	switch(type){
	case DatabaseModel::SQLiteType: // SQLite
		bGoOn = !m_szFileUrl.isEmpty();
		if(bGoOn){
			database = selectDatabase(type);
			m_szFileUrl = "";
			m_pOpenDatabaseView->getSQLiteFilePathField()->setText("");
		} else {
			szErrorMsg = tr("Please select a valid SQLite file");
		}
		break;
	case  DatabaseModel::MySQLType: // MySQL
		bGoOn = !m_pOpenDatabaseView->getMySQLHostField()->text().isEmpty()  && !m_pOpenDatabaseView->getMySQLDatabaseField()->text().isEmpty();
		if(bGoOn){
			database = selectDatabase(type);
		} else {
			szErrorMsg = tr("Please enter the necessary information.");
		}
		break;
	case DatabaseModel::PostgreSQLType: // PostgreSQL
		bGoOn = !m_pOpenDatabaseView->getPSQLHostField()->text().isEmpty()  && !m_pOpenDatabaseView->getPSQLDatabaseField()->text().isEmpty();
		if(bGoOn){
			database = selectDatabase(type);
		} else {
			szErrorMsg = tr("Please enter the necessary information.");
		}
		break;
	default:
		break;
	}

	if (bGoOn) {
		loadDatabase(database);
	} else {
		QMessageBox::critical(m_pOpenDatabaseView, tr("Invalid Informations"), szErrorMsg);
	}
}

DatabaseModel QOpenDatabaseViewController::selectDatabase(DatabaseModel::DatabaseType type) {
	DatabaseModel databaseModel;
	switch(type){
	case DatabaseModel::SQLiteType:
		databaseModel.setDatabaseName(m_szFileUrl.section('/', -1)); //Get the last part of the file path to get the name for the tab
		databaseModel.setDatabasePath(m_szFileUrl);
		databaseModel.setDatabaseType(type);

		ApplicationSettings::addHistoryDatabase(databaseModel);
		QSettingsManager::getInstance().saveDatabaseSettings();
		m_pMainWindowController->callInitHistoryList();
		break;
	case DatabaseModel::MySQLType:
		recoverMySQLDatabaseInfo(&databaseModel);
		databaseModel.setDatabaseType(type);
		break;
	case DatabaseModel::PostgreSQLType:
		recoverPostgreSQLInfo(&databaseModel);
		databaseModel.setDatabaseType(type);
		break;
	default:
		break;
	}
	return databaseModel;
}


void QOpenDatabaseViewController::loadDatabase(const DatabaseModel&  databaseModel)
{
	QString szErrorMsg;
	bool bGoOn = true;
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
	default:
		break;
	}

	// Testing the connection, if it pass load the selected database else give a warning
	if(bGoOn){
		bGoOn = dbControl->openDatabase();
		if(bGoOn){
			dbControl->closeDataBase();
		} else {
			szErrorMsg = tr("Unable to connect to the database, please check the connection information.");
		}
	}
	// Init the view and fill it
	if(bGoOn){
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

	if(!bGoOn){
		if(dbControl){
			delete dbControl;
			dbControl = NULL;
		}
		QMessageBox::critical(m_pOpenDatabaseView, tr("Connection error"), szErrorMsg);
	}
}

void QOpenDatabaseViewController::openSQLite() {
	m_pOpenDatabaseView->openSQLiteTab();
}

void QOpenDatabaseViewController::openMySQL() {
	m_pOpenDatabaseView->openMySQLTab();
}

void QOpenDatabaseViewController::openPostgreSQL() {
	m_pOpenDatabaseView->openPostgreSQLTab();
}
