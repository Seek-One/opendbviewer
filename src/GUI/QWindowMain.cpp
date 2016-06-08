/*
 * MainWindow.cpp
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#include "QWindowMain.h"
#include "QDatabaseConnectionView.h"

QWindowMain::QWindowMain(QWidget* parent)
	: QMainWindow(parent)
{
    //Set minimum window size
    setMinimumSize(600, 600);

    //Creation of the menu bar
    createMenu();

    //Creation of the central area
    QWidget *pCentralArea = new QWidget();
    setCentralWidget(pCentralArea);

    //Creation of the main layout
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pCentralArea->setLayout(pMainLayout);

    //Creation of the first set of tabs for database files
    m_pDatabaseConnectionTab = new QTabWidget(this);
    pMainLayout->addWidget(m_pDatabaseConnectionTab);

    newDatabaseConnection();
}

QWindowMain::~QWindowMain()
{

}

void QWindowMain::createMenu()
{
	QAction *pAction;

    QMenu *pFileMenu = menuBar()->addMenu(tr("&File"));
    pAction = new QAction(tr("&New connection"), this);
    pFileMenu->addAction(pAction);

    connect(pAction, SIGNAL(triggered()), this, SLOT(newDatabaseConnection()));

    pAction = new QAction(tr("&Quit"), this);
    pFileMenu->addAction(pAction);

    connect(pAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *pEditMenu = menuBar()->addMenu(tr("&Edit"));
    QMenu *pViewMenu = menuBar()->addMenu(tr("&View"));
    QMenu *pHelpMenu = menuBar()->addMenu(tr("&Help"));
}

void QWindowMain::newDatabaseConnection()
{
	qDebug("clicked");

	QDatabaseConnectionView* pConnectionView = new QDatabaseConnectionView(this);

	m_pDatabaseConnectionTab->addTab(pConnectionView, tr("new tab"));
}
