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
#include <QFont>

#include "Controller/ConfigDatabaseController.h"
#include "Database/DatabaseController.h"
#include "Global/ApplicationSettings.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "GUIController/QDatabaseWorksheetViewController.h"
#include "GUIController/QDatabaseTableViewController.h"
#include "GUIController/QWindowMainController.h"

QDatabaseWorksheetViewController::QDatabaseWorksheetViewController()
{
	m_pDatabaseWorksheetView = NULL;
	m_pDatabaseController = NULL;
	m_pSqlHighlighterController = NULL;
	m_pDatabaseDisplayModel = NULL;
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
		addRequestHistory(szWorksheetQuery);
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
	if(!bEmptyText && m_pDatabaseWorksheetView->getExportButton()->isEnabled())
	{
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
	m_pDatabaseWorksheetView->getWorksheetTextEdit()->setPlainText(action->data().toString());
}

void QDatabaseWorksheetViewController::updateRequestHistory()
{
	// Update the queries in the requests history
	m_pDatabaseWorksheetView->clearRequestHistoryItems();

	int iMaxSize = m_pDatabaseWorksheetView->window()->width();
	m_pDatabaseWorksheetView->getRequestHistoryMenu()->setMaximumWidth(iMaxSize);

	QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(m_pDatabaseController->getSqlDatabase().databaseName());
	QStringList szListQueries;
	ApplicationSettings::getConfigDatabaseController()->loadDatabaseQueries(szDatabaseName, szListQueries);

	if(szListQueries.isEmpty()){
		m_pDatabaseWorksheetView->getRequestHistoryButton()->setEnabled(false);
	}else{
		m_pDatabaseWorksheetView->getRequestHistoryButton()->setEnabled(true);

		int iMargin = iMaxSize/10;

		foreach(const QString& szQuery, szListQueries)
		{
			QFontMetrics fontMetrics = m_pDatabaseWorksheetView->getRequestHistoryMenu()->fontMetrics();
			int iElidedTextSize = fontMetrics.width(szQuery);

			if(iElidedTextSize >= iMaxSize - iMargin){
				QString szElidedText = fontMetrics.elidedText(szQuery, Qt::ElideMiddle, iMaxSize - iMargin);
				m_pDatabaseWorksheetView->addRequestHistoryItem(szElidedText, szQuery);
			}else{
				m_pDatabaseWorksheetView->addRequestHistoryItem(szQuery, szQuery);
			}
		}
	}
}

void QDatabaseWorksheetViewController::setRequestHistory()
{
	QString szDatabaseIdentifier = m_pDatabaseController->getSqlDatabase().databaseName();

	bool bDatabaseExists = false;

	ConfigDatabaseList listConfigDatabase = ApplicationSettings::getConfigDatabaseController()->getDatabaseList();

	foreach(const ConfigDatabase& configDatabase, listConfigDatabase)
	{
		if(QString::compare(configDatabase.getDatabaseIdentifier(), szDatabaseIdentifier) == 0)
		{
			bDatabaseExists = true;
		}
	}

	// Add a new database to databases.json
	if(!bDatabaseExists)
	{
		int iNextID = ApplicationSettings::getConfigDatabaseController()->getNextID();
		bool bTakenID = false;

		do{
			bTakenID = false;
			foreach(const ConfigDatabase& configDatabase, listConfigDatabase)
			{
				if(configDatabase.getDatabaseID() == iNextID)
				{
					bTakenID = true;
					iNextID++;
				}
			}
		}while(bTakenID);

		ConfigDatabase configDatabaseNew(szDatabaseIdentifier, iNextID);
		ApplicationSettings::getConfigDatabaseController()->addDatabase(configDatabaseNew);
		bool bGoOn = ApplicationSettings::getConfigDatabaseController()->saveDatabasesList();

		if(bGoOn)
		{
			// Create a new json file for the requests corresponding to the database
			QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(szDatabaseIdentifier);
			bGoOn = ApplicationSettings::getConfigDatabaseController()->initDatabaseQueries(szDatabaseName);
		}

		if(!bGoOn){
			qDebug("[Worsheet requests history] Error while writing databases");
		}
	}
	updateRequestHistory();
}

void QDatabaseWorksheetViewController::addRequestHistory(const QString& szWorksheetQuery)
{
	// Check the duplicate requests and set a maximum of requests
	bool bGoOn = true;

	const int iMaxRequestHistory = 15;

	if(szWorksheetQuery.isEmpty()){
		bGoOn = false;
	}

	if(bGoOn)
	{
		QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(m_pDatabaseController->getSqlDatabase().databaseName());

		QStringList szListQueries;
		ApplicationSettings::getConfigDatabaseController()->loadDatabaseQueries(szDatabaseName, szListQueries);

		int iVal = 0;
		foreach(const QString& szQuery, szListQueries)
		{
			if(QString::compare(szQuery.simplified().toUpper(), szWorksheetQuery.simplified().toUpper()) == 0){
				szListQueries.removeAt(iVal);
				break;
			}
			iVal++;
		}

		if((szListQueries.size() < 0 || szListQueries.size() > iMaxRequestHistory) && bGoOn){
			bGoOn = false;
		}

		if(szListQueries.size() == iMaxRequestHistory && bGoOn){
			szListQueries.removeLast();
		}

		if(bGoOn)
		{
			ApplicationSettings::getConfigDatabaseController()->addDatabaseQuery(szWorksheetQuery, szListQueries);
			bGoOn = ApplicationSettings::getConfigDatabaseController()->saveDatabaseQueries(szDatabaseName, szListQueries);
		}
	}

	if(bGoOn){
		updateRequestHistory();
	}
}
