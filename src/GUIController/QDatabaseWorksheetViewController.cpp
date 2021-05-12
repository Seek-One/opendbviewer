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
#include <QWidgetAction>

#include "Controller/ConfigDatabaseController.h"
#include "Database/DatabaseController.h"
#include "Global/ApplicationSettings.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "GUIController/QDatabaseWorksheetViewController.h"
#include "GUIController/QDatabaseTableViewController.h"
#include "GUIController/QWindowMainController.h"
#include "Widget/QHistoryMenuWidget.h"

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

	initRequestHistory();

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

void QDatabaseWorksheetViewController::initRequestHistory()
{
	QString szDatabaseIdentifier = m_pDatabaseController->getSqlDatabase().databaseName();

	bool bGoOn = true;
	bool bDatabaseExists = false;

	ConfigDatabaseList listConfigDatabase = ApplicationSettings::getConfigDatabaseController()->getDatabaseList();

	int iDatabaseIndex = 0;
	ConfigDatabaseList::const_iterator iterConfig;
	for(iterConfig = listConfigDatabase.constBegin(); iterConfig != listConfigDatabase.constEnd(); ++iterConfig)
	{
		if(QString::compare(iterConfig->getDatabaseIdentifier(), szDatabaseIdentifier) == 0)
		{
			bDatabaseExists = true;
			const ConfigDatabase& configDatabase = (*iterConfig);
			ApplicationSettings::getConfigDatabaseController()->moveDatabaseFirstInList(configDatabase, iDatabaseIndex);
			break;
		}
		iDatabaseIndex++;
	}

	// Add a new database to the databases list
	if(!bDatabaseExists)
	{
		int iNextID = ApplicationSettings::getConfigDatabaseController()->getNextID();
		bool bTakenID = false;

		do{
			bTakenID = false;
			ConfigDatabaseList::const_iterator iterConfig;
			for(iterConfig = listConfigDatabase.constBegin(); iterConfig != listConfigDatabase.constEnd(); ++iterConfig)
			{
				if(iterConfig->getDatabaseID() == iNextID)
				{
					bTakenID = true;
					iNextID++;
				}
			}
		}while(bTakenID);

		ConfigDatabase configDatabaseNew(szDatabaseIdentifier, iNextID);
		ApplicationSettings::getConfigDatabaseController()->addDatabase(configDatabaseNew);

		// Create a new file to store the queries corresponding to the database
		QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(szDatabaseIdentifier);
		bGoOn = ApplicationSettings::getConfigDatabaseController()->initQueries(szDatabaseName);

		if(!bGoOn){
			qDebug("[Worsheet requests history] Error while writing databases");
		}
	}

	if(bGoOn){
		bGoOn = ApplicationSettings::getConfigDatabaseController()->saveDatabasesList();
	}

	if(bGoOn)
	{
		QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(szDatabaseIdentifier);
		QStringList szListQueries;
		bool bGetQueries = ApplicationSettings::getConfigDatabaseController()->loadQueries(szDatabaseName, szListQueries);

		if(bGetQueries)
		{
			/* Why a reverse loop ?
			* 'addQueryToMenu(szQuery)' add query as first item in menu
			* But the queries read in the file are store like that too*/
			for(int i = szListQueries.size()-1; i >= 0; i--)
			{
				addQueryToMenu(szListQueries.at(i));
			}
		}
		updateAvailabilityMenu();
	}
}

void QDatabaseWorksheetViewController::addRequestHistory(const QString& szWorksheetQuery)
{
	bool bGoOn = true;
	int iMaxRequestHistory = 15;

	if(szWorksheetQuery.isEmpty()){
		bGoOn = false;
	}

	if(bGoOn)
	{
		QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(m_pDatabaseController->getSqlDatabase().databaseName());

		QStringList szListQueries;
		bool bGetQueries = ApplicationSettings::getConfigDatabaseController()->loadQueries(szDatabaseName, szListQueries);

		if(bGetQueries)
		{
			// Check the duplicate requests
			int iCounter = 0;
			QStringList::const_iterator iterListQuery;
			for(iterListQuery = szListQueries.constBegin(); iterListQuery != szListQueries.constEnd(); ++iterListQuery)
			{
				if(QString::compare(iterListQuery->simplified().toUpper(), szWorksheetQuery.simplified().toUpper()) == 0){
					szListQueries.removeAt(iCounter);
					QAction* pActionToRemove = m_pDatabaseWorksheetView->getRequestHistoryMenu()->actions().at(iCounter);
					m_pDatabaseWorksheetView->getRequestHistoryMenu()->removeAction(pActionToRemove);
					break;
				}
				iCounter++;
			}
		}

		if((szListQueries.size() < 0 || szListQueries.size() > iMaxRequestHistory) && bGoOn){
			bGoOn = false;
		}

		if(szListQueries.size() == iMaxRequestHistory && bGoOn){
			szListQueries.removeLast();
		}

		// Add the query to the corresponding database
		if(bGoOn)
		{
			addQueryToMenu(szWorksheetQuery);
			ApplicationSettings::getConfigDatabaseController()->addQuery(szWorksheetQuery, szListQueries);
			bGoOn = ApplicationSettings::getConfigDatabaseController()->saveQueries(szDatabaseName, szListQueries);
		}

		// Update the databases list
		if(bGoOn)
		{
			ConfigDatabaseList listConfigDatabase = ApplicationSettings::getConfigDatabaseController()->getDatabaseList();

			int iDatabaseIndex = 0;
			ConfigDatabaseList::const_iterator iterConfig;
			for(iterConfig = listConfigDatabase.constBegin(); iterConfig != listConfigDatabase.constEnd(); ++iterConfig)
			{
				if(QString::compare(iterConfig->getDatabaseName(), szDatabaseName) == 0)
				{
					const ConfigDatabase& configDatabase = (*iterConfig);
					ApplicationSettings::getConfigDatabaseController()->moveDatabaseFirstInList(configDatabase, iDatabaseIndex);
					bGoOn = ApplicationSettings::getConfigDatabaseController()->saveDatabasesList();
				}
				iDatabaseIndex++;
			}
		}
	}
	updateAvailabilityMenu();
}

