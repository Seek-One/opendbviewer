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

QAboutDialog::QAboutDialog(QWidget * parent)
{
	QLayout* pMainLayout = new QVBoxLayout();
	this->setLayout(pMainLayout);

	QString szTmp;
	QLabel* pLabel;

	QImage image(":/logos/logo-about.jpg");

	// Add the logo image
	pLabel = new QLabel(this);
	pLabel->setPixmap(QPixmap::fromImage(image));
	pMainLayout->addWidget(pLabel);

	// Global information
	szTmp = "<b>%1 %2</b><br>";
	szTmp += "<br>";
	szTmp += tr("Copyright 2012-2016 - All rights reserved.") + "<br>";
#ifndef ORGANIZATION_ABOUT_HIDDEN
	szTmp += tr("%1 is a product %3: <a href=\"http://%4\">%4</a>") + "<br>";
#endif
	szTmp += "<br><br>";
	szTmp += tr("This product uses open source softwares allowing commercial use:") + "<br/>";
	szTmp = szTmp.arg(QApplication::applicationName());
	szTmp = szTmp.arg(QApplication::applicationVersion());
#ifndef ORGANIZATION_ABOUT_HIDDEN
	szTmp = szTmp.arg(QApplication::organizationName());
	szTmp = szTmp.arg(QApplication::organizationDomain());
#endif
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
	pButton = addLibraryButton(pLayout, pFrame, ":/logos/ffmpeg-logo.png", "FFmpeg");
	connect(pButton, SIGNAL(clicked()), this, SLOT(menuAboutFFmpegClicked()));
	pButton = addLibraryButton(pLayout, pFrame, ":/logos/libcurl-logo.png", "libcurl");
	connect(pButton, SIGNAL(clicked()), this, SLOT(menuAboutLibcurlClicked()));

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

void QAboutDialog::menuAboutFFmpegClicked()
{
	QString szTmp;
	szTmp = "<b>" + tr("About FFmpeg") + "</b><br/><br/>";
	szTmp += tr("This software uses code of <a href=http://ffmpeg.org>FFmpeg</a> licensed under the <a href=http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html>LGPLv2.1</a> and its source can be downloaded <a href=link_to_your_sources>here</a>.")+"<br/><br/>";

	szTmp += tr("FFmpeg libraries used by this program have been built with the following options:") + "<br/>";
	szTmp += "- <u>"+ tr("Windows 32bits version:") + "</u> </li></ul><i>--enable-memalign-hack --arch=x86 --target-os=mingw32 --cross-prefix=i686-w64-mingw32- --enable-shared --pkg-config=pkg-config</i>";

	QMessageBox::about(this, tr("About FFmpeg"), szTmp);
}

void QAboutDialog::menuAboutLibcurlClicked()
{
	QString szTmp;
	szTmp = "<b>" + tr("About libcurl") + "</b><br/><br/>";
	szTmp += tr("This software uses code of <a href=http://curl.haxx.se/libcurl/>libcurl</a> licensed under a <a href=http://curl.haxx.se/docs/copyright.html>MIT/X derivate license</a> and its source can be downloaded <a href=link_to_your_sources>here</a>.")+"<br/><br/>";

	QMessageBox::about(this, tr("About libcurl"), szTmp);
}
