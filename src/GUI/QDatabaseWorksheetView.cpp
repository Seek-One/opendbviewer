/*
 * QDatabaseWorksheetView.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include <QTreeView>

#include "QDatabaseWorksheetView.h"
#include "QSqlSourceView.h"
#include "QQueryResultView.h"

QDatabaseWorksheetView::QDatabaseWorksheetView(QWidget* parent)
		: QWidget(parent)
{
	//Creation of a layout for worksheet tab
	QVBoxLayout *pWorksheetViewLayout = new QVBoxLayout;
	setLayout(pWorksheetViewLayout);

	//Creating a toolbar for worksheet tab
	QToolBar *pWorksheetToolbar = makeWorksheetToolbar();
	pWorksheetViewLayout->addWidget(pWorksheetToolbar);

	//Creating a text edit area
	//m_pWorksheetTextEdit = new QTextEdit();
	//pWorksheetViewLayout->addWidget(m_pWorksheetTextEdit);

	m_pWorksheetTextEdit = new QSqlSourceView(this);
	pWorksheetViewLayout->addWidget(m_pWorksheetTextEdit);

	//Creation of a tab widget for query results and console

	m_pQueryResultView = new QQueryResultView();
	pWorksheetViewLayout->addWidget(m_pQueryResultView);
}


QDatabaseWorksheetView::~QDatabaseWorksheetView()
{

}

QToolBar* QDatabaseWorksheetView::makeWorksheetToolbar()
{
	QToolBar *pWorksheetToolbar = new QToolBar();

	m_pExecuteButton = new QPushButton("Execute");
	pWorksheetToolbar->addWidget(m_pExecuteButton);
	//m_pExecuteButton->setToolTip(tr("Execute the selected query"));

	m_pReformatButton = new QPushButton("Reformat");
	m_pReformatButton->setVisible(false);
	pWorksheetToolbar->addWidget(m_pReformatButton);
	//m_pReformatButton->setToolTip(tr("Reformat SQL text"));

	m_pClearTextButton = new QPushButton("Clear");
	pWorksheetToolbar->addWidget(m_pClearTextButton);
	//m_pClearTextButton->setToolTip(tr("Clear the text"));

	return pWorksheetToolbar;
}

QPushButton* QDatabaseWorksheetView::getExecuteButton() const
{
	return m_pExecuteButton;
}

QPushButton* QDatabaseWorksheetView::getReformatButton() const
{
	return m_pReformatButton;
}

QPushButton* QDatabaseWorksheetView::getClearTextButton() const
{
	return m_pClearTextButton;
}

QSqlSourceView* QDatabaseWorksheetView::getWorksheetTextEdit() const
{
	return m_pWorksheetTextEdit;
}

QTextEdit* QDatabaseWorksheetView::getWorksheetConsoleTextEdit() const
{
	return m_pQueryResultView->getConsoleTextEdit();
}

QStandardItemModel* QDatabaseWorksheetView::getWorksheetResultsModel() const
{
	return m_pQueryResultView->getDataResultsModel();
}

QTreeView* QDatabaseWorksheetView::getWorksheetTreeView() const
{
	return m_pQueryResultView->getDataTreeView();
}


void QDatabaseWorksheetView::showTabData()
{
	m_pQueryResultView->showTabData();
}

void QDatabaseWorksheetView::showTabConsole()
{
	m_pQueryResultView->showTabConsole();
}
