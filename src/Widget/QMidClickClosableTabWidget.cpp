/*
 * QMidClickClosableTabWidget.cpp
 *
 *  Created on: 18 sept. 2017
 *      Author: alavier
 */

#include <QMouseEvent>
#include <QTabBar>

#include "QMidClickClosableTabWidget.h"

QMidClickClosableTabWidget::QMidClickClosableTabWidget(QWidget* pParent): QTabWidget(pParent)
{
	tabBar()->installEventFilter(this);
}

QMidClickClosableTabWidget::~QMidClickClosableTabWidget()
{
}

bool QMidClickClosableTabWidget::eventFilter(QObject* pObject, QEvent* pEvent)
{
	if ((pObject == tabBar()) && (pEvent->type() == QEvent::MouseButtonPress)) {
		QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(pEvent);
		if (pMouseEvent->button() == Qt::MiddleButton) {
			emit tabCloseRequested(tabBar()->tabAt(pMouseEvent->pos()));
			return true;
		}
	}
	return QTabWidget::eventFilter(pObject, pEvent);
}
