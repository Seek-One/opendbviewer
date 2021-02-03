/*
 * QQueryResultView.cpp
 *
 *  Created on: 8 sept. 2017
 *      Author: ebeuque
 */

#include <QVBoxLayout>
#include <QTableView>
#include <QTextEdit>
#include <QPushButton>

#include "QQueryResultView.h"

QQueryResultView::QQueryResultView(QWidget* pParent)
{
	setTabPosition(QTabWidget::South);

	QWidget *pWidget;
	QVBoxLayout *pTmpVLayout;

	// Add the result tabs
	{
		pWidget = new QWidget(this);
		addTab(pWidget, tr("Results"));

		pTmpVLayout = new QVBoxLayout();
		pWidget->setLayout(pTmpVLayout);

		//Creation of a table view and model for results tabs
		m_pDataTableView = new QTableView();
		m_pDataTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		pTmpVLayout->addWidget(m_pDataTableView);
	}

	// Add the console Tab
	{
		pWidget = new QWidget();
		addTab(pWidget, tr("Console"));

		pTmpVLayout = new QVBoxLayout();
		pWidget->setLayout(pTmpVLayout);

		m_pConsoleTextEdit = new QTextEdit;
		m_pConsoleTextEdit->setReadOnly(true);
		pTmpVLayout->addWidget(m_pConsoleTextEdit);
	}

	// Add the export button
	{
		m_pExportDataButton = new QPushButton(tr("Export"));
		setCornerWidget(m_pExportDataButton, Qt::BottomRightCorner);
	}
}

QQueryResultView::~QQueryResultView()
{
}


QTextEdit* QQueryResultView::getConsoleTextEdit() const
{
	return m_pConsoleTextEdit;
}

QTableView* QQueryResultView::getDataTableView() const
{
	return m_pDataTableView;
}

QPushButton* QQueryResultView::getExportDataButton() const
{
	return m_pExportDataButton;
}

void QQueryResultView::showTabData()
{
	setCurrentIndex(0);
}

void QQueryResultView::showTabConsole()
{
	setCurrentIndex(1);
}
