/*
 * QDatabaseWorksheetViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QApplication>
#include <QPalette>
#include <QTableView>

#include "GUIController/QDatabaseWorksheetViewController.h"
#include "GUIController/QDatabaseTableViewController.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "Database/DatabaseController.h"


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

	m_pSqlHighlighterController = new QSqlHighlighterController(m_pDatabaseWorksheetView->getWorksheetTextEdit()->document());

	connect(m_pDatabaseWorksheetView->getExecuteButton(), SIGNAL(clicked()), this, SLOT(executeQuery()));
	connect(m_pDatabaseWorksheetView->getReformatButton(), SIGNAL(clicked()), this, SLOT(reformatSqlText()));
	connect(m_pDatabaseWorksheetView->getClearTextButton(), SIGNAL(clicked()), this, SLOT(clearWorksheetText()));
}

void QDatabaseWorksheetViewController::executeQuery()
{
	bool bRes;

	QString szWorksheetQuery = m_pDatabaseWorksheetView->getWorksheetTextEdit()->toPlainText();
	bRes = m_pDatabaseController->loadWorksheetQueryResults(szWorksheetQuery, &m_pDatabaseDisplayModel);

	m_pDatabaseWorksheetView->getWorksheetTableView()->setModel(m_pDatabaseDisplayModel);
	m_pDatabaseWorksheetView->getWorksheetTableView()->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pDatabaseWorksheetView->getWorksheetTableView()->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	m_pDatabaseWorksheetView->getWorksheetTableView()->horizontalHeader()->setStretchLastSection(true);
	m_pDatabaseWorksheetView->getWorksheetTableView()->verticalHeader()->setDefaultSectionSize(20);
	m_pDatabaseWorksheetView->getWorksheetTableView()->verticalHeader()->setHidden(true);
	m_pDatabaseWorksheetView->getWorksheetTableView()->sortByColumn(0, Qt::AscendingOrder);
	showWorksheetQueryInformation();

	if(bRes){
		m_pDatabaseWorksheetView->showTabData();
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
