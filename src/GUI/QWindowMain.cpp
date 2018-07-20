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

    m_pStackedDatabaseWidget = new QStackedWidget(this);

    //Creation of the databases container
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(5);
    m_pStackedDatabaseWidget->setSizePolicy(spRight);
    pMainLayout->addWidget(m_pStackedDatabaseWidget);

    m_pDatabaseConnectionTab = new QMidClickClosableTabWidget(this);
    m_pDatabaseConnectionTab->setTabsClosable(true);
    m_pDatabaseConnectionTab->setMovable(true);

    m_pOpenDatabaseView = new QOpenDatabaseView(this);
    m_pDatabaseConnectionTab->addTab(m_pOpenDatabaseView, tr("No selection"));
    m_pDatabaseConnectionTab->removeTab(0);
    m_pStackedDatabaseWidget->addWidget(m_pDatabaseConnectionTab);

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

    m_pNoSelectWidget = m_pOpenDatabaseView->makeNoSelectionTab(m_pOpenDatabaseView);
	m_pStackedDatabaseWidget->addWidget(m_pNoSelectWidget);

    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    showViewsTab();
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
	m_pStackedDatabaseWidget->setVisible(true);

	if (m_pDatabaseConnectionTab->count()==0) {
		m_pStackedDatabaseWidget->setCurrentWidget(m_pNoSelectWidget);
	} else {
		m_pStackedDatabaseWidget->setCurrentWidget(m_pDatabaseConnectionTab);
	}
}

void QWindowMain::addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle)
{
	if (m_pDatabaseConnectionTab->currentWidget()==m_pOpenDatabaseView)  {
		m_pDatabaseConnectionTab->removeTab(0);
	}
	m_pDatabaseConnectionTab->addTab(pDatabaseConnectionView, szTitle);
	int index = m_pDatabaseConnectionTab->currentIndex()+1;
	m_pDatabaseConnectionTab->setCurrentIndex(index);
	showViewsTab();
}

void QWindowMain::showHistoryTab()
{
	showViewsTab();
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pHistWidget);
}

void QWindowMain::showExplorerTab()
{
	m_pStackedDatabaseWidget->setHidden(true);
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pExplorerWidget);
}

void QWindowMain::showNewConnMenuTab()
{
	showViewsTab();
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pNewConnWidget);
}

void QWindowMain::openSQLiteTab()
{
	showViewsTab();
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pSQLiteWidget);
}

void QWindowMain::openMySQLTab()
{
	showViewsTab();
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pMySQLWidget);
}

void QWindowMain::openPostgreSQLTab()
{
	showViewsTab();
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
	m_pWindowToolBar->addSeparator();
	m_pWindowToolBar->setOrientation(Qt::Vertical);
	m_pWindowToolBar->setMovable(false);
	m_pWindowToolBar->setStyleSheet("QToolBar {border:none; background: qlineargradient("
									"x1: 0, y1: 0, x2: 1, y2: 0,   stop: 0.2 #B0B0B0, stop: 0.4 #B9B9B9, stop: 0.6  #C0C0C0, stop: 0.8 #C9C9C9);}");

	QWidget *pToolBarButton;
	int iHeight = 50, iWidth = 70;
	QString qText, qElidedText, qTest;

	m_pViewsAction = new QAction(QIconThemeFallback::fromThemeFallback("database"), tr("Views"), this);
	m_pWindowToolBar->addAction(m_pViewsAction);
	pToolBarButton = m_pWindowToolBar->widgetForAction(m_pViewsAction);
	pToolBarButton->setFixedSize(iWidth,iHeight);
	m_pWindowToolBar->addSeparator();

	m_pHistAction = new QAction(QIconThemeFallback::fromThemeFallback("history"), tr("History"), this);
	m_pWindowToolBar->addAction(m_pHistAction);
	pToolBarButton = m_pWindowToolBar->widgetForAction(m_pHistAction);
	pToolBarButton->setFixedSize(iWidth,iHeight);
	m_pWindowToolBar->addSeparator();

	QFontMetrics metrics(pToolBarButton->font()); //Recover the size of the font to adjust the following texts

	m_pExplorerAction = new QAction(QIconThemeFallback::fromThemeFallback("folder"), tr("File Explorer"), this);
	m_pWindowToolBar->addAction(m_pExplorerAction);
	qText = m_pExplorerAction->text();
	qElidedText = metrics.elidedText(qText, Qt::ElideRight, iWidth);
	m_pExplorerAction->setIconText(qElidedText);
	qTest = m_pExplorerAction->text();
	pToolBarButton = m_pWindowToolBar->widgetForAction(m_pExplorerAction);
	pToolBarButton->setFixedSize(iWidth,iHeight);
	pToolBarButton->setToolTip(qText);
	m_pWindowToolBar->addSeparator();

	m_pNewConnAction = new QAction(QIconThemeFallback::fromThemeFallback("database-add"), tr("New Connection"), this);
	m_pWindowToolBar->addAction(m_pNewConnAction);
	qText = m_pNewConnAction->text();
	qElidedText = metrics.elidedText(qText, Qt::ElideRight, iWidth);
	m_pNewConnAction->setIconText(qElidedText);
	qTest = m_pNewConnAction->text();
	pToolBarButton = m_pWindowToolBar->widgetForAction(m_pNewConnAction);
	pToolBarButton->setFixedSize(iWidth,iHeight);
	pToolBarButton->setToolTip(qText);

	addToolBar(Qt::LeftToolBarArea, m_pWindowToolBar);
	setContextMenuPolicy(Qt::NoContextMenu);
}
