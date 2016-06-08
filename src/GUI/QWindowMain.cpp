/*
 * MainWindow.cpp
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#include "QWindowMain.h"

QWindowMain::QWindowMain()
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
    QTabWidget *pDBtabs = new QTabWidget(this);
    pMainLayout->addWidget(pDBtabs);

    QWidget* pPage1 = makeDBTab();
    pDBtabs->addTab(pPage1, "freetuxtv.db");//TODO Being able to open files instead of creating this fake file window
}

void QWindowMain::createMenu()
{
    QMenu *pFileMenu = menuBar()->addMenu("&File");
    QAction *pQuitAction = new QAction("&Quit", this);
    pFileMenu->addAction(pQuitAction);
    connect(pQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *pEditMenu = menuBar()->addMenu("&Edit");
    QMenu *pViewMenu = menuBar()->addMenu("&View");
    QMenu *pHelpMenu = menuBar()->addMenu("&Help");
}

QWidget* QWindowMain::makeDBTab()
{
    QWidget *pPage1 = new QWidget();

    //Creation of other layouts within the main layout
    QHBoxLayout *pFileLayout = new QHBoxLayout(); //Creates a layout to be used for widgets in the file tab
    pPage1->setLayout(pFileLayout);

    QVBoxLayout *pSecondaryFileLayout = new QVBoxLayout(); //Layout to be used within fileLayout to allow positionning of the widgets
    pFileLayout->addLayout(pSecondaryFileLayout);

    QToolBar *pOptionButtonsToolbar = new QToolBar();
    pSecondaryFileLayout->addWidget(pOptionButtonsToolbar);

    QTabWidget *pTabInFile = new QTabWidget(pPage1);//Used to create tabs in the opened file tab
    QPushButton *pRefresh = new QPushButton("refresh", pTabInFile); //TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pRefresh);

	QPushButton *pAddFile = new QPushButton("add", pTabInFile);//TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pAddFile);

    //Creation of the "structure" Model
    QStandardItemModel *pStructureModel = new QStandardItemModel();
    QStandardItem *pTableItem = new QStandardItem("Tables");
    pStructureModel->appendRow(pTableItem);

    QStandardItem *pSystemTablesItem = new QStandardItem("System tables");
    pStructureModel->appendRow(pSystemTablesItem);

    //Creation of the view to be applied to the "structure" model
    QTreeView *pStructureView = new QTreeView();
    pStructureView->setModel(pStructureModel);
    pStructureView->header()->hide();
    pSecondaryFileLayout->addWidget(pStructureView);

    //Creation of the first tab called Worksheet
    QTabWidget *pTab1 = makeWorksheetTab();
    pTabInFile->addTab(pTab1, "worksheet");

    //Creation of the second tab called Channels
    QTabWidget *pTab2 = makeChannelTab();
    pTabInFile->addTab(pTab2, "channels");

    pFileLayout->addWidget(pTabInFile);

    return pPage1;
}

QTabWidget* QWindowMain::makeWorksheetTab()
{
	QTabWidget *pTab1 = new QTabWidget();//Creates the first tab

	QVBoxLayout *pWorksheetTabLayout = new QVBoxLayout();
	QToolBar *pWorksheetToolbar = new QToolBar();
	pWorksheetTabLayout->addWidget(pWorksheetToolbar);

	QPushButton *pButton1 = new QPushButton("button 1", pTab1);
	pWorksheetToolbar->addWidget(pButton1);

	QPushButton *pButton2 = new QPushButton("button2", pTab1);
	pWorksheetToolbar->addWidget(pButton2);

	QPushButton *pButton3 = new QPushButton("button3", pTab1);
	pWorksheetToolbar->addWidget(pButton3);

	QTextEdit *pWorksheetTextEdit = new QTextEdit();
	pWorksheetTabLayout->addWidget(pWorksheetTextEdit);

	//Creation of the worksheet view
    QTableView *pWorksheetTableView = new QTableView();
    pWorksheetTableView->horizontalHeader()->hide();
	pWorksheetTabLayout->addWidget(pWorksheetTableView);

	pTab1->setLayout(pWorksheetTabLayout);

	return pTab1;
}

QTabWidget* QWindowMain::makeChannelTab()
{
	QTabWidget *pTab2 = new QTabWidget();

	QVBoxLayout *pChannelTabLayout = new QVBoxLayout;//Layout to be used in the Channel tab
	QTabWidget *pChannelTabContent = new QTabWidget(pTab2);
	pChannelTabLayout->addWidget(pChannelTabContent);//Adding tabs to the layout
	pTab2->setLayout(pChannelTabLayout);

	QWidget *pSubTab1 = new QWidget();
	pChannelTabContent->addTab(pSubTab1, "Structure");

	QWidget *pSubTab2 = makeDataTab();
	pChannelTabContent->addTab(pSubTab2, "Data");

	QWidget *pSubTab3 = new QWidget();
	pChannelTabContent->addTab(pSubTab3, "Original SQL creation script");

	return pTab2;
}

QTabWidget* QWindowMain::makeDataTab()
{
	QTabWidget *pSubTab2 = new QTabWidget();
	return pSubTab2;
}
