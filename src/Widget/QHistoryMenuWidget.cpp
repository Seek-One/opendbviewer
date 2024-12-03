/*
 * QHistoryMenuWidget.cpp
 *
 *  Created on: 3 mai 2021
 *      Author: gsegard
 */

#include <QIcon>
#include <QHBoxLayout>

#include "GUIController/QDatabaseWorksheetViewController.h"
#include "Widget/QHistoryMenuWidget.h"

QHistoryMenuWidget::QHistoryMenuWidget(QWidget* parent, const QString& szData)
	: QWidget(parent)
{
	m_pLabel = NULL;
	m_pButton = NULL;

	QHBoxLayout* pMainLayout = new QHBoxLayout();
	setLayout(pMainLayout);

	m_pLabel = new QLabel(this);
	pMainLayout->addWidget(m_pLabel);

	m_pButton = new QPushButton(this);
	m_pButton->setIcon(QIcon::fromTheme("list-remove"));
	m_pButton->setFixedWidth(30);
	m_pButton->setStyleSheet("border: none;");
	m_pButton->setToolTip(tr("Delete"));
	m_pButton->setObjectName(szData);
	connect(m_pButton, SIGNAL(clicked()), this, SLOT(slotAboutToBeRemoved()));

	pMainLayout->setContentsMargins(10, 3, 10, 3);
	pMainLayout->addWidget(m_pButton);
}

QHistoryMenuWidget::~QHistoryMenuWidget()
{

}

void QHistoryMenuWidget::setTextLabel(const QString& szText)
{
	m_pLabel->setText(szText);
}

QPushButton* QHistoryMenuWidget::getButton() const
{
	return m_pButton;
}

void QHistoryMenuWidget::slotAboutToBeRemoved()
{
	if(this->sender()->isWidgetType()){
		QPushButton* pButton = qobject_cast<QPushButton*>(this->sender());
		if(pButton){
			emit signalAboutToBeRemoved(pButton->objectName());
		}
	}
}

void QHistoryMenuWidget::enterEvent(QEnterEvent *event)
{
	QPalette palette = this->style()->standardPalette();
	palette.setColor(QPalette::Window, palette.highlight().color());
	palette.setColor(QPalette::WindowText, Qt::white);
	setAutoFillBackground(true);
	setPalette(palette);
}

void QHistoryMenuWidget::leaveEvent(QEvent *event)
{
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	palette.setColor(QPalette::WindowText, Qt::black);
	setAutoFillBackground(true);
	setPalette(palette);
}
