/*
 * QExportParametersProgressBarDialog.cpp
 *
 *  Created on: 23 févr. 2021
 *      Author: gsegard
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "GUI/QExportParametersProgressBarDialog.h"
#include "GUIController/QWindowMainController.h"

#include <QVBoxLayout>
#include <QProgressBar>
#include <QDialogButtonBox>

#include <unistd.h>

QExportParametersProgressBarDialog::QExportParametersProgressBarDialog(QString szTitle, QWidget* parent)
{
	m_pProgressBar = NULL;
	m_bCancel = false;
	setWindowTitle(szTitle);

	QVBoxLayout* pMainLayout = new QVBoxLayout(this);

	// Define the progress bar
	{
		m_pProgressBar = new QProgressBar();
		m_pProgressBar->setFormat("%p% (%v/%m)");
		pMainLayout->addWidget(m_pProgressBar);
	}

	// Define the cancel button
	{
		m_pCancelButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
		pMainLayout->addWidget(m_pCancelButtonBox);
	}
}

QExportParametersProgressBarDialog::~QExportParametersProgressBarDialog()
{
}

QDialogButtonBox* QExportParametersProgressBarDialog::getCancelButton() const
{
	return m_pCancelButtonBox;
}

bool QExportParametersProgressBarDialog::isCancel() const
{
	return m_bCancel;
}

void QExportParametersProgressBarDialog::setCancel()
{
	m_bCancel = true;
}

void QExportParametersProgressBarDialog::setData(int iData)
{
	m_pProgressBar->setValue(iData);
}

void QExportParametersProgressBarDialog::setMaximumData(int maxData)
{
	m_pProgressBar->setMaximum(maxData);
}
