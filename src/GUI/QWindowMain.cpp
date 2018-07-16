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


	createAddDatabasesToolBar();
	m_pAddDatabasesToolBar->setHidden(true);

    //Creation of the central area
    QWidget *pCentralArea = new QWidget();
    setCentralWidget(pCentralArea);

    //Creation of the main layout
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pCentralArea->setLayout(pMainLayout);

    m_pMenuTab = new QTabWidget(this);
    pMainLayout->addWidget(m_pMenuTab);

    //Creation of the first set of tabs for database files
    m_pDatabaseConnectionTab = new QMidClickClosableTabWidget(this);
    m_pDatabaseConnectionTab->setTabsClosable(true);
    m_pDatabaseConnectionTab->setMovable(true);
    m_pDatabaseConnectionTab->setHidden(true);
    pMainLayout->addWidget(m_pDatabaseConnectionTab);

    m_pOpenDatabaseView = new QOpenDatabaseView(this);
    m_pDatabaseConnectionTab->addTab(m_pOpenDatabaseView, tr("No selection"));

    m_pDatabaseConnectionTab->setTabNonClosable(0);
    m_pDatabaseConnectionTab->removeTab(0);
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
	m_pMenuTab->setHidden(true);
	m_pDatabaseConnectionTab->setVisible(true);
	m_pAddDatabasesToolBar->setHidden(true);
}

void QWindowMain::addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle)
{
	m_pMenuTab->setHidden(true);
	m_pDatabaseConnectionTab->setVisible(true);
	m_pDatabaseConnectionTab->addTab(pDatabaseConnectionView, szTitle);
	int index = m_pDatabaseConnectionTab->currentIndex()+1;
	m_pDatabaseConnectionTab->setCurrentIndex(index);
	m_pAddDatabasesToolBar->setHidden(true);
}

void QWindowMain::addFavouritesDatabasesTab()
{	//OOpen a QOpenDatabaseView and creates only the tab for Favourites Databases
	m_pMenuTab->removeTab(0);
	m_pMenuTab->setVisible(true);
	m_pDatabaseConnectionTab->setHidden(true);
	/*if (m_pDatabaseConnectionTab->count()!=0) {
		m_pDatabaseConnectionTab->setVisible(true);
	}*/
	m_pAddDatabasesToolBar->setHidden(true);

	QWidget * pWidget;
	pWidget = m_pOpenDatabaseView->makeFavouriteTab(m_pOpenDatabaseView);
	m_pMenuTab->addTab(pWidget, tr("Favourites Databases"));
}

void QWindowMain::addExplorerTab()
{
	m_pMenuTab->removeTab(0);
	m_pAddDatabasesToolBar->setHidden(true);
	m_pDatabaseConnectionTab->setHidden(true);
	m_pMenuTab->setVisible(true);
	QWidget * pWidget;
	pWidget = m_pOpenDatabaseView->makeExplorerTab(m_pOpenDatabaseView);
	m_pMenuTab->addTab(pWidget, tr("File Explorer"));
}

void QWindowMain::addNewConnMenuTab()
{
	m_pMenuTab->removeTab(0);
	m_pDatabaseConnectionTab->setHidden(true);
	m_pMenuTab->setVisible(true);
	m_pAddDatabasesToolBar->setVisible(true);

}

void QWindowMain::OpenSQLiteTab()
{
	m_pMenuTab->removeTab(0);
	QWidget * pWidget;
	pWidget = m_pOpenDatabaseView->makeSQLiteTab(m_pOpenDatabaseView);
	m_pMenuTab->addTab(pWidget, tr("SQLite"));
}

void QWindowMain::OpenMySQLTab()
{
	m_pMenuTab->removeTab(0);
	QWidget * pWidget;
	pWidget = m_pOpenDatabaseView->makeMySQLTab(m_pOpenDatabaseView);
	m_pMenuTab->addTab(pWidget, tr("MySQL"));
}

void QWindowMain::OpenPostgreSQLTab()
{
	m_pMenuTab->removeTab(0);
	QWidget * pWidget;
	pWidget = m_pOpenDatabaseView->makePostgreSQLTab(m_pOpenDatabaseView);
	m_pMenuTab->addTab(pWidget, tr("PostgreSQL"));
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

void QWindowMain::createAddDatabasesToolBar() {
	m_pAddDatabasesToolBar = new QToolBar();

	m_pAddDatabasesToolBar->setWindowTitle("Databases ToolBar");
	m_pAddDatabasesToolBar->addSeparator();

	m_pSQLiteAction = new QAction(tr("SQLite"), this);
	m_pAddDatabasesToolBar->addAction(m_pSQLiteAction);
	m_pAddDatabasesToolBar->setMovable(false);

	m_pAddDatabasesToolBar->addSeparator();
	m_pMySQLAction = new QAction(tr("MySQL"), this);
	m_pAddDatabasesToolBar->addAction(m_pMySQLAction);
	m_pAddDatabasesToolBar->setMovable(false);

	m_pAddDatabasesToolBar->addSeparator();
	m_pPostgreSQLAction = new QAction(tr("PostgreSQL"), this);
	m_pAddDatabasesToolBar->addAction(m_pPostgreSQLAction);
	m_pAddDatabasesToolBar->setMovable(false);
	addToolBar(Qt::LeftToolBarArea, m_pAddDatabasesToolBar);
}
