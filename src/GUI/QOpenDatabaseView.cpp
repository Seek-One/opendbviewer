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
#include <QFontMetrics>


#include "QOpenDatabaseView.h"

QOpenDatabaseView::QOpenDatabaseView(QWidget* parent)
		: QWidget(parent)
{
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);
	pMainLayout->setContentsMargins(0,0,0,0);

	m_pStackedMenuConnWidget = new QStackedWidget();
	pMainLayout->addWidget(m_pStackedMenuConnWidget);

	m_pNewConnWidget = makeNewConnMenu(this);
    m_pSQLiteWidget = makeSQLiteTab(this);
    m_pMySQLWidget = makeMySQLTab(this);
    m_pPSQLWidget = makePostgreSQLTab(this);

    m_pStackedMenuConnWidget->addWidget(m_pNewConnWidget);
    m_pStackedMenuConnWidget->addWidget(m_pSQLiteWidget);
    m_pStackedMenuConnWidget->addWidget(m_pMySQLWidget);
    m_pStackedMenuConnWidget->addWidget(m_pPSQLWidget);
}



QOpenDatabaseView::~QOpenDatabaseView()
{

}

QStackedWidget* QOpenDatabaseView::getStackedMenuConnWidget() const
{
	return m_pStackedMenuConnWidget;
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

QWidget* QOpenDatabaseView::makeNewConnMenu(QWidget* pParent)
{
	m_pSQLiteSelection = new QPushButton(tr("Open SQLite Database"));
	m_pMySQLSelection = new QPushButton(tr("Open MySQL Database"));
	m_pPSQLSelection = new QPushButton(tr("Open PostgreSQL Database"));

	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

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
	m_pSQLiteFilePathField = new QLineEdit();
	m_pSQLiteFileSelectionButton = new QPushButton(tr("Browse"), pParent);
	m_pSQLiteButton = new QPushButton(tr("OK"));
	connect(m_pSQLiteButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));
	QString szDropAreaName = tr("Drag and drop \n your files here");
	m_pDropAreaWidget = new QDropAreaWidget(szDropAreaName, pParent);

	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

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
	m_pMySQLHostField = new QLineEdit();
	m_pMySQLPortField = new QLineEdit();
	m_pMySQLUsernameField = new QLineEdit();
	m_pMySQLPasswordField = new QLineEdit();
	m_pMySQLDatabaseField = new QLineEdit();
	m_pMySQLConnectButton = new QPushButton(tr("Connect"));
	connect(m_pMySQLConnectButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));

	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	int iSpace = 10;
	pMainLayout->setContentsMargins(iSpace,0,iSpace,0);

	QGroupBox *pGroupBox = new QGroupBox(tr("MySQL Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);
	iSpace = 20;
	pGroupBox->setContentsMargins(0,iSpace,0,0);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);
	pFormLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	pFormLayout->setContentsMargins(0,0,0,0);

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


	pMainLayout->addWidget(m_pMySQLConnectButton, 1, Qt::AlignRight);
	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makePostgreSQLTab(QWidget* pParent)
{
	//PostgreSQL Menu
	m_pPSQLHostField = new QLineEdit();
	m_pPSQLPortField = new QLineEdit();
	m_pPSQLUsernameField = new QLineEdit();
	m_pPSQLPasswordField = new QLineEdit();
	m_pPSQLDatabaseField = new QLineEdit();
	m_pPostgreSQLConnectButton = new QPushButton(tr("Connect"));
	connect(m_pPostgreSQLConnectButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));

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

void QOpenDatabaseView::openNewConnMenuTab()
{
	m_pStackedMenuConnWidget->setCurrentWidget(m_pNewConnWidget);
}

void QOpenDatabaseView::openSQLiteTab()
{
	m_pStackedMenuConnWidget->setCurrentWidget(m_pSQLiteWidget);
}

void QOpenDatabaseView::openMySQLTab()
{
	m_pStackedMenuConnWidget->setCurrentWidget(m_pMySQLWidget);
}

void QOpenDatabaseView::openPostgreSQLTab()
{
	m_pStackedMenuConnWidget->setCurrentWidget(m_pPSQLWidget);
}
