/*
 * QDatabaseSelectionViewController.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionViewController.h"
#include "QDatabaseSelectionViewController.h"
#include "GUI/QDatabaseSelectionView.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QWindowMain.h"

#include <QMessageBox>
#include <QString>
#include <QFileDialog>

QDatabaseSelectionViewController::QDatabaseSelectionViewController()
{
	m_pDatabaseSelectionView = NULL;
	m_pMainWindow = NULL;
}


QDatabaseSelectionViewController::~QDatabaseSelectionViewController()
{

}



void QDatabaseSelectionViewController::init(QWindowMain* pMainWindow, QDatabaseSelectionView* pDatabaseSelectionView)
{
	m_pMainWindow = pMainWindow;
	m_pDatabaseSelectionView = pDatabaseSelectionView;
	connect(m_pDatabaseSelectionView->getFileSelectionButton(), SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(m_pDatabaseSelectionView->getCancelButton(), SIGNAL(clicked()), this, SLOT(closeSelectionWindow()));
	connect(m_pDatabaseSelectionView->getOKButton(), SIGNAL(clicked()), this, SLOT(loadDatabase()));
}


void QDatabaseSelectionViewController::openFileDialog()
{
	m_fileName = QFileDialog::getOpenFileName(m_pDatabaseSelectionView, "Open a file", QString(), "Sqlite files (*.sqlite)");
	if(m_fileName == "")
	{
		return;
	}
	m_pDatabaseSelectionView->getFilePathField()->setText(m_fileName);
	m_pDatabaseSelectionView->getFileSelectionButton()->setText("choose another file"); //TODO image rather than text
}

void QDatabaseSelectionViewController::closeSelectionWindow()
{
	m_pDatabaseSelectionView->close();
	m_pDatabaseSelectionView->~QDatabaseSelectionView();
}

void QDatabaseSelectionViewController::loadDatabase()
{
	if(m_fileName == "")
	{
		QMessageBox::warning(m_pDatabaseSelectionView, tr("No file selected"),tr("Please choose a file to open."));
		return;
	}

	QDatabaseConnectionView* pConnectionView = new QDatabaseConnectionView(m_pMainWindow);
	m_pMainWindow->addDatabaseConnectionView(pConnectionView, tr("new tab"));

	QDatabaseConnectionViewController* pDatabaseConnectionViewController = new QDatabaseConnectionViewController(m_fileName);
	pDatabaseConnectionViewController->init(pConnectionView);

	connect(pConnectionView, SIGNAL(destroyed(QObject*)), pDatabaseConnectionViewController, SLOT(deleteLater()));

	closeSelectionWindow();
}

QString QDatabaseSelectionViewController::getFileName() const
{
	return m_fileName;
}
