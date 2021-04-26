/*
 * QDatabaseWorksheetView.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include <QTreeView>
#include <QTableView>

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

	m_pExecuteButton = new QPushButton(tr("Execute"));
	pWorksheetToolbar->addWidget(m_pExecuteButton);
	//m_pExecuteButton->setToolTip(tr("Execute the selected query"));

	m_pReformatButton = new QPushButton(tr("Reformat"));
	m_pReformatButton->setVisible(false);
	pWorksheetToolbar->addWidget(m_pReformatButton);
	//m_pReformatButton->setToolTip(tr("Reformat SQL text"));

	m_pClearTextButton = new QPushButton(tr("Clear"));
	pWorksheetToolbar->addWidget(m_pClearTextButton);
	//m_pClearTextButton->setToolTip(tr("Clear the text"));

	m_pRequestHistoryButton = new QPushButton(tr("Last requests"));
	m_pRequestHistoryMenu = new QMenu();
	m_pRequestHistoryButton->setMenu(m_pRequestHistoryMenu);
	pWorksheetToolbar->addWidget(m_pRequestHistoryButton);
	//m_pRequestHistoryButton->setToolTip(tr("Get request history"));

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

QPushButton* QDatabaseWorksheetView::getRequestHistoryButton() const
{
	return m_pRequestHistoryButton;
}

QPushButton* QDatabaseWorksheetView::getImportButton() const
{
	return m_pQueryResultView->getImportButton();
}

QPushButton* QDatabaseWorksheetView::getExportButton() const
{
	return m_pQueryResultView->getExportButton();
}

QSqlSourceView* QDatabaseWorksheetView::getWorksheetTextEdit() const
{
	return m_pWorksheetTextEdit;
}

QTextEdit* QDatabaseWorksheetView::getWorksheetConsoleTextEdit() const
{
	return m_pQueryResultView->getConsoleTextEdit();
}

QTableView* QDatabaseWorksheetView::getWorksheetTableView() const
{
	return m_pQueryResultView->getDataTableView();
}

void QDatabaseWorksheetView::showTabData()
{
	m_pQueryResultView->showTabData();
}

void QDatabaseWorksheetView::showTabConsole()
{
	m_pQueryResultView->showTabConsole();
}

QMenu* QDatabaseWorksheetView::getRequestHistoryMenu() const
{
	return m_pRequestHistoryMenu;
}

void QDatabaseWorksheetView::addRequestHistoryItem(const QString& szItemText, const QString& szItemData)
{
	QAction* pAction = new QAction(szItemText, this);
	pAction->setData(szItemData);
	m_pRequestHistoryMenu->addAction(pAction);
}

void QDatabaseWorksheetView::clearRequestHistoryItems()
{
	m_pRequestHistoryMenu->clear();
}
