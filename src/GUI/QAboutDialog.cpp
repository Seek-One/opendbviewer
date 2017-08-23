/*
 * QAboutDialog.cpp
 *
 *  Created on: 11 juil. 2012
 *      Author: ebeuque
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "QAboutDialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIcon>
#include <QToolButton>
#include <QPushButton>

#include <QFile>


QAboutDialog::QAboutDialog(QWidget * parent)
{
	QLayout* pMainLayout = new QVBoxLayout();
	this->setLayout(pMainLayout);

	QString szTmp;
	QLabel* pLabel;

	QFile fileExits(":/logos/logo-about.jpg");
	if(fileExits.exists()){
		szTmp = "tri";
	}

	QImage image(":/logos/logo-about.jpg");

	// Add the logo image
	pLabel = new QLabel(this);
	pLabel->setPixmap(QPixmap::fromImage(image));
	pMainLayout->addWidget(pLabel);

	// Global information
	szTmp = "<b>%1 %2</b><br>";
	szTmp += "<br>";
	szTmp += tr("This program is distributed under the terms of the <a href=\"https://www.gnu.org/licenses/gpl-3.0.fr.html\">GNU GPL version 3</a>.") + "<br><br>";
	szTmp += tr("%1 is a product originally created by Eric Beuque in Python.") + "<br/>";
	szTmp += tr("It has been rewritten in C++ and sponsored by %3: <a href=\"http://%4\">%4</a>");
	szTmp += "<br><br>";
	szTmp += tr("This product uses open source softwares:") + "<br/>";
	szTmp = szTmp.arg(QApplication::applicationName());
	szTmp = szTmp.arg(QApplication::applicationVersion());
	szTmp = szTmp.arg(QApplication::organizationName());
	szTmp = szTmp.arg(QApplication::organizationDomain());
	pLabel = new QLabel(szTmp, this);
	pMainLayout->addWidget(pLabel);

	// Grid for each library licence
	QFrame* pFrame = new QFrame(this);
	QLayout* pLayout = new QHBoxLayout();
	pFrame->setLayout(pLayout);
	pMainLayout->addWidget(pFrame);

	QAbstractButton* pButton;
	pButton = addLibraryButton(pLayout, pFrame, ":/logos/qt-logo.png", "Qt");
	connect(pButton, SIGNAL(clicked()), this, SLOT(menuAboutQtClicked()));

	// Separator
    QFrame* pLine = new QFrame(this);
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    pMainLayout->addWidget(pLine);

	// Button box
	QDialogButtonBox* pButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);
	pMainLayout->addWidget(pButtonBox);
	pButtonBox->button(QDialogButtonBox::Ok)->setFocus();
    connect(pButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(pButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

	szTmp = tr("About %1");
	szTmp = szTmp.arg(QApplication::applicationName());
	setWindowTitle(szTmp);
}

QAboutDialog::~QAboutDialog()
{

}

QAbstractButton* QAboutDialog::addLibraryButton(QLayout* pLayout, QWidget* pParent, const QString& szIconFile, const QString& szName)
{
	QToolButton* pButton;

	pButton = new QToolButton(pParent);
	pButton->setIcon(QIcon(szIconFile));
	pButton->setIconSize(QSize(48, 48));
	pButton->setText(szName);
	pButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	pButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	pLayout->addWidget(pButton);

	return pButton;
}

void QAboutDialog::menuAboutQtClicked()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}
