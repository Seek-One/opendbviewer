/*
 * QProgressBarDialog.cpp
 *
 *  Created on: 23 févr. 2021
 *      Author: gsegard
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QVBoxLayout>
#include <QProgressBar>
#include <QDialogButtonBox>

#include "GUI/QProgressBarDialog.h"
#include "GUIController/QWindowMainController.h"

QProgressBarDialog::QProgressBarDialog(QWidget* parent, QString szTitle)
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
		connect(m_pCancelButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
	}
}

QProgressBarDialog::~QProgressBarDialog()
{
}

QDialogButtonBox* QProgressBarDialog::getCancelButton() const
{
	return m_pCancelButtonBox;
}

bool QProgressBarDialog::isCancel() const
{
	return m_bCancel;
}

void QProgressBarDialog::setCancel()
{
	m_bCancel = true;
}

void QProgressBarDialog::setData(int iData)
{
	m_pProgressBar->setValue(iData);
}

void QProgressBarDialog::setMaximumData(int maxData)
{
	m_pProgressBar->setMaximum(maxData);
}
