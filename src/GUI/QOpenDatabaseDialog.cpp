/*
 * QOpenDatabaseDialog.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QValidator>

#include "QOpenDatabaseDialog.h"

QOpenDatabaseDialog::QOpenDatabaseDialog(QWidget* parent)
		: QDialog(parent)
{
	QWidget *pWidget;
	QHBoxLayout *pTmpHLayout;

	// Creation of the dialog box for opening the files
	setMinimumSize(450, 100);

	QVBoxLayout *pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);

	m_pConnectionTypeTabWidget = new QTabWidget(this);
	pMainLayout->addWidget(m_pConnectionTypeTabWidget);

	// Adding an SQLite tab to the selection window
	pWidget = makeSQLiteTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("SQLite"));

	// Adding a MySQL tab to the selection window
	pWidget = makeMySQLTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("MySQL"));

	// Adding a MySQL tab to the selection window
	pWidget = makePostgreSQLTab(m_pConnectionTypeTabWidget);
	m_pConnectionTypeTabWidget->addTab(pWidget, tr("PostgreSQL"));

	// Bottom buttons
	pTmpHLayout = new QHBoxLayout();
	pMainLayout->addLayout(pTmpHLayout);
	pTmpHLayout->addStretch();
	m_pCancelButton = new QPushButton(tr("Cancel"));
	pTmpHLayout->addWidget(m_pCancelButton);
	m_pOKButton = new QPushButton(tr("OK"));
	pTmpHLayout->addWidget(m_pOKButton);
}



QOpenDatabaseDialog::~QOpenDatabaseDialog()
{

}

QTabWidget* QOpenDatabaseDialog::getConnectionTypeTabWidget() const
{
	return m_pConnectionTypeTabWidget;
}

QPushButton* QOpenDatabaseDialog::getSQLiteFileSelectionButton() const
{
	return m_pSQLiteFileSelectionButton;
}

QLineEdit* QOpenDatabaseDialog::getSQLiteFilePathField() const

{
	return m_pSQLiteFilePathField;
}

QDropAreaWidget* QOpenDatabaseDialog::getDropAreaWidget() const
{
	return m_pDropAreaWidget;
}

QLineEdit* QOpenDatabaseDialog::getMySQLHostField() const
{
	return m_pMySQLHostField;
}

QLineEdit* QOpenDatabaseDialog::getMySQLPortField() const
{
	return m_pMySQLPortField;
}

QLineEdit* QOpenDatabaseDialog::getMySQLUsernameField() const
{
	return m_pMySQLUsernameField;
}

QLineEdit* QOpenDatabaseDialog::getMySQLPasswordField() const
{
	return m_pMySQLPasswordField;
}

QLineEdit* QOpenDatabaseDialog::getMySQLDatabaseField() const
{
	return m_pMySQLDatabaseField;
}

QLineEdit* QOpenDatabaseDialog::getPSQLHostField() const
{
	return m_pPSQLHostField;
}

QLineEdit* QOpenDatabaseDialog::getPSQLPortField() const
{
	return m_pPSQLPortField;
}

QLineEdit* QOpenDatabaseDialog::getPSQLUsernameField() const
{
	return m_pPSQLUsernameField;
}

QLineEdit* QOpenDatabaseDialog::getPSQLPasswordField() const
{
	return m_pPSQLPasswordField;
}

QLineEdit* QOpenDatabaseDialog::getPSQLDatabaseField() const
{
	return m_pPSQLDatabaseField;
}

QPushButton* QOpenDatabaseDialog::getCancelButton() const
{
	return m_pCancelButton;
}

QPushButton* QOpenDatabaseDialog::getOKButton() const
{
	return m_pOKButton;
}

QWidget* QOpenDatabaseDialog::makeSQLiteTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	QBoxLayout* pTmpLayout;

	// File field
	{
		pTmpLayout = new QHBoxLayout();

		m_pSQLiteFilePathField = new QLineEdit();
		m_pSQLiteFilePathField->setReadOnly(true);
		pTmpLayout->addWidget(m_pSQLiteFilePathField);

		m_pSQLiteFileSelectionButton = new QPushButton(tr("Browse"), this);
		pTmpLayout->addWidget(m_pSQLiteFileSelectionButton);

		pFormLayout->addRow(tr("File:"), pTmpLayout);
	}

	{
		pTmpLayout = new QHBoxLayout();
		pTmpLayout->setContentsMargins(10,0,10,10);

		QString szDropAreaName = tr("Drag and drop your files here");
		m_pDropAreaWidget = new QDropAreaWidget(szDropAreaName,this);
		pTmpLayout->addWidget(m_pDropAreaWidget);
	}
	pMainLayout->addLayout(pTmpLayout);
	return pMainWidget;
}

QWidget* QOpenDatabaseDialog::makeMySQLTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

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

	return pMainWidget;
}

QWidget* QOpenDatabaseDialog::makePostgreSQLTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

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

	return pMainWidget;
}