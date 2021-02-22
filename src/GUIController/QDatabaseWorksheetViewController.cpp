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
#include <QFileInfo>


#include "Database/DatabaseController.h"

#include "GUI/QDatabaseWorksheetView.h"
#include "GUI/QExportParametersDialog.h"

#include "GUIController/QDatabaseWorksheetViewController.h"
#include "GUIController/QDatabaseTableViewController.h"
#include "GUIController/QWindowMainController.h"


QDatabaseWorksheetViewController::QDatabaseWorksheetViewController()
{
	m_pDatabaseWorksheetView = NULL;
	m_pDatabaseController = NULL;
	m_pSqlHighlighterController = NULL;
	m_pDatabaseDisplayModel = NULL;
	m_bExportAvailable = false;
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

	m_pSqlHighlighterController = new QSqlHighlighterController(m_pDatabaseWorksheetView->getWorksheetTextEdit()->document());

	connect(m_pDatabaseWorksheetView->getExecuteButton(), SIGNAL(clicked()), this, SLOT(executeQuery()));
	connect(m_pDatabaseWorksheetView->getReformatButton(), SIGNAL(clicked()), this, SLOT(reformatSqlText()));
	connect(m_pDatabaseWorksheetView->getClearTextButton(), SIGNAL(clicked()), this, SLOT(clearWorksheetText()));
	connect(m_pDatabaseWorksheetView->getExportDataButton(), SIGNAL(clicked()), this, SLOT(exportDataWorksheet()));
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
		if(!szWorksheetQuery.isEmpty()){
			m_bExportAvailable = true;
		}
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
	if(m_bExportAvailable){
		QString szErrorMsg = "";
		bool bRes = QWindowMainController::saveSQLResultsToCSV(m_pDatabaseDisplayModel, m_pDatabaseWorksheetView, m_pDatabaseWorksheetView->getWorksheetTableView()->horizontalHeader()->orientation(), szErrorMsg);
		if (!bRes){
			QMessageBox::critical(m_pDatabaseWorksheetView, tr("Error"), tr("Unable to export the data into the file:") + "<br/>" + szErrorMsg);
		}
	}
}
