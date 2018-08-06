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
	m_szFileName = "";
	m_pDatabaseController = NULL;
	m_pSqlHighlighterController = NULL;
}

QDatabaseWorksheetViewController::~QDatabaseWorksheetViewController()
{
	if(m_pSqlHighlighterController){
		delete m_pSqlHighlighterController;
		m_pSqlHighlighterController = NULL;
	}
}

void QDatabaseWorksheetViewController::init(QDatabaseWorksheetView* pDatabaseWorksheetView, QString& szFileName, DatabaseController* pDatabaseController)
{
	m_pDatabaseWorksheetView = pDatabaseWorksheetView;
	m_szFileName = szFileName;
	m_pDatabaseController = pDatabaseController;

	m_pSqlHighlighterController = new QSqlHighlighterController(m_pDatabaseWorksheetView->getWorksheetTextEdit()->document());

	connect(m_pDatabaseWorksheetView->getExecuteButton(), SIGNAL(clicked()), this, SLOT(executeQuery()));
	connect(m_pDatabaseWorksheetView->getReformatButton(), SIGNAL(clicked()), this, SLOT(reformatSqlText()));
	connect(m_pDatabaseWorksheetView->getClearTextButton(), SIGNAL(clicked()), this, SLOT(clearWorksheetText()));
}

void QDatabaseWorksheetViewController::executeQuery()
{
	bool bRes;

	m_pDatabaseWorksheetView->getWorksheetResultsModel()->clear();
	QString szWorksheetQuery = m_pDatabaseWorksheetView->getWorksheetTextEdit()->toPlainText();
	bRes = m_pDatabaseController->loadWorksheetQueryResults(szWorksheetQuery, onDbLoadWorksheetQueryResults, this);
	showWorksheetQueryInformation();
	if(bRes){
		m_pDatabaseWorksheetView->showTabData();
	}else{
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


void QDatabaseWorksheetViewController::onDbLoadWorksheetQueryResults(const QStringList& listRowHeader, const QStringList& listRowData, DatabaseQueryStep step, void* user_data)
{
	QDatabaseWorksheetViewController* pWorksheetController = (QDatabaseWorksheetViewController*) (user_data);

	if(step == DBQueryStepStart){
		pWorksheetController->m_pDatabaseWorksheetView->getWorksheetResultsModel()->setHorizontalHeaderLabels(listRowHeader);
		pWorksheetController->m_pDatabaseWorksheetView->getWorksheetTableView()->resizeColumnsToContents();
	}

	//Creating a QList<QStandardItem> in order to append a row to the model
	if(step == DBQueryStepRow){
		QList<QStandardItem*> listRowDataItemList;
		QList<QString>::const_iterator iter = listRowData.begin();

		QStandardItem* pStandardItem;
		QFont font;

		QPalette palette = QApplication::palette(pWorksheetController->m_pDatabaseWorksheetView);
		QBrush nullbrush = palette.brush(QPalette::Disabled, QPalette::Text);

		while(iter != listRowData.end())
		{
			//Getting an item from QList<QString> to add it to a QList<QStandardItem>
			if((*iter).isNull()){
				pStandardItem = new QStandardItem(QString("NULL"));
				font = pStandardItem->font();
				font.setItalic(true);
				pStandardItem->setFont(font);
				pStandardItem->setForeground(nullbrush);
			}else{
				pStandardItem = new QStandardItem(*iter);
			}
			pStandardItem->setEditable(true);
			listRowDataItemList.append(pStandardItem);
			iter++;
		}
		//Appending the row with the QList<QStandardItem>
		pWorksheetController->m_pDatabaseWorksheetView->getWorksheetResultsModel()->appendRow(listRowDataItemList);
	}
}
