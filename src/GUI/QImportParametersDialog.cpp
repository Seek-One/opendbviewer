/*
 * QImportParametersDialog.cpp
 *
 *  Created on: 23 mars 2021
 *      Author: gsegard
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QApplication>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>

#include "GUI/QImportParametersDialog.h"

class QComboBoxEventFilter : public QObject
{
private:
	QComboBox* m_pComboBox;

public:
	QComboBoxEventFilter(QComboBox* pComboBox)
	{
		m_pComboBox = pComboBox;
	}

	virtual ~QComboBoxEventFilter()
	{

	}

protected:
	bool eventFilter(QObject *obj, QEvent *event)
	{
		if(event->type() == QEvent::KeyPress){
			// Set index for a personalised item
			if(m_pComboBox->currentIndex() != -1){
				m_pComboBox->setCurrentIndex(-1);
			}
		}
		return false;
	}

};

QImportParametersDialog::QImportParametersDialog(QWidget* parent)
{
	setWindowTitle(tr("Import a CSV file"));

	QVBoxLayout* pMainLayout = new QVBoxLayout(this);

	setMinimumSize(370, 250);

	QBoxLayout* pTmpBoxLayout;

	// Define the form fields
	{
		QGroupBox* pFieldGroupBox = new QGroupBox(tr("Informations"), this);
		pMainLayout->addWidget(pFieldGroupBox);

		QFormLayout *pFieldFormLayout = new QFormLayout();
		pFieldGroupBox->setLayout(pFieldFormLayout);

		// Field separator
		m_pComboBoxFieldSeparator = new QComboBox();
		m_pComboBoxFieldSeparator->addItem(",", FieldSeparatorType_Comma);
		m_pComboBoxFieldSeparator->addItem(";", FieldSeparatorType_Semicolon);
		m_pComboBoxFieldSeparator->addItem(":", FieldSeparatorType_Colon);
		m_pComboBoxFieldSeparator->addItem(tr("{Space}"), FieldSeparatorType_Space);
		m_pComboBoxFieldSeparator->addItem(tr("{Tab key}"), FieldSeparatorType_TabKey);
		m_pComboBoxFieldSeparator->setEditable(true);
		m_pComboBoxFieldSeparator->setCurrentIndex(FieldSeparatorType_Comma);
		pFieldFormLayout->addRow(tr("Data separator:"), m_pComboBoxFieldSeparator);

		m_pComboBoxFieldSeparator->installEventFilter(new QComboBoxEventFilter(m_pComboBoxFieldSeparator));

		// String separator
		m_pComboBoxStringSeparator = new QComboBox();
		m_pComboBoxStringSeparator->addItem("\"", StringSeparatorType_DoubleQuote);
		m_pComboBoxStringSeparator->addItem("\'", StringSeparatorType_SimpleQuote);
		m_pComboBoxStringSeparator->setEditable(true);
		m_pComboBoxStringSeparator->setCurrentIndex(StringSeparatorType_DoubleQuote);
		pFieldFormLayout->addRow(tr("String separator:"), m_pComboBoxStringSeparator);

		m_pComboBoxStringSeparator->installEventFilter(new QComboBoxEventFilter(m_pComboBoxStringSeparator));
	}

	// Define the file path section
	{
		QGroupBox* pFilePathGroupBox = new QGroupBox(tr("File path"), this);
		pMainLayout->addWidget(pFilePathGroupBox);

		pTmpBoxLayout = new QHBoxLayout();
		pFilePathGroupBox->setLayout(pTmpBoxLayout);

		m_pFileExplorerLineEdit = new QLineEdit(pFilePathGroupBox);
		pTmpBoxLayout->addWidget(m_pFileExplorerLineEdit);

		QPushButton* pButtonExplore = new QPushButton(tr("Explore"));
		pTmpBoxLayout->addWidget(pButtonExplore);
		connect(pButtonExplore, SIGNAL(clicked()), this, SLOT(explore()));
	}

	// Define validation buttons
	{
		QHBoxLayout* pButtonsLayout = new QHBoxLayout();
		pMainLayout->addLayout(pButtonsLayout);

		QDialogButtonBox* pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		pButtonsLayout->addWidget(pButtonBox);

		connect(pButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
		connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
	}
}

QImportParametersDialog::~QImportParametersDialog() {

}

void QImportParametersDialog::explore()
{
	QString szFilePath = QFileDialog::getOpenFileName(this, tr("Import"), m_szCurrentPath, tr("CSV files (*.csv)"));
	QFileInfo info(szFilePath);
	m_szCurrentPath = info.absoluteDir().path();
	if(!szFilePath.isEmpty()){
		m_pFileExplorerLineEdit->setText(szFilePath);
	}
}

QString QImportParametersDialog::getFilePath() const
{
	return m_pFileExplorerLineEdit->text();
}

QString QImportParametersDialog::getTextFieldSeparator() const
{
	QString szCurrText = m_pComboBoxFieldSeparator->currentText();

	switch(m_pComboBoxFieldSeparator->currentIndex()){
		case FieldSeparatorType_Space:
			return " ";
		case FieldSeparatorType_TabKey:
			return "\t";
		default:
			break;
	}
	return szCurrText;
}

QString QImportParametersDialog::getTextStringSeparator() const
{
	QString szCurrText = m_pComboBoxStringSeparator->currentText();

	switch(m_pComboBoxStringSeparator->currentIndex()){
		case StringSeparatorType_DoubleQuote:
			return "\"";
		case StringSeparatorType_SimpleQuote:
			return "\'";
		default:
			break;
	}
	return szCurrText;
}
