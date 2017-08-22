/*
 * QDatabaseSelectionView.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "GUI/QDatabaseSelectionView.h"
#include "QWindowMain.h"

#include <QFormLayout>
#include <QValidator>

QDatabaseSelectionView::QDatabaseSelectionView(QWidget* parent)
		: QDialog(parent)
{
	//Creation of the dialog box for opening the files
	setMinimumSize(450, 100);

	QVBoxLayout *pMainFileSelectionLayout = new QVBoxLayout();
	setLayout(pMainFileSelectionLayout);

	m_pFileSelectionTabWidget = new QTabWidget(this);
	pMainFileSelectionLayout->addWidget(m_pFileSelectionTabWidget);

	//Adding an SQlite tab to the selection window
	QWidget *pSQLiteTab = makeSqliteTab();
	m_pFileSelectionTabWidget->addTab(pSQLiteTab, "Sqlite");

	//Bottom buttons
	QHBoxLayout *pBottomButtonsLayout = new QHBoxLayout;
	pMainFileSelectionLayout->addLayout(pBottomButtonsLayout);
	pBottomButtonsLayout->addSpacing(70);
	m_pCancelButton = new QPushButton("Cancel");
	pBottomButtonsLayout->addWidget(m_pCancelButton);
	m_pOKButton = new QPushButton("OK");
	pBottomButtonsLayout->addWidget(m_pOKButton);

	//Adding a mysql tab to the selection window
	QWidget *pMySqlTab = makeMySqlTab();
	m_pFileSelectionTabWidget->addTab(pMySqlTab, "MySQL");
}



QDatabaseSelectionView::~QDatabaseSelectionView()
{

}

QWidget* QDatabaseSelectionView::makeSqliteTab()
{
	QWidget* pSqliteTab = new QWidget;

	QVBoxLayout *pTabLayout = new QVBoxLayout;
	pSqliteTab->setLayout(pTabLayout);

	QLabel *pConnectionLabel = new QLabel("");
	pConnectionLabel->setTextFormat(Qt::RichText);
	pConnectionLabel->setText("<b>Connection</b>");
	pTabLayout->addWidget(pConnectionLabel);

	QToolBar *pToolBar = new QToolBar;
	pTabLayout->addWidget(pToolBar);

	QLabel *pFileLabel = new QLabel("File:");
	pToolBar->addWidget(pFileLabel);

	m_pFilePathField = new QLineEdit();
	m_pFilePathField->setReadOnly(true);
	pToolBar->addWidget(m_pFilePathField);

	m_pFileSelectionButton = new QPushButton(tr("Browse"), this);
	pToolBar->addWidget(m_pFileSelectionButton);

	pTabLayout->addSpacing(70);

	return pSqliteTab;
}

QWidget* QDatabaseSelectionView::makeMySqlTab()
{
	QWidget* pMySqlTab = new QWidget;
	QFormLayout* pMySqlTabLayout = new QFormLayout();
	pMySqlTabLayout->setVerticalSpacing(2);
	pMySqlTab->setLayout(pMySqlTabLayout);

	//Adding lines to the form layout
	m_pHostField = new QLineEdit(this);
	pMySqlTabLayout->addRow(tr("Host"), m_pHostField);
	m_pPortField = new QLineEdit(this);
	QValidator *pPortValidator = new QIntValidator(0, 65535, this);
	m_pPortField->setValidator(pPortValidator);
	pMySqlTabLayout->addRow(tr("Port:"), m_pPortField);
	m_pUsernameField = new QLineEdit(this);
	pMySqlTabLayout->addRow(tr("Username"), m_pUsernameField);
	m_pPasswordField = new QLineEdit(this);
	m_pPasswordField->setEchoMode(QLineEdit::Password);
	pMySqlTabLayout->addRow(tr("Password"), m_pPasswordField);
	m_pDatabaseField = new QLineEdit(this);
	pMySqlTabLayout->addRow(tr("Database"), m_pDatabaseField);

	return pMySqlTab;
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

QTabWidget* QDatabaseSelectionView::getFileSelectionTabWidget() const
{
	return m_pFileSelectionTabWidget;
}
