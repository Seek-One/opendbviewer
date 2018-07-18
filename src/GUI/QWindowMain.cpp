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
#include "Image/QIconThemeFallback.h"
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
    spRight.setHorizontalStretch(5);
    m_pDatabaseConnectionTab->setSizePolicy(spRight);
    pMainLayout->addWidget(m_pDatabaseConnectionTab);

    m_pOpenDatabaseView = new QOpenDatabaseView(this);
    m_pDatabaseConnectionTab->addTab(m_pOpenDatabaseView, tr("No selection"));

    //Menu Widgets
    m_pHistWidget = m_pOpenDatabaseView->makeHistoryTab(m_pOpenDatabaseView);
    m_pStackedMenuWidget->addWidget(m_pHistWidget);

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

    pMainLayout->setSpacing(0);
    int iLeft, iTop, iRight, iBottom;
    pMainLayout->getContentsMargins(&iLeft, &iTop, &iRight, &iBottom);
    pMainLayout->setContentsMargins(0,iTop,iRight, iBottom);
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

QAction* QWindowMain::getHistAction() const
{
	return m_pHistAction;
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

void QWindowMain::showViewsTab()
{
	m_pStackedMenuWidget->setHidden(true);
	m_pDatabaseConnectionTab->setVisible(true);
}

void QWindowMain::addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle)
{
	if (m_pDatabaseConnectionTab->currentWidget()==m_pOpenDatabaseView)  {
		m_pDatabaseConnectionTab->removeTab(0);
	}
	m_pDatabaseConnectionTab->addTab(pDatabaseConnectionView, szTitle);
	int index = m_pDatabaseConnectionTab->currentIndex()+1;
	m_pDatabaseConnectionTab->setCurrentIndex(index);
	m_pDatabaseConnectionTab->setVisible(true);

	m_pStackedMenuWidget->setHidden(true);
}

void QWindowMain::showHistoryTab()
{
	m_pDatabaseConnectionTab->setVisible(true);
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pHistWidget);

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

void QWindowMain::openSQLiteTab()
{
	m_pDatabaseConnectionTab->setVisible(true);
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pSQLiteWidget);
}

void QWindowMain::openMySQLTab()
{
	m_pDatabaseConnectionTab->setVisible(true);
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pMySQLWidget);
}

void QWindowMain::openPostgreSQLTab()
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
    QMenu *pFileMenu = menuBar()->addMenu(tr("File"));
    m_pQuitAction = new QAction(tr("Quit"), this);
    pFileMenu->addAction(m_pQuitAction);

    QMenu *pHelpMenu = menuBar()->addMenu(tr("Help"));
    m_pAboutAction = new QAction(tr("About"), this);
    pHelpMenu->addAction(m_pAboutAction);
}

void QWindowMain::createToolbar() {
	m_pWindowToolBar = new QToolBar();

	m_pWindowToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	m_pWindowToolBar->setWindowTitle("ToolBar");
	m_pWindowToolBar->setContentsMargins(0,0,0,0);
	m_pWindowToolBar->addSeparator();
	//set the background with the method setStyleSheet("Background: ;")

	QWidget * ToolBarButton;
	int iHeight = 50, iWidth = 100;

	m_pViewsAction = new QAction(QIconThemeFallback::fromThemeFallback("database"), tr("Views"), this);
	m_pWindowToolBar->addAction(m_pViewsAction);
	ToolBarButton = m_pWindowToolBar->widgetForAction(m_pViewsAction);
	ToolBarButton->setFixedSize(iWidth,iHeight);
	m_pWindowToolBar->addSeparator();

	m_pHistAction = new QAction(QIconThemeFallback::fromThemeFallback("history"), tr("History"), this);
	m_pWindowToolBar->addAction(m_pHistAction);
	ToolBarButton = m_pWindowToolBar->widgetForAction(m_pHistAction);
	ToolBarButton->setFixedSize(iWidth,iHeight);
	m_pWindowToolBar->addSeparator();

	m_pExplorerAction = new QAction(QIconThemeFallback::fromThemeFallback("folder"), tr("File Explorer"), this);
	m_pWindowToolBar->addAction(m_pExplorerAction);
	ToolBarButton = m_pWindowToolBar->widgetForAction(m_pExplorerAction);
	ToolBarButton->setFixedSize(iWidth,iHeight);
	m_pWindowToolBar->addSeparator();

	m_pNewConnAction = new QAction(QIconThemeFallback::fromThemeFallback("database-add"), tr("New Connection"), this);
	m_pWindowToolBar->addAction(m_pNewConnAction);
	ToolBarButton = m_pWindowToolBar->widgetForAction(m_pNewConnAction);
	ToolBarButton->setFixedSize(iWidth,iHeight);


	m_pWindowToolBar->setOrientation(Qt::Vertical);
	m_pWindowToolBar->setMovable(false);

	addToolBar(Qt::LeftToolBarArea, m_pWindowToolBar);
}
