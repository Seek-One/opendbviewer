/*
 * QDatabaseWorksheetView.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include "QDatabaseWorksheetView.h"
#include <QPlainTextEdit>

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
	m_pWorksheetTextEdit = new QTextEdit();
	pWorksheetViewLayout->addWidget(m_pWorksheetTextEdit);

	//Creation of a tab widget for query results and console
	QTabWidget *pWorksheetQueryTabWidget = new QTabWidget();
	pWorksheetQueryTabWidget->setTabPosition(QTabWidget::East);
	pWorksheetViewLayout->addWidget(pWorksheetQueryTabWidget);

	//Adding the result tabs
	QWidget *pWorksheetQueryResultsTab = new QWidget();
	pWorksheetQueryTabWidget->addTab(pWorksheetQueryResultsTab, "Results");

	//Creating a layout for the results tab
	QVBoxLayout *pResultsLayout = new QVBoxLayout();
	pWorksheetQueryResultsTab->setLayout(pResultsLayout);

	//Creation of a tree view and model for results tabs
	QTreeView *pWorksheetResultsTable = new QTreeView();
	pResultsLayout->addWidget(pWorksheetResultsTable);
	m_pWorksheetResultsModel = new QStandardItemModel();
	pWorksheetResultsTable->setModel(m_pWorksheetResultsModel);

	//Adds the console Tab
	QWidget *pWorksheetQueryConsoleTab = new QWidget();
	pWorksheetQueryTabWidget->addTab(pWorksheetQueryConsoleTab, "Console");

	//Creating a layout for the console tab
	QVBoxLayout *pConsoleLayout = new QVBoxLayout();
	pWorksheetQueryConsoleTab->setLayout(pConsoleLayout);

	//Creating a text edit area for the console tab
	m_pWorksheetConsoleTextEdit = new QTextEdit;
	m_pWorksheetConsoleTextEdit->setReadOnly(true);
	pConsoleLayout->addWidget(m_pWorksheetConsoleTextEdit);
}


QDatabaseWorksheetView::~QDatabaseWorksheetView()
{

}

QToolBar* QDatabaseWorksheetView::makeWorksheetToolbar()
{
	QToolBar *pWorksheetToolbar = new QToolBar();

	m_pExecuteButton = new QPushButton("Execute");
	pWorksheetToolbar->addWidget(m_pExecuteButton);
	m_pExecuteButton->setToolTip(tr("Execute the selected query"));

	m_pReformatButton = new QPushButton("Reformat");
	pWorksheetToolbar->addWidget(m_pReformatButton);
	m_pReformatButton->setToolTip(tr("Reformat SQL text"));

	m_pClearTextButton = new QPushButton("Clear");
	pWorksheetToolbar->addWidget(m_pClearTextButton);
	m_pClearTextButton->setToolTip(tr("Clear the text"));

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

QTextEdit* QDatabaseWorksheetView::getWorksheetTextEdit() const
{
	return m_pWorksheetTextEdit;
}

QTextEdit* QDatabaseWorksheetView::getWorksheetConsoleTextEdit() const
{
	return m_pWorksheetConsoleTextEdit;
}

QStandardItemModel* QDatabaseWorksheetView::getWorksheetResultsModel() const
{
	return m_pWorksheetResultsModel;
}