void QDatabaseWorksheetViewController::addQueryToMenu(const QString& szQuery)
{
	int iMaxSize = m_pDatabaseWorksheetView->window()->width();
	int iMargin = iMaxSize/10;
	m_pDatabaseWorksheetView->getRequestHistoryMenu()->setMaximumWidth(iMaxSize);

	QMenu* pMenu = m_pDatabaseWorksheetView->getRequestHistoryMenu();
	QHistoryMenuWidget* pHistoryMenuWidget = new QHistoryMenuWidget(pMenu, szQuery);

	connect(pHistoryMenuWidget, SIGNAL(signalAboutToBeRemoved(const QString&)), this, SLOT(removeQuery(const QString&)));

	QFontMetrics fontMetrics = m_pDatabaseWorksheetView->getRequestHistoryMenu()->fontMetrics();
	int iElidedTextSize = fontMetrics.width(szQuery);

	if(iElidedTextSize >= iMaxSize - iMargin){
		QString szElidedText = fontMetrics.elidedText(szQuery, Qt::ElideMiddle, iMaxSize - iMargin);
		pHistoryMenuWidget->setTextLabel(szElidedText);
	}else{
		pHistoryMenuWidget->setTextLabel(szQuery);
	}

	QWidgetAction* pWidgetAction = new QWidgetAction(this);
	pWidgetAction->setDefaultWidget(pHistoryMenuWidget);
	pWidgetAction->setData(szQuery);

	// Insert action at first in the menu
	QList<QAction*> listAction = pMenu->actions();
	if(listAction.isEmpty()){
		pMenu->addAction(pWidgetAction);
	}else{
		pMenu->insertAction(listAction.at(0), pWidgetAction);
	}
}

void QDatabaseWorksheetViewController::removeQuery(const QString& szQuery)
{
	QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(m_pDatabaseController->getSqlDatabase().databaseName());
	QStringList szListQueries;
	bool bGetQueries = ApplicationSettings::getConfigDatabaseController()->loadQueries(szDatabaseName, szListQueries);

	if(bGetQueries)
	{
		bool bGoOn = ApplicationSettings::getConfigDatabaseController()->removeQuery(szQuery, szListQueries);
		if(bGoOn){
			bGoOn = ApplicationSettings::getConfigDatabaseController()->saveQueries(szDatabaseName, szListQueries);
		}

		if(bGoOn)
		{
			QMenu* pMenu = m_pDatabaseWorksheetView->getRequestHistoryMenu();
			QList<QAction*>::const_iterator iterAction;
			for(iterAction = pMenu->actions().constBegin(); iterAction != pMenu->actions().constEnd(); ++iterAction)
			{
				QAction* pAction = (*iterAction);
				if(QString::compare(pAction->data().toString(), szQuery) == 0)
				{
					pMenu->removeAction(pAction);
					break;
				}
			}
		}
	}
	updateAvailabilityMenu();
}

void QDatabaseWorksheetViewController::updateAvailabilityMenu()
{
	QString szDatabaseName = ApplicationSettings::getConfigDatabaseController()->getDatabaseName(m_pDatabaseController->getSqlDatabase().databaseName());
	QStringList szListQueries;
	bool bGetQueries = ApplicationSettings::getConfigDatabaseController()->loadQueries(szDatabaseName, szListQueries);
	if(bGetQueries){
		m_pDatabaseWorksheetView->getRequestHistoryButton()->setEnabled(true);
	}else{
		m_pDatabaseWorksheetView->getRequestHistoryButton()->setEnabled(false);
	}
}

void QDatabaseWorksheetViewController::changeWorksheetTextFromHistory(QAction* pAction)
{
	m_pDatabaseWorksheetView->getWorksheetTextEdit()->setPlainText(pAction->data().toString());
}
