/*
 * QLineNumberAreaView.h
 *
 *  Created on: 1 juil. 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QLINENUMBERAREAVIEW_H_
#define SRC_GUI_QLINENUMBERAREAVIEW_H_

#include "QSqlSourceView.h"

#include <QWidget>
#include <QSize>
#include <QPaintEvent>
#include <QPlainTextEdit>

/* Class created as per http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html alongside
 * QSqlSourceView class. This seemed to be the most flexible solution for possible future improvements.
 */

class QLineNumberAreaView : public QWidget
{
public:
	QLineNumberAreaView(QSqlSourceView *pEditor) : QWidget(pEditor) {
		m_pCodeEditor = pEditor;
    }

    QSize sizeHint() const {
        return QSize(m_pCodeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *pEvent) {
    	m_pCodeEditor->lineNumberAreaPaintEvent(pEvent);
    }

private:
    QSqlSourceView* m_pCodeEditor;
};


#endif /* SRC_GUI_QLINENUMBERAREAVIEW_H_ */
