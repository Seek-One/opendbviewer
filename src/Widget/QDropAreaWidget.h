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
	QDropAreaWidget(const QString& dropAreaName, QWidget* parent = 0);
	virtual ~QDropAreaWidget();

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void paintEvent(QPaintEvent* event);

private:
	QString m_dropAreaName;

signals:
	void fileDropped(const QString& szFileUrl);
};

#endif /* SRC_WIDGET_QDROPAREAWIDGET_H_ */
