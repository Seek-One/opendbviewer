/*
 * QDatabaseWorksheetViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <GUIController/QDatabaseWorksheetViewController.h>
#include <GUI/QDatabaseWorksheetView.h>

QDatabaseWorksheetViewController::QDatabaseWorksheetViewController()
{
	m_pDatabaseWorksheetView = NULL;
}

QDatabaseWorksheetViewController::~QDatabaseWorksheetViewController()
{

}

void QDatabaseWorksheetViewController::init(QDatabaseWorksheetView* pDatabaseWorksheetView)
{
	m_pDatabaseWorksheetView = pDatabaseWorksheetView;
}
