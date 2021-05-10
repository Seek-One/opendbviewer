/*
 * QHistoryMenuWidget.h
 *
 *  Created on: 3 mai 2021
 *      Author: gsegard
 */

#ifndef SRC_WIDGET_QHISTORYMENUWIDGET_H_
#define SRC_WIDGET_QHISTORYMENUWIDGET_H_

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class QHistoryMenuWidget : public QWidget
{
	Q_OBJECT
public:
	QHistoryMenuWidget(QWidget* parent, const QString& szData);
	virtual ~QHistoryMenuWidget();

	void setTextLabel(const QString& szText);
	QPushButton* getButton() const;

signals:
	void signalAboutToBeRemoved(const QString& szData);

public slots:
	void slotAboutToBeRemoved();

private:
	QLabel* m_pLabel;
	QPushButton* m_pButton;
};

#endif /* SRC_WIDGET_QHISTORYMENUWIDGET_H_ */
