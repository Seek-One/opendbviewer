/*
 * QSqlSourceView.h
 *
 *  Created on: 30 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QSQLSOURCEVIEW_H_
#define SRC_GUI_QSQLSOURCEVIEW_H_

#include "GUIController/QSqlHighlighterController.h"

#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>

#include <QObject>
#include <QTextDocument>

/* Class created as per http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html alongside
 * QLineNumberAreaView class. This seemed to be the most flexible solution for possible future improvements.
 */

class QSqlSourceView : public QPlainTextEdit
{
	Q_OBJECT
public:
	QSqlSourceView(QWidget* parent);
	virtual ~QSqlSourceView();
	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();

protected:
	void resizeEvent(QResizeEvent *event);

public slots:
	void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget* m_pLineNumberArea;

};

#endif /* SRC_GUI_QSQLSOURCEVIEW_H_ */
