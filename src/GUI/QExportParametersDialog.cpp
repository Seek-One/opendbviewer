/*
 * QExportParametersDialog.cpp
 *
 *  Created on: 4 feb. 2021
 *      Author: gsegard
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QApplication>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>

#include "GUI/QExportParametersDialog.h"

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

QExportParametersDialog::QExportParametersDialog(QWidget* parent)
{
	setWindowTitle(tr("Export to csv file"));

	QVBoxLayout* pMainLayout = new QVBoxLayout(parent);
	setLayout(pMainLayout);

	setMinimumSize(370, 300);

	QBoxLayout* pTmpBoxLayout;

	// Define the form fields
	{
		QGroupBox* pFieldGroupBox = new QGroupBox(tr("Options"), this);
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

		// Line break separator
		m_pComboBoxLineBreakSeparator = new QComboBox();
		m_pComboBoxLineBreakSeparator->addItem("\\n", LineBreakSeparatorType_LF);
		m_pComboBoxLineBreakSeparator->addItem("\\r\\n", LineBreakSeparatorType_CRLF);
		pFieldFormLayout->addRow(tr("Line break separator:"), m_pComboBoxLineBreakSeparator);

		// Header check box
		m_pCheckBoxHeader = new QCheckBox();
		m_pCheckBoxHeader->setChecked(true);
		m_pCheckBoxHeader->setMaximumWidth(20);
		pFieldFormLayout->addRow(tr("Add column title:"), m_pCheckBoxHeader);
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


QExportParametersDialog::~QExportParametersDialog()
{
}

void QExportParametersDialog::explore()
{
	QString szFilePath = QFileDialog::getSaveFileName(this, tr("Export"), m_szCurrentPath, tr("CSV files (*.csv)"));
	QFileInfo info(szFilePath);
	m_szCurrentPath = info.absoluteDir().path();
	if(!szFilePath.isEmpty()){
		m_pFileExplorerLineEdit->setText(szFilePath);
	}
}

QString QExportParametersDialog::getFilePath() const
{
	return m_pFileExplorerLineEdit->text();
}

QString QExportParametersDialog::getTextFieldSeparator() const
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

QString QExportParametersDialog::getTextStringSeparator() const
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

QString QExportParametersDialog::getTextLineBreakSeparator() const
{
	if (m_pComboBoxLineBreakSeparator->currentIndex() == LineBreakSeparatorType_CRLF){
		return "\r\n";
	}
	return "\n";
}

bool QExportParametersDialog::isIncludesHeaders()
{
	return m_pCheckBoxHeader->isChecked();
}
