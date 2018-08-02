/*
 * QDropAreaWidget.h
 *
 *  Created on: 1 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_WIDGET_QDROPAREAWIDGET_H_
#define SRC_WIDGET_QDROPAREAWIDGET_H_

#include <QFrame>

class QDropAreaWidget : public QFrame {
	Q_OBJECT

public:
	QDropAreaWidget(const QString& dropAreaName, QWidget* parent);
	virtual ~QDropAreaWidget();

	void startPaint();
	void stopPaint();

protected:
	void paintEvent(QPaintEvent* event);

private:
	QString m_dropAreaName;
	bool m_bRepaint;
};

#endif /* SRC_WIDGET_QDROPAREAWIDGET_H_ */
