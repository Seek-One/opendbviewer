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
#include "QOpenHistoryView.h"
#include "QWindowMain.h"
#include "Settings/QSettingsManager.h"

QWindowMain::QWindowMain(QWidget* parent)
	: QMainWindow(parent)
{
	//Set minimum window size
    setMinimumSize(800, 600);
    setAcceptDrops(true);

    //Creation of the menu bar and the toolbar
    createMenu();
    createToolbar();

    //Creation of the central area
    QWidget *pCentralArea = new QWidget();
    setCentralWidget(pCentralArea);

    //Creation of the main layout
    QHBoxLayout *pMainLayout = new QHBoxLayout();
    pCentralArea->setLayout(pMainLayout);
    pMainLayout->setContentsMargins(0,0,0,0);

    m_pMainSplitter = new QSplitter(pCentralArea);
    m_pMainSplitter->setContentsMargins(0,0,0,0);
    m_pMainSplitter->setChildrenCollapsible(false);
    pMainLayout->addWidget(m_pMainSplitter);

    QString szName = tr("Drop files here");
    m_pDropArea = new QDropAreaWidget(szName, this);

    //Creation of the Menu Container
    m_pStackedMenuWidget = new QStackedWidget();
    m_pMainSplitter->addWidget(m_pStackedMenuWidget);

    //Creation of the Database Container
    m_pStackedDatabaseWidget = new QStackedWidget();
    m_pMainSplitter->addWidget(m_pStackedDatabaseWidget);

    int iMinMenu = 180, iMinView = 200;
	m_pStackedDatabaseWidget->setMinimumWidth(iMinView);
	m_pStackedMenuWidget->setMinimumWidth(iMinMenu);

    m_pMainSplitter->setStretchFactor(0,1);
    m_pMainSplitter->setStretchFactor(1,10);

    //Databases Widgets
    m_pDatabaseConnectionTab = new QMidClickClosableTabWidget(this);
    m_pDatabaseConnectionTab->setTabsClosable(true);
    m_pDatabaseConnectionTab->setMovable(true);
    m_pStackedDatabaseWidget->addWidget(m_pDatabaseConnectionTab);

    m_pNoSelectWidget = makeNoSelectionTab();
    m_pStackedDatabaseWidget->addWidget(m_pNoSelectWidget);

    //Menu Widgets
    m_pOpenDatabaseView = new QOpenDatabaseView(this);
    m_pStackedMenuWidget->addWidget(m_pOpenDatabaseView);

    m_pOpenHistoryView = new QOpenHistoryView(this);
    m_pStackedMenuWidget->addWidget(m_pOpenHistoryView);

    m_pExplorerWidget = makeExplorerTab();
    m_pStackedMenuWidget->addWidget(m_pExplorerWidget);

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

QDropAreaWidget* QWindowMain::getDropArea() const
{
	return m_pDropArea;
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

QOpenDatabaseView* QWindowMain::getOpenDatabaseView() const
{
	return m_pOpenDatabaseView;
}

QOpenHistoryView* QWindowMain::getOpenHistoryView() const
{
	return m_pOpenHistoryView;
}

QMidClickClosableTabWidget* QWindowMain::getDatabaseConnectionTab() const
{
	return m_pDatabaseConnectionTab;
}

QFileExplorerWidget* QWindowMain::getFileExplorerWidget() const
{
	return m_pFileExplorerWidget;
}

QStackedWidget* QWindowMain::getStackedDatabaseWidget() const
{
	return m_pStackedDatabaseWidget;
}

QWidget * QWindowMain::getNoSelectionWidget() const
{
	return m_pNoSelectWidget;
}

void QWindowMain::showViewsTab()
{
	m_pStackedMenuWidget->setHidden(true);
	m_pStackedDatabaseWidget->setVisible(true);

	if (m_pDatabaseConnectionTab->count() == 0) {
		m_pStackedDatabaseWidget->setCurrentWidget(m_pNoSelectWidget);
	} else {
		m_pStackedDatabaseWidget->setCurrentWidget(m_pDatabaseConnectionTab);
	}
}

void QWindowMain::addDatabaseConnectionView(QDatabaseConnectionView* pDatabaseConnectionView, const QString& szTitle)
{
	m_pDatabaseConnectionTab->addTab(pDatabaseConnectionView, szTitle);
	int index = m_pDatabaseConnectionTab->currentIndex()+1;
	m_pDatabaseConnectionTab->setCurrentIndex(index);
	showViewsTab();
}

void QWindowMain::showHistoryTab()
{
	showViewsTab();
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pOpenHistoryView);
}

void QWindowMain::showExplorerTab()
{
	m_pStackedDatabaseWidget->setHidden(true);
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pExplorerWidget);
}

