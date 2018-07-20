/*
 * QOpenDatabaseDialog.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QValidator>
#include <QVBoxLayout>
#include <QScrollBar>

#include "QOpenDatabaseView.h"

QOpenDatabaseView::QOpenDatabaseView(QWidget* parent)
		: QWidget(parent)
{
	QHBoxLayout *pMainLayout = new QHBoxLayout();
	setLayout(pMainLayout);

	QVBoxLayout *pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	m_pConnectionTypeTabWidget = new QTabWidget(this);
	pSecondLayout->addWidget(m_pConnectionTypeTabWidget);

	m_pHistoryTreeWidget = new QTreeWidget(m_pConnectionTypeTabWidget);

	m_pFileExplorerWidget = new QFileExplorerWidget();


	m_pSQLiteFilePathField = new QLineEdit();
	m_pSQLiteFileSelectionButton = new QPushButton(tr("Browse"), m_pConnectionTypeTabWidget);
	m_pSQLiteButton = new QPushButton(tr("OK"));
	connect(m_pSQLiteButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));
	QString szDropAreaName = tr("Drag and drop \n your files here");
	m_pDropAreaWidget = new QDropAreaWidget(szDropAreaName, m_pConnectionTypeTabWidget);


	m_pMySQLHostField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pMySQLPortField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pMySQLUsernameField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pMySQLPasswordField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pMySQLDatabaseField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pMySQLConnectButton = new QPushButton(tr("Connect"));
	connect(m_pMySQLConnectButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));

	m_pPSQLHostField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pPSQLPortField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pPSQLUsernameField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pPSQLPasswordField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pPSQLDatabaseField = new QLineEdit(m_pConnectionTypeTabWidget);
	m_pPostgreSQLConnectButton = new QPushButton(tr("Connect"));
	connect(m_pPostgreSQLConnectButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));

	m_pSQLiteSelection = new QPushButton(tr("Open SQLite Database"));
	m_pMySQLSelection = new QPushButton(tr("Open MySQL Database"));
	m_pPSQLSelection = new QPushButton(tr("Open PostgreSQL Database"));

	connect(m_pHistoryTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onHistoryTreeWidgetDoubleClicked(QTreeWidgetItem *, int)));
}



QOpenDatabaseView::~QOpenDatabaseView()
{

}

QTabWidget* QOpenDatabaseView::getConnectionTypeTabWidget() const
{
	return m_pConnectionTypeTabWidget;
}

QPushButton* QOpenDatabaseView::getSQLiteFileSelectionButton() const
{
	return m_pSQLiteFileSelectionButton;
}

QLineEdit* QOpenDatabaseView::getSQLiteFilePathField() const

{
	return m_pSQLiteFilePathField;
}

QDropAreaWidget* QOpenDatabaseView::getDropAreaWidget() const
{
	return m_pDropAreaWidget;
}

QFileExplorerWidget* QOpenDatabaseView::getFileExplorerWidget() const
{
	return m_pFileExplorerWidget;
}

QTreeWidget* QOpenDatabaseView::getHistoryTreeWidget() const
{
	return m_pHistoryTreeWidget;
}

QLineEdit* QOpenDatabaseView::getMySQLHostField() const
{
	return m_pMySQLHostField;
}

QLineEdit* QOpenDatabaseView::getMySQLPortField() const
{
	return m_pMySQLPortField;
}

QLineEdit* QOpenDatabaseView::getMySQLUsernameField() const
{
	return m_pMySQLUsernameField;
}

QLineEdit* QOpenDatabaseView::getMySQLPasswordField() const
{
	return m_pMySQLPasswordField;
}

QLineEdit* QOpenDatabaseView::getMySQLDatabaseField() const
{
	return m_pMySQLDatabaseField;
}

QLineEdit* QOpenDatabaseView::getPSQLHostField() const
{
	return m_pPSQLHostField;
}

QLineEdit* QOpenDatabaseView::getPSQLPortField() const
{
	return m_pPSQLPortField;
}

QLineEdit* QOpenDatabaseView::getPSQLUsernameField() const
{
	return m_pPSQLUsernameField;
}

QLineEdit* QOpenDatabaseView::getPSQLPasswordField() const
{
	return m_pPSQLPasswordField;
}

QLineEdit* QOpenDatabaseView::getPSQLDatabaseField() const
{
	return m_pPSQLDatabaseField;
}

QPushButton* QOpenDatabaseView::getSQLiteButton() const
{
	return m_pSQLiteButton;
}

QPushButton* QOpenDatabaseView::getMySQLConnectButton() const
{
	return m_pMySQLConnectButton;
}

QPushButton* QOpenDatabaseView::getPostgreSQLConnectButton() const
{
	return m_pPostgreSQLConnectButton;
}

QPushButton* QOpenDatabaseView::getSQLiteSelection() const
{
	return m_pSQLiteSelection;
}

QPushButton* QOpenDatabaseView::getMySQLSelection() const
{
	return m_pMySQLSelection;
}

QPushButton* QOpenDatabaseView::getPostgreSQLSelection() const
{
	return m_pPSQLSelection;
}

QWidget* QOpenDatabaseView::makeNoSelectionTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	pMainWidget->setStyleSheet("background: #EEEEEE; font-size:18px; font:bold italic");
	pMainLayout->setContentsMargins(0,0,0,0);

	QLabel * pLabel = new QLabel();
	pLabel->setText(tr("No selection"));
	pMainLayout->addWidget(pLabel, 1, Qt::AlignCenter);

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeHistoryTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);

	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	pMainLayout->setSpacing(0);
	pMainLayout->setContentsMargins(0,0,0,0);

	m_pHistoryTreeWidget->setHeaderLabel(tr("History"));
	m_pHistoryTreeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
	pMainLayout->addWidget(m_pHistoryTreeWidget);

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeExplorerTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	pMainLayout->setSpacing(0);
	pMainLayout->setContentsMargins(0,0,0,0);

	pMainLayout->addWidget(m_pFileExplorerWidget);
	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeNewConnMenu(QWidget* pParent)
{

	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	pMainWidget->setMaximumWidth(300);

	pMainLayout->setContentsMargins(10,0,10,0);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection Database choice :"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);
	pGroupBox->setContentsMargins(0,0,0,0);

	QVBoxLayout* pTmpLayout = new QVBoxLayout();
	pTmpLayout->addWidget(m_pSQLiteSelection);
	pTmpLayout->addWidget(m_pMySQLSelection);
	pTmpLayout->addWidget(m_pPSQLSelection);
	pTmpLayout->setContentsMargins(0,20,0,0);

	pGroupBox->setLayout(pTmpLayout);

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeSQLiteTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	pMainWidget->setMaximumWidth(300);

	int iSpace = 10;
	pMainLayout->setContentsMargins(iSpace,0,0,0);

	QGroupBox *pGroupBox = new QGroupBox(tr("SQLite Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);
	pFormLayout->setContentsMargins(0,iSpace,iSpace,0);

	QBoxLayout* pTmpLayout;
	// File field
	{
		pTmpLayout = new QVBoxLayout();
		pTmpLayout->addWidget(m_pSQLiteFilePathField);
		pTmpLayout->addWidget(m_pSQLiteFileSelectionButton, 1, Qt::AlignRight);
		pTmpLayout->addWidget(m_pSQLiteButton, 1, Qt::AlignLeft);
		pFormLayout->addRow(pTmpLayout);
		pTmpLayout->setContentsMargins(0,iSpace,0,0);
	}

	{
		pTmpLayout = new QVBoxLayout();
		pTmpLayout->addWidget(m_pDropAreaWidget);
		pTmpLayout->setContentsMargins(0,iSpace,iSpace,iSpace);
	}
	pMainLayout->addLayout(pTmpLayout, 3);
	pGroupBox->setContentsMargins(0,0,0,0);

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeMySQLTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	pMainWidget->setMaximumWidth(300);

	int iSpace = 10;
	pMainLayout->setContentsMargins(iSpace,0,iSpace,0);

	QGroupBox *pGroupBox = new QGroupBox(tr("MySQL Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	// Host field
	{
		pFormLayout->addRow(tr("Host:"), m_pMySQLHostField);
	}
	// Port field
	{
		QValidator *pPortValidator = new QIntValidator(0, 65535, this);
		m_pMySQLPortField->setValidator(pPortValidator);
		pFormLayout->addRow(tr("Port:"), m_pMySQLPortField);
	}
	// Username field
	{
		pFormLayout->addRow(tr("Username:"), m_pMySQLUsernameField);
	}
	// Password field
	{
		m_pMySQLPasswordField->setEchoMode(QLineEdit::Password);
		pFormLayout->addRow(tr("Password:"), m_pMySQLPasswordField);
	}
	// Database field
	{
		pFormLayout->addRow(tr("Database:"), m_pMySQLDatabaseField);
	}
	iSpace = 20;
	pFormLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	pFormLayout->setContentsMargins(0,0,0,0);
	pGroupBox->setContentsMargins(0,iSpace,0,0);

	pMainLayout->addWidget(m_pMySQLConnectButton, 1, Qt::AlignRight);
	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makePostgreSQLTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	int iSpace = 10;
	pMainLayout->setContentsMargins(iSpace,0,iSpace,0);


	QGroupBox *pGroupBox = new QGroupBox(tr("PostgreSQL Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

	iSpace = 20;
	pGroupBox->setContentsMargins(0,iSpace,0,0);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	// Host field
	{
		pFormLayout->addRow(tr("Host:"), m_pPSQLHostField);
	}
	// Port field
	{
		QValidator *pPortValidator = new QIntValidator(0, 65535, this);
		m_pPSQLPortField->setValidator(pPortValidator);
		pFormLayout->addRow(tr("Port:"), m_pPSQLPortField);
	}
	// Username field
	{
		pFormLayout->addRow(tr("Username:"), m_pPSQLUsernameField);
	}
	// Password field
	{
		m_pPSQLPasswordField->setEchoMode(QLineEdit::Password);
		pFormLayout->addRow(tr("Password:"), m_pPSQLPasswordField);
	}
	// Database field
	{
		pFormLayout->addRow(tr("Database:"), m_pPSQLDatabaseField);
	}
	pFormLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	pFormLayout->setContentsMargins(0,0,0,0);

	pMainLayout->addWidget(m_pPostgreSQLConnectButton, 1, Qt::AlignRight);
	pMainLayout->addStretch();

	return pMainWidget;
}

void QOpenDatabaseView::dispatchClicked()
{
	if(sender() == m_pSQLiteButton){
		emit clicked(DatabaseModel::SQLiteType);
	}
	if(sender() == m_pMySQLConnectButton){
		emit clicked(DatabaseModel::MySQLType);
	}
	if(sender() == m_pPostgreSQLConnectButton){
		emit clicked(DatabaseModel::PostgreSQLType);
	}
}

void QOpenDatabaseView::onHistoryTreeWidgetDoubleClicked(QTreeWidgetItem *item, int column)
{
	QString szPath;
	szPath = item->toolTip(column);
	emit openHistorySQLiteDatabase(szPath);
}
