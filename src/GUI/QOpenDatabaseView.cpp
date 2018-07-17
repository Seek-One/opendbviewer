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
	QWidget *pWidget;

	QHBoxLayout *pMainLayout = new QHBoxLayout();
	setLayout(pMainLayout);

	QVBoxLayout *pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	m_pConnectionTypeTabWidget = new QTabWidget(this);
	pSecondLayout->addWidget(m_pConnectionTypeTabWidget);

	//Adding Explorer tab to the selection window
	pWidget = makeFavouriteTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("Favourite Database"));

	//Adding Explorer tab to the selection window
	pWidget = makeExplorerTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("File Explorer"));

	// Adding an SQLite tab to the selection window
	pWidget = makeSQLiteTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("SQLite"));

	// Adding a MySQL tab to the selection window
	pWidget = makeMySQLTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("MySQL"));

	// Adding a MySQL tab to the selection window
	pWidget = makePostgreSQLTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("PostgreSQL"));

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

QPushButton* QOpenDatabaseView::getOKButton() const
{
	return m_pOKButton;
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
	QWidget* pMainWidget = new QWidget(pParent);

	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	m_pFavouriteTabTreeWidget = new QTreeWidget(this);
	QString szFavouriteHeaderName = "Favourite Database";
	m_pFavouriteTabTreeWidget->setHeaderLabel(szFavouriteHeaderName);
	m_pFavouriteTabTreeWidget->header()->setDefaultAlignment(Qt::AlignCenter);

	pMainLayout->addWidget(m_pFavouriteTabTreeWidget);

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeExplorerTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	m_pFileExplorerWidget = new QFileExplorerWidget();
	pMainLayout->addWidget(m_pFileExplorerWidget);

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeSQLiteTab(QWidget* pParent)
{
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

	// File field
	{
		pTmpLayout = new QHBoxLayout();

		m_pSQLiteFilePathField = new QLineEdit();
		pTmpLayout->addWidget(m_pSQLiteFilePathField);

		m_pSQLiteFileSelectionButton = new QPushButton(tr("Browse"), this);
		pTmpLayout->addWidget(m_pSQLiteFileSelectionButton);

		pFormLayout->addRow(tr("File:"), pTmpLayout);
	}

	{
		pTmpLayout = new QVBoxLayout();
		pTmpLayout->setContentsMargins(10,0,10,10);

		m_pOKButton = new QPushButton(tr("OK"));
		pTmpLayout->addWidget(m_pOKButton, 1, Qt::AlignLeft);

		QString szDropAreaName = tr("Drag and drop \n your files here");
		m_pDropAreaWidget = new QDropAreaWidget(szDropAreaName,this);
		pTmpLayout->addWidget(m_pDropAreaWidget);


	}
	pSecondLayout->addLayout(pTmpLayout, 3);

	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeMySQLTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QVBoxLayout* pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pSecondLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	// Host field
	{
		m_pMySQLHostField = new QLineEdit(this);
		pFormLayout->addRow(tr("Host:"), m_pMySQLHostField);
	}
	// Port field
	{
		m_pMySQLPortField = new QLineEdit(this);
		QValidator *pPortValidator = new QIntValidator(0, 65535, this);
		m_pMySQLPortField->setValidator(pPortValidator);
		pFormLayout->addRow(tr("Port:"), m_pMySQLPortField);
	}
	// Username field
	{
		m_pMySQLUsernameField = new QLineEdit(this);
		pFormLayout->addRow(tr("Username:"), m_pMySQLUsernameField);
	}
	// Password field
	{
		m_pMySQLPasswordField = new QLineEdit(this);
		m_pMySQLPasswordField->setEchoMode(QLineEdit::Password);
		pFormLayout->addRow(tr("Password:"), m_pMySQLPasswordField);
	}
	// Database field
	{
		m_pMySQLDatabaseField = new QLineEdit(this);
		pFormLayout->addRow(tr("Database:"), m_pMySQLDatabaseField);
	}

	m_pMySQLConnectButton = new QPushButton(tr("Connect"));
	pSecondLayout->addWidget(m_pMySQLConnectButton, 1, Qt::AlignRight);

	pSecondLayout->addStretch();
	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makePostgreSQLTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QVBoxLayout* pSecondLayout = new QVBoxLayout();
	pMainLayout->addLayout(pSecondLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pSecondLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	// Host field
	{
		m_pPSQLHostField = new QLineEdit(this);
		pFormLayout->addRow(tr("Host:"), m_pPSQLHostField);
	}
	// Port field
	{
		m_pPSQLPortField = new QLineEdit(this);
		QValidator *pPortValidator = new QIntValidator(0, 65535, this);
		m_pPSQLPortField->setValidator(pPortValidator);
		pFormLayout->addRow(tr("Port:"), m_pPSQLPortField);
	}
	// Username field
	{
		m_pPSQLUsernameField = new QLineEdit(this);
		pFormLayout->addRow(tr("Username:"), m_pPSQLUsernameField);
	}
	// Password field
	{
		m_pPSQLPasswordField = new QLineEdit(this);
		m_pPSQLPasswordField->setEchoMode(QLineEdit::Password);
		pFormLayout->addRow(tr("Password:"), m_pPSQLPasswordField);
	}
	// Database field
	{
		m_pPSQLDatabaseField = new QLineEdit(this);
		pFormLayout->addRow(tr("Database:"), m_pPSQLDatabaseField);
	}

	m_pPostgreSQLConnectButton = new QPushButton(tr("Connect"));
	pSecondLayout->addWidget(m_pPostgreSQLConnectButton, 1, Qt::AlignRight);

	pSecondLayout->addStretch();
	pMainLayout->addStretch();

	return pMainWidget;
}

void QOpenDatabaseView::onFavouriteTreeWidgetDoubleClicked(QTreeWidgetItem *item, int column)
{
	QString szPath;
	szPath = item->text(column);
	emit openFavouriteSQLiteDatabase(szPath);
}