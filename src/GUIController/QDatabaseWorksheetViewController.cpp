/*
 * QDatabaseWorksheetViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QApplication>
#include <QPalette>
#include <QTableView>
#include <QMessageBox>

#include "Database/DatabaseController.h"

#include "GUI/QDatabaseWorksheetView.h"

#include "GUIController/QDatabaseWorksheetViewController.h"
#include "GUIController/QDatabaseTableViewController.h"
#include "GUIController/QWindowMainController.h"

#include "Settings/QSettingsManager.h"

QDatabaseWorksheetViewController::QDatabaseWorksheetViewController()
{
	m_pDatabaseWorksheetView = NULL;
	m_pDatabaseController = NULL;
	m_pSqlHighlighterController = NULL;
	m_pDatabaseDisplayModel = NULL;
	m_szDatabasesJsonPath = QSettingsManager::getInstance().getDatabasesJson();
	m_szDatabasesFilePath = QSettingsManager::getInstance().getDatabasesJsonDir();
}

QDatabaseWorksheetViewController::~QDatabaseWorksheetViewController()
{
	if(m_pSqlHighlighterController){
		delete m_pSqlHighlighterController;
		m_pSqlHighlighterController = NULL;
	}
}

void QDatabaseWorksheetViewController::init(QDatabaseWorksheetView* pDatabaseWorksheetView, DatabaseController* pDatabaseController)
{
	m_pDatabaseWorksheetView = pDatabaseWorksheetView;
	m_pDatabaseController = pDatabaseController;
	m_pDatabaseWorksheetView->getImportButton()->setVisible(false);
	m_pDatabaseWorksheetView->getExportButton()->setEnabled(false);

	setJsonDatabases();
	setRequestHistory();

	m_pSqlHighlighterController = new QSqlHighlighterController(m_pDatabaseWorksheetView->getWorksheetTextEdit()->document());

	connect(m_pDatabaseWorksheetView->getExecuteButton(), SIGNAL(clicked()), this, SLOT(executeQuery()));
	connect(m_pDatabaseWorksheetView->getReformatButton(), SIGNAL(clicked()), this, SLOT(reformatSqlText()));
	connect(m_pDatabaseWorksheetView->getClearTextButton(), SIGNAL(clicked()), this, SLOT(clearWorksheetText()));
	connect(m_pDatabaseWorksheetView->getExportButton(), SIGNAL(clicked()), this, SLOT(exportDataWorksheet()));
	connect(m_pDatabaseWorksheetView->getWorksheetTextEdit(), SIGNAL(textChanged()), this, SLOT(setExportButtonDisabled()));
	connect(m_pDatabaseWorksheetView->getRequestHistoryMenu(), SIGNAL(triggered(QAction*)), this, SLOT(changeWorksheetTextFromHistory(QAction*)));
}

void QDatabaseWorksheetViewController::executeQuery()
{
	bool bRes;
	int iDefaultSize = 20;
	QString szWorksheetQuery = m_pDatabaseWorksheetView->getWorksheetTextEdit()->toPlainText();
	bRes = m_pDatabaseController->loadWorksheetQueryResults(szWorksheetQuery, &m_pDatabaseDisplayModel);

	m_pDatabaseWorksheetView->getWorksheetTableView()->setModel(m_pDatabaseDisplayModel);
	m_pDatabaseWorksheetView->getWorksheetTableView()->setShowGrid(false);
	m_pDatabaseWorksheetView->getWorksheetTableView()->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pDatabaseWorksheetView->getWorksheetTableView()->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	m_pDatabaseWorksheetView->getWorksheetTableView()->horizontalHeader()->setStretchLastSection(true);
	m_pDatabaseWorksheetView->getWorksheetTableView()->verticalHeader()->setDefaultSectionSize(iDefaultSize);
	m_pDatabaseWorksheetView->getWorksheetTableView()->verticalHeader()->setHidden(true);
	m_pDatabaseWorksheetView->getWorksheetTableView()->sortByColumn(0, Qt::AscendingOrder);
	showWorksheetQueryInformation();

	if(bRes){
		m_pDatabaseWorksheetView->showTabData();
		m_pDatabaseWorksheetView->getExportButton()->setEnabled(true);
		addRequestHistoryToJson(szWorksheetQuery);
	} else {
		m_pDatabaseWorksheetView->showTabConsole();
	}
}

void QDatabaseWorksheetViewController::reformatSqlText()
{
	qDebug() << "Test reformat button";
}

void QDatabaseWorksheetViewController::clearWorksheetText()
{
	m_pDatabaseWorksheetView->getWorksheetTextEdit()->clear();
}

void QDatabaseWorksheetViewController::showWorksheetQueryInformation()
{
	QString szQueryInformation = m_pDatabaseController->getQueryResultString();
	QTextCursor cursor(m_pDatabaseWorksheetView->getWorksheetConsoleTextEdit()->textCursor());//Creates a cursor
	cursor.movePosition(QTextCursor::Start);//Moves the cursor to the start
	m_pDatabaseWorksheetView->getWorksheetConsoleTextEdit()->setTextCursor(cursor);//Sets the cursor position
	m_pDatabaseWorksheetView->getWorksheetConsoleTextEdit()->insertPlainText(szQueryInformation);//insert text at the cursor position
}

void QDatabaseWorksheetViewController::exportDataWorksheet()
{
	bool bEmptyText = m_pDatabaseWorksheetView->getWorksheetTextEdit()->toPlainText().isEmpty();
	if(!bEmptyText && m_pDatabaseWorksheetView->getExportButton()->isEnabled()){
		QString szErrorMsg = "";
		bool bRes = QWindowMainController::saveSQLResultsToCSV(m_pDatabaseDisplayModel, m_pDatabaseWorksheetView, m_pDatabaseWorksheetView->getWorksheetTableView()->horizontalHeader()->orientation(), szErrorMsg);
		if (!bRes){
			QMessageBox::critical(m_pDatabaseWorksheetView, tr("Error"), tr("Unable to export the data into the file:") + "<br/>" + szErrorMsg);
		}
	}else{
		QMessageBox::warning(m_pDatabaseWorksheetView, tr("Error"), tr("Unable to export the data:") + "<br/>" + tr("There is no data to export."));
	}
}

void QDatabaseWorksheetViewController::setExportButtonDisabled()
{
	m_pDatabaseWorksheetView->getExportButton()->setEnabled(false);
}

void QDatabaseWorksheetViewController::changeWorksheetTextFromHistory(QAction* action)
{
	m_pDatabaseWorksheetView->getWorksheetTextEdit()->setPlainText(action->text());
}

void QDatabaseWorksheetViewController::setJsonDatabases()
{
	QJsonDocument jsonDocument = QSettingsManager::parseToJsonDocument(m_szDatabasesJsonPath);
	QJsonObject jsonObject = jsonDocument.object();
	QJsonArray jsonArray = jsonObject.value("databases").toArray();

	int iVal = 0;
	foreach(const QJsonValue & val, jsonArray)
	{
		QString szIdentifier = val.toObject().value("identifier").toString();
		// Check if each file path exists: if not, the database is deleted from the json
		if(!QFileInfo::exists(szIdentifier)){
			jsonArray.removeAt(iVal);
			iVal--;
		}
		iVal++;
	}

	int iLastId = 0;

	if(!jsonArray.isEmpty()){
		iLastId = jsonObject.value("last_id").toInt();
	}

	jsonObject.insert("last_id", iLastId);
	jsonObject.insert("databases", jsonArray);
	jsonDocument.setObject(jsonObject);

	QSettingsManager::writeToFile(m_szDatabasesJsonPath, jsonDocument.toJson());
}

void QDatabaseWorksheetViewController::updateRequestHistory()
{
	// Update the queries in the requests history
	m_pDatabaseWorksheetView->requestHistoryClearItems();

	int iMaxSize = m_pDatabaseWorksheetView->window()->width();

	m_pDatabaseWorksheetView->getRequestHistoryMenu()->setMaximumWidth(iMaxSize);

	QString szDatabaseID = QSettingsManager::getInstance().getStringDatabaseId(m_pDatabaseController->getSqlDatabase().databaseName());
	QString szFilePath = m_szDatabasesFilePath + "database_" + szDatabaseID + ".json";

	QJsonDocument jsonDocument = QSettingsManager::parseToJsonDocument(szFilePath);
	QJsonObject jsonObject = jsonDocument.object();
	QJsonArray jsonArray = jsonObject.value("queries").toArray();

	if(jsonArray.isEmpty()){
		m_pDatabaseWorksheetView->getRequestHistoryButton()->setEnabled(false);
	}else{
		m_pDatabaseWorksheetView->getRequestHistoryButton()->setEnabled(true);

		foreach(const QJsonValue & val, jsonArray){
			QString szValue = val.toString();
			m_pDatabaseWorksheetView->requestHistoryAddItem(szValue);
		}
	}
}

void QDatabaseWorksheetViewController::setRequestHistory()
{
	QString szDatabaseIdentifier = m_pDatabaseController->getSqlDatabase().databaseName();
	QString szDatabaseName = m_pDatabaseController->getDatabaseName();

	bool bDatabaseExists = false;

	QJsonDocument jsonDocument = QSettingsManager::parseToJsonDocument(m_szDatabasesJsonPath);
	QJsonObject jsonObject = jsonDocument.object();
	QJsonValue jsonID = jsonObject.value("last_id");
	QJsonArray jsonArray = jsonObject.value("databases").toArray();

	foreach(const QJsonValue & val, jsonArray)
	{
		QString szIdentifier = val.toObject().value("identifier").toString();
		if(QString::compare(szIdentifier, szDatabaseIdentifier) == 0){
			bDatabaseExists = true;
		}
	}

	// Add a new database to databases.json
	if(!bDatabaseExists)
	{
		// Add an ID in the name of the file
		szDatabaseName = "database_" + QString::number(jsonID.toInt());
		int iNewId = jsonID.toInt() + 1;

		QJsonObject newJsonObject;
		newJsonObject.insert("identifier", szDatabaseIdentifier);
		newJsonObject.insert("name", szDatabaseName);

		jsonArray.push_back(newJsonObject);
		QJsonObject jsonArrayToObject;
		jsonArrayToObject.insert("last_id", iNewId);
		jsonArrayToObject.insert("databases", jsonArray);
		jsonDocument.setObject(jsonArrayToObject);

		bool bGoOn = QSettingsManager::writeToFile(m_szDatabasesJsonPath, jsonDocument.toJson());

		if(bGoOn)
		{
			// Create a new json file for the requests corresponding to the database
			QJsonObject newDatabaseJsonObject;
			QJsonArray newDatabaseJsonArray;
			newDatabaseJsonObject.insert("queries", newDatabaseJsonArray);
			QJsonDocument newDatabaseJsonDocument;
			newDatabaseJsonDocument.setObject(newDatabaseJsonObject);

			bGoOn = QSettingsManager::writeToFile(m_szDatabasesFilePath + szDatabaseName + ".json", newDatabaseJsonDocument.toJson());
		}

		if(!bGoOn){
			qDebug("[Worsheet History] Error while writing databases");
		}
	}
	updateRequestHistory();
}

void QDatabaseWorksheetViewController::addRequestHistoryToJson(QString szWorksheetQuery)
{
	// Check the duplicate requests and set a maximum of requests
	bool bGoOn = true;

	const int iMaxRequestHistory = 15;

	if(szWorksheetQuery.isEmpty()){
		bGoOn = false;
	}

	if(bGoOn)
	{
		QString szDatabaseID = QSettingsManager::getInstance().getStringDatabaseId(m_pDatabaseController->getSqlDatabase().databaseName());
		QString szFilePath = m_szDatabasesFilePath + "database_" + szDatabaseID + ".json";
		QJsonDocument jsonDocument = QSettingsManager::parseToJsonDocument(szFilePath);
		QJsonObject jsonObject;
		QJsonArray jsonArray = jsonDocument.object().find("queries").value().toArray();

		int iVal = 0;
		foreach(const QJsonValue & val, jsonArray){
			QString szRequest = val.toString();
			if(QString::compare(szRequest.simplified().toUpper(), szWorksheetQuery.simplified().toUpper()) == 0){
				jsonArray.removeAt(iVal);
				break;
			}
			iVal++;
		}

		if((jsonArray.size() < 0 || jsonArray.size() > iMaxRequestHistory) && bGoOn){
			bGoOn = false;
		}

		if(jsonArray.size() == iMaxRequestHistory && bGoOn){
			jsonArray.removeLast();
		}

		if(bGoOn)
		{
			jsonArray.push_front(szWorksheetQuery);
			jsonObject.insert("queries", jsonArray);
			QJsonDocument jsonDocument;
			jsonDocument.setObject(jsonObject);

			bGoOn = QSettingsManager::writeToFile(szFilePath, jsonDocument.toJson());
		}
	}

	if(bGoOn){
		updateRequestHistory();
	}
}
