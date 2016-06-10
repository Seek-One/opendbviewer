/*
 * QDatabaseConnectionViewController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionViewController.h"

#include "GUI/QDatabaseConnectionView.h"

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

}

void QDatabaseConnectionViewController::openWorksheet()
{
	QWidget *pNewTab = new QWidget;
	m_pDatabaseConnectionView->getTabsInConnection()->addTab(pNewTab, "worksheet");
}

void QDatabaseConnectionViewController::closeTab(const int& index)
{
	m_pDatabaseConnectionView->getTabsInConnection()->widget(index);
	m_pDatabaseConnectionView->getTabsInConnection()->removeTab(index);
}