void QWindowMain::showMenuConnTab()
{
	showViewsTab();
	m_pStackedMenuWidget->setVisible(true);
	m_pStackedMenuWidget->setCurrentWidget(m_pOpenDatabaseView);
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

void QWindowMain::resizeEvent(QResizeEvent* pEvent)
{
    m_pDropArea->resize(this->size());
}

void QWindowMain::dragEnterEvent(QDragEnterEvent* pEvent)
{
	emit dragEnterTriggered(pEvent);
}

void QWindowMain::dragLeaveEvent(QDragLeaveEvent* pEvent)
{
	emit dragLeaveTriggered(pEvent);
}

void QWindowMain::dropEvent(QDropEvent* pEvent)
{
	emit dropTriggered(pEvent);
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
	m_pWindowToolBar = new QToolBar(this);

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
	pToolBarButton = m_pWindowToolBar->widgetForAction(m_pExplorerAction);
	pToolBarButton->setFixedSize(iWidth,iHeight);
	pToolBarButton->setToolTip(qText);
	m_pWindowToolBar->addSeparator();

	m_pNewConnAction = new QAction(QIconThemeFallback::fromThemeFallback("database-add"), tr("New Connection"), this);
	m_pWindowToolBar->addAction(m_pNewConnAction);
	qText = m_pNewConnAction->text();
	qElidedText = metrics.elidedText(qText, Qt::ElideRight, iWidth);
	m_pNewConnAction->setIconText(qElidedText);
	pToolBarButton = m_pWindowToolBar->widgetForAction(m_pNewConnAction);
	pToolBarButton->setFixedSize(iWidth,iHeight);
	pToolBarButton->setToolTip(qText);

	addToolBar(Qt::LeftToolBarArea, m_pWindowToolBar);
	setContextMenuPolicy(Qt::NoContextMenu);
}

QWidget * QWindowMain::makeExplorerTab()
{
	QWidget* pMainWidget = new QWidget(this);
	m_pFileExplorerWidget = new QFileExplorerWidget();

	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	pMainLayout->setSpacing(0);
	pMainLayout->setContentsMargins(0,0,0,0);

	pMainLayout->addWidget(m_pFileExplorerWidget);

	return pMainWidget;
}

QWidget* QWindowMain::makeNoSelectionTab()
{
	QWidget* pMainWidget = new QWidget(this);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	pMainWidget->setStyleSheet("background: #EEEEEE; font-size:18px; font:bold italic");
	pMainLayout->setContentsMargins(0,0,0,0);

	QLabel * pLabel = new QLabel();
	pLabel->setText(tr("No selection"));
	pMainLayout->addWidget(pLabel, 1, Qt::AlignCenter);

	return pMainWidget;
}

void QWindowMain::enableBlurEffect(bool bGo)
{
	if (bGo) {
		m_pWindowToolBar->setGraphicsEffect(new QGraphicsBlurEffect);
		m_pStackedDatabaseWidget->setGraphicsEffect(new QGraphicsBlurEffect);
		m_pStackedMenuWidget->setGraphicsEffect(new QGraphicsBlurEffect);
		menuBar()->setGraphicsEffect(new QGraphicsBlurEffect);
	} else {
		m_pWindowToolBar->setGraphicsEffect(0);
		m_pStackedDatabaseWidget->setGraphicsEffect(0);
		m_pStackedMenuWidget->setGraphicsEffect(0);
		menuBar()->setGraphicsEffect(0);
	}
}
