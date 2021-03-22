/*
 * QDropAreaWidget.cpp
 *
 *  Created on: 1 juin 2018
 *      Author: cdegeorgi
 */

#include <QPainter>
#include <QPainterPath>

#include "QDropAreaWidget.h"


QDropAreaWidget::QDropAreaWidget(const QString& dropAreaName, QWidget* parent)
	: QFrame(parent)
{
	m_dropAreaName = dropAreaName;
	m_bRepaint = false;
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

QDropAreaWidget::~QDropAreaWidget()
{

}

void QDropAreaWidget::startPaint()
{
	m_bRepaint = true;
	update();
}

void QDropAreaWidget::stopPaint()
{
	m_bRepaint = false;
	update();
}

void QDropAreaWidget::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	QPainterPath path;
	QPen pen(Qt::black, 1);
	QFont font = p.font();
	font.setBold(true);

	if (m_bRepaint) {
		p.setRenderHint(QPainter::Antialiasing);
		path.addRoundedRect(QRectF(0, 0, width(), height()), 0, 0);
		pen.setStyle(Qt::DashLine);
		pen.setWidth(5);
		p.setPen(pen);
		p.drawPath(path);
		font.setPointSize(font.pointSize()*5);
		p.setFont(font);
		p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, m_dropAreaName);
	}
}
