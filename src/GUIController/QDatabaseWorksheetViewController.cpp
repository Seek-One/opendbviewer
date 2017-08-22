/*
 * QDatabaseWorksheetViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "GUIController/QDatabaseWorksheetViewController.h"
#include "GUIController/QDatabaseTableViewController.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "Database/DatabaseController.h"


QDatabaseWorksheetViewController::QDatabaseWorksheetViewController()
{
	m_pDatabaseWorksheetView = NULL;
	m_szFileName = "";
	m_pDatabaseController = NULL;
}

QDatabaseWorksheetViewController::~QDatabaseWorksheetViewController()
{

}

void QDatabaseWorksheetViewController::init(QDatabaseWorksheetView* pDatabaseWorksheetView, QString& szFileName, DatabaseController* pDatabaseController)
{
	m_pDatabaseWorksheetView = pDatabaseWorksheetView;
	m_szFileName = szFileName;
	m_pDatabaseController = pDatabaseController;

	connect(m_pDatabaseWorksheetView->getExecuteButton(), SIGNAL(clicked()), this, SLOT(executeQuery()));
	connect(m_pDatabaseWorksheetView->getReformatButton(), SIGNAL(clicked()), this, SLOT(reformatSqlText()));
	connect(m_pDatabaseWorksheetView->getClearTextButton(), SIGNAL(clicked()), this, SLOT(clearWorksheetText()));
}

void QDatabaseWorksheetViewController::executeQuery()
{
	m_pDatabaseWorksheetView->getWorksheetResultsModel()->clear();
	QString szWorksheetQuery = m_pDatabaseWorksheetView->getWorksheetTextEdit()->toPlainText();
	m_pDatabaseController->loadWorksheetQueryResults(szWorksheetQuery, onDbLoadWorksheetQueryResults, this);
	showWorksheetQueryInformation();
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


void QDatabaseWorksheetViewController::onDbLoadWorksheetQueryResults(const QList<QString>& pColumnNameList, const QList<QString>& pRowData, void* user_data)
{
	QDatabaseWorksheetViewController* pWorksheetController = (QDatabaseWorksheetViewController*) (user_data);

	QList<QString> pHeader;
	pHeader << pColumnNameList;
	pWorksheetController->m_pDatabaseWorksheetView->getWorksheetResultsModel()->setHorizontalHeaderLabels(pHeader);

	//Creating a QList<QStandardItem> in order to append a row to the model
	QList<QStandardItem*> pRowDataItemList;
	QList<QString>::const_iterator iter = pRowData.begin();
	while(iter != pRowData.end())
	{
		//Getting an item from QList<QString> to add it to a QList<QStandardItem>
		QStandardItem* pDataItem = new QStandardItem(*iter);
		pDataItem->setEditable(true);
		pRowDataItemList.append(pDataItem);
		iter++;
	}
	//Appending the row with the QList<QStandardItem>
	pWorksheetController->m_pDatabaseWorksheetView->getWorksheetResultsModel()->appendRow(pRowDataItemList);
}
