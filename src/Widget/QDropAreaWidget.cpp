/*
 * QDropAreaWidget.cpp
 *
 *  Created on: 1 juin 2018
 *      Author: cdegeorgi
 */

#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QUrl>

#include "QDropAreaWidget.h"


QDropAreaWidget::QDropAreaWidget(const QString& dropAreaName, QWidget* parent)
	: QFrame(parent)
{
	m_dropAreaName = dropAreaName;

	setAcceptDrops(true);
}

QDropAreaWidget::~QDropAreaWidget()
{

}

void QDropAreaWidget::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData* pMimeData = event->mimeData();
	if(pMimeData){
		QList<QUrl> listUrls = pMimeData->urls();
		QList<QUrl>::iterator iter;
		QString szFileName;
		for(iter = listUrls.begin(); iter != listUrls.end(); ++iter){
			szFileName = (*iter).path();
			if(szFileName.endsWith(".sqlite") || szFileName.endsWith(".db")){
				event->acceptProposedAction();
				break;
			}
		}
	}
}

void QDropAreaWidget::dropEvent(QDropEvent *event)
{
	const QMimeData* pMimeData = event->mimeData();
	if(pMimeData){
		QList<QUrl> listUrls = pMimeData->urls();
		QList<QUrl>::iterator iter;
		QString szUrl;
		for(iter = listUrls.begin(); iter != listUrls.end(); ++iter){
			szUrl = (*iter).path();
			if(szUrl.endsWith(".sqlite") || szUrl.endsWith(".db")){
				emit fileDropped(szUrl);
			}
		}
	}
}

void QDropAreaWidget::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	QPainterPath path;
	path.addRoundedRect(QRectF(0, 0, width(), height()), 10, 10);
	QPen pen(Qt::black, 1);
	pen.setStyle(Qt::DashLine);
	p.setPen(pen);
	p.fillPath(path, Qt::white);
	p.drawPath(path);

	QFont font = p.font();
	font.setPointSize(font.pointSize()*1.5);
	p.setFont(font);

	p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, m_dropAreaName);
}
