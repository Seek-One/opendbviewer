/*
 * QDatabaseConnectionViewController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionViewController.h"

#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QDatabaseWorksheetView.h"
#include "QDatabaseWorksheetViewController.h"

QDatabaseConnectionViewController::QDatabaseConnectionViewController()
{
	m_pDatabaseConnectionView = NULL;
}

QDatabaseConnectionViewController::~QDatabaseConnectionViewController()
{

}

void QDatabaseConnectionViewController::init(QDatabaseConnectionView* pDatabaseConnectionView)
{
	m_pDatabaseConnectionView = pDatabaseConnectionView;

	connect(m_pDatabaseConnectionView->getNewWorksheetButton(), SIGNAL(clicked()), this, SLOT(openWorksheet()));
	connect(m_pDatabaseConnectionView->getTabsInConnection(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	openWorksheet();
}

void QDatabaseConnectionViewController::openWorksheet()
{
	QDatabaseWorksheetView* pDatabaseWorksheetView = new QDatabaseWorksheetView(m_pDatabaseConnectionView);
	m_pDatabaseConnectionView->addWorksheetView(pDatabaseWorksheetView, tr("worksheet"));

	QDatabaseWorksheetViewController* pDatabaseWorksheetViewController = new QDatabaseWorksheetViewController;
	pDatabaseWorksheetViewController->init(pDatabaseWorksheetView);
}

void QDatabaseConnectionViewController::closeTab(const int& index)
{
	if(m_pDatabaseConnectionView->getTabsInConnection()->count() < 2)
	{
		return;
	}

	QWidget* tabItem = m_pDatabaseConnectionView->getTabsInConnection()->widget(index);
	m_pDatabaseConnectionView->getTabsInConnection()->removeTab(index);

	delete(tabItem);
}
