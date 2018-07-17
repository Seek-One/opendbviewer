/*
 * MainWindow.cpp
 *
 *  Created on: 7 juin 2016
 *      Author: echopin
 */

#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>

#include "QDatabaseConnectionView.h"
#include "Global/ApplicationSettings.h"
#include "QOpenDatabaseView.h"
#include "QWindowMain.h"
#include "Settings/QSettingsManager.h"

QWindowMain::QWindowMain(QWidget* parent)
	: QMainWindow(parent)
{
    //Set minimum window size
    setMinimumSize(800, 600);

    //Creation of the menu bar
    createMenu();

    //Creation of the tool bar
    createToolbar();

    //Creation of the central area
    QWidget *pCentralArea = new QWidget();
    setCentralWidget(pCentralArea);

    //Creation of the main layout
    QHBoxLayout *pMainLayout = new QHBoxLayout();
    pCentralArea->setLayout(pMainLayout);

    //Creation of the Menu Container
    m_pStackedMenuWidget = new QStackedWidget(this);

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    m_pStackedMenuWidget->setSizePolicy(spLeft);
    pMainLayout->addWidget(m_pStackedMenuWidget);

    //Creation of the databases tabs container
    m_pDatabaseConnectionTab = new QMidClickClosableTabWidget(this);
    m_pDatabaseConnectionTab->setTabsClosable(true);
    m_pDatabaseConnectionTab->setMovable(true);
    m_pDatabaseConnectionTab->setHidden(true);

    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(4);
    m_pDatabaseConnectionTab->setSizePolicy(spRight);
    pMainLayout->addWidget(m_pDatabaseConnectionTab);

    m_pOpenDatabaseView = new QOpenDatabaseView(this);
    m_pDatabaseConnectionTab->addTab(m_pOpenDatabaseView, tr("No selection"));

    m_pDatabaseConnectionTab->setTabNonClosable(0);
    m_pDatabaseConnectionTab->removeTab(0);

    //Menu Widgets
    m_pFavWidget = m_pOpenDatabaseView->makeFavouriteTab(m_pOpenDatabaseView);
    m_pStackedMenuWidget->addWidget(m_pFavWidget);

    m_pExplorerWidget = m_pOpenDatabaseView->makeExplorerTab(m_pOpenDatabaseView);
    m_pStackedMenuWidget->addWidget(m_pExplorerWidget);

    m_pSQLiteWidget = m_pOpenDatabaseView->makeSQLiteTab(m_pOpenDatabaseView);
    m_pStackedMenuWidget->addWidget(m_pSQLiteWidget);

	m_pMySQLWidget = m_pOpenDatabaseView->makeMySQLTab(m_pOpenDatabaseView);
	 m_pStackedMenuWidget->addWidget(m_pMySQLWidget);

    m_pPSQLWidget = m_pOpenDatabaseView->makePostgreSQLTab(m_pOpenDatabaseView);
    m_pStackedMenuWidget->addWidget(m_pPSQLWidget);

    m_pNewConnWidget = m_pOpenDatabaseView->makeNewConnMenu(m_pOpenDatabaseView);
	m_pStackedMenuWidget->addWidget(m_pNewConnWidget);
    m_pStackedMenuWidget->setHidden(true);
}

QWindowMain::~QWindowMain()
{

}

QAction* QWindowMain::getQuitAction() const
{
	return m_pQuitAction;
}

QAction* QWindowMain::getAboutAction() const
{
	return m_pAboutAction;
}

QAction* QWindowMain::getViewsAction() const
{
	return m_pViewsAction;
}

QAction* QWindowMain::getFavAction() const
{
	return m_pFavAction;
}

QAction* QWindowMain::getExplorerAction() const
{
	return m_pExplorerAction;
}

QAction* QWindowMain::getNewConnAction() const
{
	return m_pNewConnAction;
}

QAction* QWindowMain::getSQLiteAction() const
{
	return m_pSQLiteAction;
}

QAction* QWindowMain::getMySQLAction() const
{
	return m_pMySQLAction;
}

QAction* QWindowMain::getPostgreSQLAction() const
{
	return m_pPostgreSQLAction;
}

QOpenDatabaseView* QWindowMain::getOpenDatabaseView() const
{
	return m_pOpenDatabaseView;
}

QTabWidget* QWindowMain::getDatabaseConnectionTab() const
{
	return m_pDatabaseConnectionTab;
}

void QWindowMain::showViewsTab() {
	m_pStackedMenuWidget->setHidden(true);
	m_pDatabaseConnectionTab->setVisible(true);
}

void QWindowMain::addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle)
{
	m_pDatabaseConnectionTab->addTab(pDatabaseConnectionView, szTitle);

	int index = m_pDatabaseConnectionTab->currentIndex()+1;
	m_pDatabaseConnectionTab->setCurrentIndex(index);
	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setHidden(true);
}

void QWindowMain::showFavouritesDatabasesTab()
{
	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pFavWidget);

}

void QWindowMain::showExplorerTab()
{
	m_pDatabaseConnectionTab->setHidden(true);

	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pExplorerWidget);
}

void QWindowMain::showNewConnMenuTab()
{
	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pNewConnWidget);
}

void QWindowMain::OpenSQLiteTab()
{
	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pSQLiteWidget);
}

void QWindowMain::OpenMySQLTab()
{
	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pMySQLWidget);
}

void QWindowMain::OpenPostgreSQLTab()
{

	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pPSQLWidget);
}

void QWindowMain::changeEvent(QEvent* pEvent)
{
	if(pEvent->type() == QEvent::WindowStateChange){
		if(isMaximized()){
			ApplicationSettings::setWindowMaximized(true);
			QSettingsManager::getInstance().saveConfigSettings();
		}else{
			ApplicationSettings::setWindowMaximized(false);
			QSettingsManager::getInstance().saveConfigSettings();
		}
	}
	pEvent->accept();
}

void QWindowMain::createMenu()
{
    QMenu *pFileMenu = menuBar()->addMenu(tr("&File"));
    m_pQuitAction = new QAction(tr("&Quit"), this);
    pFileMenu->addAction(m_pQuitAction);

    QMenu *pHelpMenu = menuBar()->addMenu(tr("&Help"));
    m_pAboutAction = new QAction(tr("&About"), this);
    pHelpMenu->addAction(m_pAboutAction);
}

void QWindowMain::createToolbar() {
	//m_pWindowToolBar->addAction(QIcon(QPixmap(views.png)), "Views");
	m_pWindowToolBar = new QToolBar();

	m_pWindowToolBar->setWindowTitle("ToolBar");
	m_pWindowToolBar->addSeparator();

	m_pViewsAction = new QAction(tr("Views"), this);
	m_pWindowToolBar->addAction(m_pViewsAction);
	m_pWindowToolBar->addSeparator();

	m_pFavAction = new QAction(tr("Favourites Databases"), this);
	m_pWindowToolBar->addAction(m_pFavAction);
	m_pWindowToolBar->addSeparator();

	m_pExplorerAction = new QAction(tr("File Explorer"), this);
	m_pWindowToolBar->addAction(m_pExplorerAction);
	m_pWindowToolBar->addSeparator();

	m_pNewConnAction = new QAction(tr("New Connection"), this);
	m_pWindowToolBar->addAction(m_pNewConnAction);

	m_pWindowToolBar->setOrientation(Qt::Vertical);
	m_pWindowToolBar->setMovable(false);
	addToolBar(Qt::LeftToolBarArea, m_pWindowToolBar);
}
