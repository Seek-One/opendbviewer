/*
 * QDatabaseSelectionViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "QDatabaseSelectionViewController.h"
#include "GUI/QDatabaseSelectionView.h"

#include <QMessageBox>
#include <QString>
#include <QFileDialog>

QDatabaseSelectionViewController::QDatabaseSelectionViewController()
{
	m_pDatabaseSelectionView = NULL;
}


QDatabaseSelectionViewController::~QDatabaseSelectionViewController()
{

}



void QDatabaseSelectionViewController::init(QDatabaseSelectionView* pDatabaseSelectionView)
{
	m_pDatabaseSelectionView = pDatabaseSelectionView;
	connect(m_pDatabaseSelectionView->getFileSelectionButton(), SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(m_pDatabaseSelectionView->getCancelButton(), SIGNAL(clicked()), this, SLOT(closeSelectionWindow()));
}


void QDatabaseSelectionViewController::openFileDialog()
{
	m_pFileName = QFileDialog::getOpenFileName(m_pDatabaseSelectionView, "Open a file", QString(), "Sqlite files (*.sqlite)");
	if(m_pFileName == "")
	{
		return;
	}
	m_pDatabaseSelectionView->getFileSelectionButton()->setText(m_pFileName);
}

void QDatabaseSelectionViewController::closeSelectionWindow()
{
	m_pDatabaseSelectionView->close();
	m_pDatabaseSelectionView->~QDatabaseSelectionView();
}
