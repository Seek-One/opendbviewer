/*
 * QMidClickClosableTabWidget.h
 *
 *  Created on: 18 sept. 2017
 *      Author: alavier
 */

#ifndef SRC_WIDGET_QMIDCLICKCLOSABLETABWIDGET_H_
#define SRC_WIDGET_QMIDCLICKCLOSABLETABWIDGET_H_

#include <QTabWidget>

class QMidClickClosableTabWidget : public QTabWidget {
public:
	QMidClickClosableTabWidget(QWidget* pParent = NULL);
	virtual ~QMidClickClosableTabWidget();

private:
	virtual bool eventFilter(QObject* pObject, QEvent* pEvent);
};

#endif /* SRC_WIDGET_QMIDCLICKCLOSABLETABWIDGET_H_ */
