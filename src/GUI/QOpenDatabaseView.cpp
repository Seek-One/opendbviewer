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
#include <QString>


#include "QOpenDatabaseView.h"

QOpenDatabaseView::QOpenDatabaseView(QWidget* parent)
		: QWidget(parent)
{
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	setLayout(pMainLayout);
	int iSpace = 5;
	pMainLayout->setContentsMargins(iSpace,0,0,0);

	m_pComboBoxSelection = new QComboBox();
    m_pValidationButton = new QPushButton(tr("Connect"));
	m_pSQLiteWidget = makeSQLiteWidget();
	m_pMySQLWidget = makeMySQLWidget();
	m_pPSQLWidget = makePostgreSQLWidget();

	QLabel *title = new QLabel();
	title->setText(tr("Connection Database choice :"));
	title->setStyleSheet("font: bold;");
	pMainLayout->addWidget(title);

	m_pComboBoxSelection->addItem("SQLite");
	m_pComboBoxSelection->addItem("MySQL");
	m_pComboBoxSelection->addItem("PostgreSQL");

	pMainLayout->addWidget(m_pComboBoxSelection);

	QVBoxLayout *pSecondLayout = new QVBoxLayout();
	pSecondLayout->addWidget(m_pSQLiteWidget);
	pSecondLayout->addWidget(m_pMySQLWidget);
	pSecondLayout->addWidget(m_pPSQLWidget);
	pSecondLayout->addWidget(m_pValidationButton, 1, Qt::AlignRight);
	pSecondLayout->addStretch();

	pMainLayout->addLayout(pSecondLayout);
    pMainLayout->addStretch();

	connect(m_pValidationButton, SIGNAL(clicked()), this, SLOT(dispatchClicked()));

    openSQLiteMenu();
}



QOpenDatabaseView::~QOpenDatabaseView()
{
}

QPushButton* QOpenDatabaseView::getSQLiteFileSelectionButton() const
{
	return m_pSQLiteFileSelectionButton;
}

QLineEdit* QOpenDatabaseView::getSQLiteFilePathField() const

{
	return m_pSQLiteFilePathField;
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

QPushButton* QOpenDatabaseView::getValidationButton() const
{
	return m_pValidationButton;
}

QComboBox* QOpenDatabaseView::getComboBoxSelection() const
{
	return m_pComboBoxSelection;
}

QWidget* QOpenDatabaseView::makeSQLiteWidget()
{
	m_pSQLiteFilePathField = new QLineEdit();
	m_pSQLiteFileSelectionButton = new QPushButton(tr("..."));
	int iSize = 27;
	m_pSQLiteFileSelectionButton->setFixedSize(iSize,iSize);

	QWidget* pMainWidget = new QWidget(this);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	int iSpace = 5;
	pMainLayout->setContentsMargins(0,iSpace,0,0);

	QLabel * title = new QLabel();
	title->setText(tr("SQLite Connection:"));
	title->setStyleSheet("font: bold;");

	pMainLayout->addWidget(title);

	QHBoxLayout* pSecondLayout = new QHBoxLayout();
	pSecondLayout->setSpacing(0);
	pSecondLayout->addWidget(m_pSQLiteFilePathField);
	pSecondLayout->addWidget(m_pSQLiteFileSelectionButton);
	pMainLayout->addLayout(pSecondLayout);


	return pMainWidget;
}

QWidget* QOpenDatabaseView::makeMySQLWidget()
{
	m_pMySQLHostField = new QLineEdit();
	m_pMySQLPortField = new QLineEdit();
	m_pMySQLUsernameField = new QLineEdit();
	m_pMySQLPasswordField = new QLineEdit();
	m_pMySQLDatabaseField = new QLineEdit();

	QWidget* pMainWidget = new QWidget(this);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	int iSpace = 5;
	pMainLayout->setContentsMargins(0,iSpace,0,0);

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
	pMainLayout->addStretch();

	return pMainWidget;
}

QWidget* QOpenDatabaseView::makePostgreSQLWidget()
{
	//PostgreSQL Menu
	m_pPSQLHostField = new QLineEdit();
	m_pPSQLPortField = new QLineEdit();
	m_pPSQLUsernameField = new QLineEdit();
	m_pPSQLPasswordField = new QLineEdit();
	m_pPSQLDatabaseField = new QLineEdit();

	QWidget* pMainWidget = new QWidget(this);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	int iSpace = 5;
	pMainLayout->setContentsMargins(0,iSpace,0,0);


	QGroupBox *pGroupBox = new QGroupBox(tr("PostgreSQL Connection:"), pMainWidget);
	pMainLayout->addWidget(pGroupBox);

	iSpace = 10;
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
	pFormLayout->setContentsMargins(0,iSpace,0,0);

	pMainLayout->addStretch();

	return pMainWidget;
}

void QOpenDatabaseView::dispatchClicked()
{
	switch (m_pComboBoxSelection->currentIndex()) {
	case 0:
		emit clicked(DatabaseModel::SQLiteType);
		break;
	case 1:
		emit clicked(DatabaseModel::MySQLType);
		break;
	case 2:
		emit clicked(DatabaseModel::PostgreSQLType);
		break;
	}
}

void QOpenDatabaseView::openSQLiteMenu()
{
	hideMenus();
	m_pSQLiteWidget->setVisible(true);
}

void QOpenDatabaseView::openMySQLMenu()
{
	hideMenus();
	m_pMySQLWidget->setVisible(true);
}

void QOpenDatabaseView::openPostgreSQLMenu()
{
	hideMenus();
	m_pPSQLWidget->setVisible(true);
}

void QOpenDatabaseView::hideMenus()
{
    m_pSQLiteWidget->setHidden(true);
    m_pMySQLWidget->setHidden(true);
    m_pPSQLWidget->setHidden(true);
}
