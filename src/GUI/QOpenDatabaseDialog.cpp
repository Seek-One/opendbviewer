/*
 * QDatabaseSelectionView.cpp
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

#include "GUI/QDatabaseSelectionView.h"

QDatabaseSelectionView::QDatabaseSelectionView(QWidget* parent)
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

	// Bottom buttons
	pTmpHLayout = new QHBoxLayout();
	pMainLayout->addLayout(pTmpHLayout);
	pTmpHLayout->addStretch();
	m_pCancelButton = new QPushButton(tr("Cancel"));
	pTmpHLayout->addWidget(m_pCancelButton);
	m_pOKButton = new QPushButton(tr("OK"));
	pTmpHLayout->addWidget(m_pOKButton);
}



QDatabaseSelectionView::~QDatabaseSelectionView()
{

}

QWidget* QDatabaseSelectionView::makeSQLiteTab(QWidget* pParent)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QHBoxLayout* pMainLayout = new QHBoxLayout();
	pMainWidget->setLayout(pMainLayout);

	QGroupBox *pGroupBox = new QGroupBox(tr("Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

	QFormLayout* pFormLayout = new QFormLayout();
	pGroupBox->setLayout(pFormLayout);

	QBoxLayout* pTmpLayout;

	// File field
	{
		pTmpLayout = new QHBoxLayout();

		m_pFilePathField = new QLineEdit();
		m_pFilePathField->setReadOnly(true);
		pTmpLayout->addWidget(m_pFilePathField);

		m_pFileSelectionButton = new QPushButton(tr("Browse"), this);
		pTmpLayout->addWidget(m_pFileSelectionButton);

		pFormLayout->addRow(tr("File:"), pTmpLayout);
	}

	return pMainWidget;
}

QWidget* QDatabaseSelectionView::makeMySQLTab(QWidget* pParent)
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
		m_pHostField = new QLineEdit(this);
		pFormLayout->addRow(tr("Host:"), m_pHostField);
	}
	// Port field
	{
		m_pPortField = new QLineEdit(this);
		QValidator *pPortValidator = new QIntValidator(0, 65535, this);
		m_pPortField->setValidator(pPortValidator);
		pFormLayout->addRow(tr("Port:"), m_pPortField);
	}
	// Username field
	{
		m_pUsernameField = new QLineEdit(this);
		pFormLayout->addRow(tr("Username:"), m_pUsernameField);
	}
	// Password field
	{
		m_pPasswordField = new QLineEdit(this);
		m_pPasswordField->setEchoMode(QLineEdit::Password);
		pFormLayout->addRow(tr("Password:"), m_pPasswordField);
	}
	// Database field
	{
		m_pDatabaseField = new QLineEdit(this);
		pFormLayout->addRow(tr("Database:"), m_pDatabaseField);
	}

	return pMainWidget;
}

QTabWidget* QDatabaseSelectionView::getConnectionTypeTabWidget() const
{
	return m_pConnectionTypeTabWidget;
}

QPushButton* QDatabaseSelectionView::getFileSelectionButton() const
{
	return m_pFileSelectionButton;
}

QPushButton* QDatabaseSelectionView::getCancelButton() const
{
	return m_pCancelButton;
}

QPushButton* QDatabaseSelectionView::getOKButton() const
{
	return m_pOKButton;
}

QLineEdit* QDatabaseSelectionView::getFilePathField() const

{
	return m_pFilePathField;
}

QLineEdit* QDatabaseSelectionView::getHostField() const
{
	return m_pHostField;
}

QLineEdit* QDatabaseSelectionView::getPortField() const
{
	return m_pPortField;
}

QLineEdit* QDatabaseSelectionView::getUsernameField() const
{
	return m_pUsernameField;
}

QLineEdit* QDatabaseSelectionView::getPasswordField() const
{
	return m_pPasswordField;
}

QLineEdit* QDatabaseSelectionView::getDatabaseField() const
{
	return m_pDatabaseField;
}
