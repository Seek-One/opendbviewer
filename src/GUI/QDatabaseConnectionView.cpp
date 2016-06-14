/*
 * QDatabaseConnectionView.cpp
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#include "QDatabaseConnectionView.h"
#include "GUIController/QWindowMainController.h"
#include "Database/DatabaseController.h"

QDatabaseConnectionView::QDatabaseConnectionView(QWidget* parent)
		: QWidget(parent)
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	//Creation of the connection tab widget
	QTabWidget *pPage1 = new QTabWidget(this);
	pMainLayout->addWidget(pPage1);

	QHBoxLayout *pDatabaseConnectionLayout = new QHBoxLayout(); //Creates a layout to be used for widgets in the connection tab
	pPage1->setLayout(pDatabaseConnectionLayout);

	QSplitter *pDatabaseConnectionSplitter = new QSplitter();
	pDatabaseConnectionSplitter->setChildrenCollapsible(false);
	pDatabaseConnectionLayout->addWidget(pDatabaseConnectionSplitter);

	QWidget *pVertConnectionPanel = new QWidget;
	pVertConnectionPanel = makeVerticalConnectionPanel();
	pDatabaseConnectionSplitter->addWidget(pVertConnectionPanel);

	m_pTabsInConnection = new QTabWidget();//Used to create a tab widget in the opened connection tab
	m_pTabsInConnection->setTabsClosable(true);

	//Creation of the second tab called Channels
	m_pTab2 = new QWidget;
	m_pTab2 = makeChannelTab();
	m_pTabsInConnection->addTab(m_pTab2, "channels");

	pDatabaseConnectionSplitter->addWidget(m_pTabsInConnection);
}

QDatabaseConnectionView::~QDatabaseConnectionView()
{

}

void QDatabaseConnectionView::addWorksheetView(QDatabaseWorksheetView* pDatabaseWorksheetView, const QString& szTitle)
{
	m_pTabsInConnection->addTab(pDatabaseWorksheetView, szTitle);
}

QWidget* QDatabaseConnectionView::makeWorksheetTab()
{
	m_pTab1 = new QWidget();
	QVBoxLayout *pWorksheetTabLayout = new QVBoxLayout();
	m_pTab1->setLayout(pWorksheetTabLayout);

	QDatabaseWorksheetView* pWorksheetView = new QDatabaseWorksheetView(this);
	pWorksheetTabLayout->addWidget(pWorksheetView);

	return m_pTab1;
}

QWidget* QDatabaseConnectionView::makeChannelTab()
{
	QWidget *pTab2 = new QWidget();
	QVBoxLayout *pChannelTabLayout = new QVBoxLayout;//Layout to be used in the Channel tab
	pTab2->setLayout(pChannelTabLayout);

	QDatabaseTableView* tableView = new QDatabaseTableView(this);
	pChannelTabLayout->addWidget(tableView);

	return pTab2;
}

QWidget* QDatabaseConnectionView::makeVerticalConnectionPanel()
{
	QWidget *pVertConnectionPanel = new QWidget;

	QVBoxLayout *pVertConnectionPanelLayout = new QVBoxLayout;
	pVertConnectionPanel->setLayout(pVertConnectionPanelLayout);

	QToolBar *pOptionButtonsToolbar = new QToolBar();
	pOptionButtonsToolbar = makeOptionButtonsToolBar();
	pVertConnectionPanelLayout->addWidget(pOptionButtonsToolbar);

	m_pTableTreeView = new QTreeView();
	pVertConnectionPanelLayout->addWidget(m_pTableTreeView);

	return pVertConnectionPanel;
}

QToolBar* QDatabaseConnectionView::makeOptionButtonsToolBar()
{
	QToolBar *pOptionButtonsToolbar = new QToolBar;

	QPushButton *pRefresh = new QPushButton("Refresh", this); //TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(pRefresh);

	m_pNewWorksheetButton= new QPushButton("New", this);//TODO needs image rather than text
	pOptionButtonsToolbar->addWidget(m_pNewWorksheetButton);

	return pOptionButtonsToolbar;
}

void QDatabaseConnectionView::setTablesModel(QStandardItemModel* pModel)
{
	m_pTableTreeView->setModel(pModel);
	m_pTableTreeView->header()->hide();
}

QPushButton* QDatabaseConnectionView::getNewWorksheetButton() const
{
	return m_pNewWorksheetButton;
}

QAction* QDatabaseConnectionView::getNewWorksheetAction() const
{
	return m_pNewWorksheetAction;
}

QTabWidget* QDatabaseConnectionView::getTabsInConnection() const
{
	return m_pTabsInConnection;
}

QWidget* QDatabaseConnectionView::getWorksheetTab() const
{
	return m_pTab1;
}
