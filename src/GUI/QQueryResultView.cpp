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

QQueryResultView::QQueryResultView(QWidget* parent)
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
		pWidget = new QWidget(this);
		addTab(pWidget, tr("Console"));

		pTmpVLayout = new QVBoxLayout();
		pWidget->setLayout(pTmpVLayout);

		m_pConsoleTextEdit = new QTextEdit;
		m_pConsoleTextEdit->setReadOnly(true);
		pTmpVLayout->addWidget(m_pConsoleTextEdit);
	}

	// Add the import/export buttons
	{
		pWidget = new QWidget();
		QHBoxLayout *pTmpHLayout = new QHBoxLayout();
		pTmpHLayout->setContentsMargins(0, 2, 0, 0);

		m_pImportButton = new QPushButton(tr("Import"));
		pTmpHLayout->addWidget(m_pImportButton);

		m_pExportButton = new QPushButton(tr("Export"));
		pTmpHLayout->addWidget(m_pExportButton);

		pWidget->setLayout(pTmpHLayout);
		setCornerWidget(pWidget, Qt::BottomRightCorner);
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

QPushButton* QQueryResultView::getImportButton() const
{
	return m_pImportButton;
}

QPushButton* QQueryResultView::getExportButton() const
{
	return m_pExportButton;
}

void QQueryResultView::showTabData()
{
	setCurrentIndex(0);
}

void QQueryResultView::showTabConsole()
{
	setCurrentIndex(1);
}
