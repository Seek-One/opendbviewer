/*
 * QOpenHistoryViewController.cpp
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#include "Global/QtCompat.h"
#include "QOpenHistoryViewController.h"
#include "Global/ApplicationSettings.h"
#include "GUIModel/QHistoryDatabaseModel.h"

#include <QFormLayout>
#include <QGroupBox>

QOpenHistoryViewController::QOpenHistoryViewController(QWindowMain* pMainWindow, QWindowMainController* pMainWindowController)
{
	m_pMainWindow = pMainWindow;
	m_pMainWindowController = pMainWindowController;
	m_pOpenHistoryView = pMainWindow->getOpenHistoryView();
	m_pHistoryDatabaseModel = new QHistoryDatabaseModel();
	HTMLDelegate* delegate = new HTMLDelegate();

	m_pOpenHistoryView->getHistoryTreeView()->setModel(m_pHistoryDatabaseModel);
	m_pOpenHistoryView->getHistoryTreeView()->setItemDelegate(delegate);
	initHistoryList();
	connect(m_pOpenHistoryView, SIGNAL(callHistoryDatabase(const QModelIndex&)), this, SLOT(openHistoryDatabase(const QModelIndex&)));
	connect(m_pOpenHistoryView, SIGNAL(callHistoryInfo(const QModelIndex&)), this, SLOT(getHistoryInfo(const QModelIndex&)));
	connect(m_pOpenHistoryView, SIGNAL(improveDesign(int)), this, SLOT(resizeHistory(int)));
}

QOpenHistoryViewController::~QOpenHistoryViewController()
{
}

void QOpenHistoryViewController::getHistoryInfo(const QModelIndex& index)
{
	QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(m_pHistoryDatabaseModel->itemFromIndex(index));
	m_databaseModel = pItem->getDatabaseModel();
	QWidget* pInfoWidget = makeHistoryInfo(m_databaseModel.getDatabaseType());
	setHistoryInfo();
	if (m_pOpenHistoryView->getHistoryInfoStackedWidget()->count()!=0) {
		clearStackedWidget();
	}
	m_pOpenHistoryView->getHistoryInfoStackedWidget()->addWidget(pInfoWidget);
	m_pOpenHistoryView->getHistoryInfoStackedWidget()->setCurrentWidget(pInfoWidget);
	m_pOpenHistoryView->getHistoryInfoStackedWidget()->setVisible(true);
}

QWidget* QOpenHistoryViewController::makeHistoryInfo(DatabaseModel::DatabaseType type)
{
	QWidget* pMainWidget = new QWidget(m_pOpenHistoryView);
	pMainWidget->setStyleSheet("QWidget {border-right: 1px solid #888888; font-size:12px; color:black; font: bold;}");

	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	pMainLayout->setContentsMargins(0,0,0,0);

	QGroupBox *pGroupBox = new QGroupBox(tr("Item Informations:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);
	pGroupBox->setContentsMargins(0,0,0,0);

	int iSpace = 7;
	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	pFormLayout->setContentsMargins(0,iSpace,0,0);
	pFormLayout->setLabelAlignment(Qt::AlignHorizontal_Mask);
	//Create a second QFormLayout for the MySQL/PostgreSQL database

	m_pOpenHistoryView->getHistoryNameLabel()->setStyleSheet("font:none;");
	pFormLayout->addRow(tr("Name:"), m_pOpenHistoryView->getHistoryNameLabel());

	switch(type) {
	case DatabaseModel::SQLiteType:
		m_pOpenHistoryView->getHistoryPathLabel()->setStyleSheet("font:none;");
		pFormLayout->addRow(tr("Path:"), m_pOpenHistoryView->getHistoryPathLabel());
		break;
	case DatabaseModel::MySQLType:
		m_pOpenHistoryView->getHistoryHostLabel()->setStyleSheet("font:none;");
		m_pOpenHistoryView->getHistoryPortLabel()->setStyleSheet("font:none;");
		m_pOpenHistoryView->getHistoryUsernameLabel()->setStyleSheet("font:none;");
		pFormLayout->addRow(tr("Host:"), m_pOpenHistoryView->getHistoryHostLabel());
		pFormLayout->addRow(tr("Port:"), m_pOpenHistoryView->getHistoryPortLabel());
		pFormLayout->addRow(tr("Username:"), m_pOpenHistoryView->getHistoryUsernameLabel());
		break;
	case DatabaseModel::PostgreSQLType:
		m_pOpenHistoryView->getHistoryHostLabel()->setStyleSheet("font:none;");
		m_pOpenHistoryView->getHistoryPortLabel()->setStyleSheet("font:none;");
		m_pOpenHistoryView->getHistoryUsernameLabel()->setStyleSheet("font:none;");
		pFormLayout->addRow(tr("Host:"), m_pOpenHistoryView->getHistoryHostLabel());
		pFormLayout->addRow(tr("Port:"), m_pOpenHistoryView->getHistoryPortLabel());
		pFormLayout->addRow(tr("Username:"), m_pOpenHistoryView->getHistoryUsernameLabel());
		break;
	default:
		break;
	}
	return pMainWidget;
}

void QOpenHistoryViewController::setHistoryInfo()
{
	int iHeight = m_pOpenHistoryView->getHistoryTreeView()->height();
	DatabaseModel::DatabaseType type = m_databaseModel.getDatabaseType();
	switch(type) {
	case DatabaseModel::SQLiteType:
		setSQLiteInfo();
		m_pOpenHistoryView->getHistoryInfoStackedWidget()->setMaximumHeight(iHeight/5);
		break;
	case DatabaseModel::MySQLType:
		m_pOpenHistoryView->getHistoryNameLabel()->setText(m_databaseModel.getDatabaseName());
		m_pOpenHistoryView->getHistoryHostLabel()->setText(m_databaseModel.getDatabaseHost());
		m_pOpenHistoryView->getHistoryPortLabel()->setText(QString::number(m_databaseModel.getDatabasePort()));
		m_pOpenHistoryView->getHistoryUsernameLabel()->setText(m_databaseModel.getDatabaseUsername());
		m_pOpenHistoryView->getHistoryInfoStackedWidget()->setMaximumHeight(iHeight/4);
		break;
	case DatabaseModel::PostgreSQLType:
		m_pOpenHistoryView->getHistoryNameLabel()->setText(m_databaseModel.getDatabaseName());
		m_pOpenHistoryView->getHistoryHostLabel()->setText(m_databaseModel.getDatabaseHost());
		m_pOpenHistoryView->getHistoryPortLabel()->setText(QString::number(m_databaseModel.getDatabasePort()));
		m_pOpenHistoryView->getHistoryUsernameLabel()->setText(m_databaseModel.getDatabaseUsername());
		m_pOpenHistoryView->getHistoryInfoStackedWidget()->setMaximumHeight(iHeight/4);
		break;
	default:
		break;
	}
}

void QOpenHistoryViewController::setSQLiteInfo()
{
	QString szName, qSection, qTemp;
	szName = m_databaseModel.getDatabasePath().section('/', -1);

	int i = 1, j = 0, iCorrect = 30; //iCorrect correct a side effet on the Info History Widget
	//Count the sections in the path
	while (qSection != m_databaseModel.getDatabasePath()) {
		qSection = m_databaseModel.getDatabasePath().section('/', 0, i);
		i++;
	}
	qSection = "";
	//Set the History Path Label
	while (j<i) {
		qTemp = qSection + m_databaseModel.getDatabasePath().section('/', j, j+1) + '/';

#ifdef USE_QTFMHORIZONTALADVANCE
		int iLabelSize = m_pOpenHistoryView->getHistoryPathLabel()->fontMetrics().horizontalAdvance(qTemp);
#else
		int iLabelSize = m_pOpenHistoryView->getHistoryPathLabel()->fontMetrics().width(qTemp);
#endif
		if (iLabelSize<m_iWindowWidth-iCorrect)
		{
			qSection = qTemp;
		} else {
			if (j > 0) {
				qSection += "\n/";
			}
			qSection += m_databaseModel.getDatabasePath().section('/', j, j+1);
		}
		m_pOpenHistoryView->getHistoryPathLabel()->setText(qSection);
		j+=2;
	}
	m_pOpenHistoryView->getHistoryNameLabel()->setText(szName);
}

void QOpenHistoryViewController::clearStackedWidget()
{
	int size = m_pOpenHistoryView->getHistoryInfoStackedWidget()->count();
	for (int i = 0; i<=size; i++) {
		QWidget * w = m_pOpenHistoryView->getHistoryInfoStackedWidget()->currentWidget();
		m_pOpenHistoryView->getHistoryInfoStackedWidget()->removeWidget(w);
	}
}

void QOpenHistoryViewController::initHistoryList()
{
	HistoryDatabaseList list = ApplicationSettings::getHistoryList();
	DatabaseModel database;

	m_pHistoryDatabaseModel->clear();
	for (int row = list.size() - 1 ; row >= 0 ; row--){
		database = list.at(row);
		QHistoryDatabaseItem *item = new QHistoryDatabaseItem(database);
		m_pHistoryDatabaseModel->appendRow(item);
	}
}

void QOpenHistoryViewController::openHistoryDatabase(const QModelIndex& index)
{
	QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(m_pHistoryDatabaseModel->itemFromIndex(index));
	DatabaseModel databaseModel = pItem->getDatabaseModel();

	m_pOpenHistoryView->getHistoryInfoStackedWidget()->setHidden(true);
	m_pMainWindowController->callHistoryDatabaseLoading(databaseModel);
}

void QOpenHistoryViewController::resizeHistory(int iWidth)
{
	m_iWindowWidth = iWidth;
	m_pHistoryDatabaseModel->obtainInfoDesign(iWidth, m_pOpenHistoryView->getHistoryTreeView()->font());
	setHistoryInfo();
}
