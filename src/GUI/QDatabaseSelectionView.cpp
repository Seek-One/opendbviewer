/*
 * QDatabaseSelectionView.cpp
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#include "GUI/QDatabaseSelectionView.h"
#include "QWindowMain.h"


QDatabaseSelectionView::QDatabaseSelectionView(QWidget* parent)
		: QWidget(parent)
{
	//Creation of the dialog box for opening the files
	QDialog *pNewConnectionFileSelection = new QDialog(this);
	pNewConnectionFileSelection->setMinimumSize(450, 100);
	QVBoxLayout *pMainFileSelectionLayout = new QVBoxLayout(pNewConnectionFileSelection);
	pNewConnectionFileSelection->setLayout(pMainFileSelectionLayout);

	QTabWidget *pFileSelectionTab = new QTabWidget();
	pMainFileSelectionLayout->addWidget(pFileSelectionTab);

	QWidget *pSQLiteTab = new QWidget;
	pSQLiteTab = makeTab();
	pFileSelectionTab->addTab(pSQLiteTab, "Sqlite");
	QWidget *pAdoDBTab = new QWidget;
	pFileSelectionTab->addTab(pAdoDBTab, "AdoDB");

	//Bottom buttons
	QHBoxLayout *pBottomButtonsLayout = new QHBoxLayout;
	pMainFileSelectionLayout->addLayout(pBottomButtonsLayout);

	pBottomButtonsLayout->addSpacing(70);

	m_pCancelButton = new QPushButton("Cancel");
	pBottomButtonsLayout->addWidget(m_pCancelButton);

	m_pOKButton = new QPushButton("OK");
	pBottomButtonsLayout->addWidget(m_pOKButton);

	pNewConnectionFileSelection->show();
}



QDatabaseSelectionView::~QDatabaseSelectionView()
{
}


QWidget* QDatabaseSelectionView::makeTab()
{
	QWidget* pTab = new QWidget;

	QVBoxLayout *tabLayout = new QVBoxLayout;
	pTab->setLayout(tabLayout);

	QLabel *pConnectionLabel = new QLabel("");
	pConnectionLabel->setTextFormat(Qt::RichText);
	pConnectionLabel->setText("<b>Connection</b>");
	tabLayout->addWidget(pConnectionLabel);

	QToolBar *pToolBar = new QToolBar;
	tabLayout->addWidget(pToolBar);

	QLabel *pFileLabel = new QLabel("File:");
	pToolBar->addWidget(pFileLabel);

	m_pFilePathField = new QLineEdit();
	m_pFilePathField->setReadOnly(true);
	pToolBar->addWidget(m_pFilePathField);

	m_pFileSelectionButton = new QPushButton(tr("select a file"), this);
	pToolBar->addWidget(m_pFileSelectionButton);

	return pTab;
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
