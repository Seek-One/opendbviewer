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
#include <QPushButton>
#include <QTabWidget>
#include <QValidator>
#include <QVBoxLayout>

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

	m_pFavouriteTabTreeWidget = new QTreeWidget(m_pConnectionTypeTabWidget);

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

	HideUnusedObjects(true);

	connect(m_pFavouriteTabTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onFavouriteTreeWidgetDoubleClicked(QTreeWidgetItem *, int)));
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

QTreeWidget* QOpenDatabaseView::getFavouriteTreeWidget() const
{
	return m_pFavouriteTabTreeWidget;
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

QWidget* QOpenDatabaseView::makeFavouriteTab(QWidget* pParent)
{
	HideUnusedObjects(true);
	QWidget* pMainWidget = new QWidget(pParent);

	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QString szFavouriteHeaderName = "Favourite Database";
	m_pFavouriteTabTreeWidget->setHeaderLabel(szFavouriteHeaderName);
	m_pFavouriteTabTreeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
	m_pFavouriteTabTreeWidget->setVisible(true);

	pMainLayout->addWidget(m_pFavouriteTabTreeWidget);

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeExplorerTab(QWidget* pParent)
{
	HideUnusedObjects(true);
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	pMainLayout->addWidget(m_pFileExplorerWidget);
	m_pFileExplorerWidget->setVisible(true);
	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeSQLiteTab(QWidget* pParent)
{
	HideUnusedObjects(true);
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QVBoxLayout* pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pSecondLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	QBoxLayout* pTmpLayout;

	m_pSQLiteFilePathField->setVisible(true);
	m_pSQLiteFileSelectionButton->setVisible(true);
	m_pDropAreaWidget->setVisible(true);
	m_pSQLiteButton->setVisible(true);

	// File field
	{
		pTmpLayout = new QHBoxLayout();

		pTmpLayout->addWidget(m_pSQLiteFilePathField);

		pTmpLayout->addWidget(m_pSQLiteFileSelectionButton);

		pFormLayout->addRow(tr("File:"), pTmpLayout);
	}

	{
		pTmpLayout = new QVBoxLayout();

		pTmpLayout->setContentsMargins(10,0,10,10);

		pTmpLayout->addWidget(m_pSQLiteButton, 1, Qt::AlignLeft);

		pTmpLayout->addWidget(m_pDropAreaWidget);

	}
	pSecondLayout->addLayout(pTmpLayout, 3);

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeMySQLTab(QWidget* pParent)
{
	HideUnusedObjects(true);
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QVBoxLayout* pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pSecondLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);


	m_pMySQLHostField->setVisible(true);
	m_pMySQLPortField->setVisible(true);
	m_pMySQLUsernameField->setVisible(true);
	m_pMySQLPasswordField->setVisible(true);
	m_pMySQLDatabaseField->setVisible(true);
	m_pMySQLConnectButton->setVisible(true);

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

	pSecondLayout->addWidget(m_pMySQLConnectButton, 1, Qt::AlignRight);

	pSecondLayout->addStretch();
	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makePostgreSQLTab(QWidget* pParent)
{
	HideUnusedObjects(true);
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QVBoxLayout* pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pSecondLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);


	m_pPSQLHostField->setVisible(true);
	m_pPSQLPortField->setVisible(true);
	m_pPSQLUsernameField->setVisible(true);
	m_pPSQLPasswordField->setVisible(true);
	m_pPSQLDatabaseField->setVisible(true);
	m_pPostgreSQLConnectButton->setVisible(true);

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

	pSecondLayout->addWidget(m_pPostgreSQLConnectButton, 1, Qt::AlignRight);

	pSecondLayout->addStretch();
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

void QOpenDatabaseView::onFavouriteTreeWidgetDoubleClicked(QTreeWidgetItem *item, int column)
{
	QString szPath;
	szPath = item->text(column);
	emit openFavouriteSQLiteDatabase(szPath);
}

void QOpenDatabaseView::HideUnusedObjects(bool i) {
		m_pFileExplorerWidget->setHidden(i);

		// SQLite
		m_pSQLiteFilePathField->setHidden(i);
		m_pSQLiteFileSelectionButton->setHidden(i);
		m_pDropAreaWidget->setHidden(i);

		// MySQL
		m_pMySQLHostField->setHidden(i);
		m_pMySQLPortField->setHidden(i);
		m_pMySQLUsernameField->setHidden(i);
		m_pMySQLPasswordField->setHidden(i);
		m_pMySQLDatabaseField->setHidden(i);

		// PostgreSQL
		m_pPSQLHostField->setHidden(i);
		m_pPSQLPortField->setHidden(i);
		m_pPSQLUsernameField->setHidden(i);
		m_pPSQLPasswordField->setHidden(i);
		m_pPSQLDatabaseField->setHidden(i);

		// Validation button
		m_pSQLiteButton->setHidden(i); //SQLite
		m_pMySQLConnectButton->setHidden(i);
		m_pPostgreSQLConnectButton->setHidden(i);

		m_pFavouriteTabTreeWidget->setHidden(i); //setVisible when click on the button 'Favourites Databases' + connect(...)
}
