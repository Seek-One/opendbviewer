/*
 * QSqlSourceView.cpp
 *
 *  Created on: 30 juin 2016
 *      Author: echopin
 */

#include "GUI/QSqlSourceView.h"
#include "GUI/QLineNumberAreaView.h"
#include "GUIController/QSqlHighlighterController.h"

#include <QHBoxLayout>
#include <QTextEdit>
#include <QColor>
#include <QWidget>
#include <QDebug>
#include <QTextDocument>
#include <QPoint>
#include <QTextBlock>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>

QSqlSourceView::QSqlSourceView(QWidget* parent) : QPlainTextEdit(parent)
{
	m_pLineNumberArea = new QLineNumberAreaView(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}

QSqlSourceView::~QSqlSourceView()
{
}

int QSqlSourceView::lineNumberAreaWidth()
{
	//Calculates the width of the line number area according to the line number
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void QSqlSourceView::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QSqlSourceView::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		m_pLineNumberArea->scroll(0, dy);
	else
		m_pLineNumberArea->update(0, rect.y(), m_pLineNumberArea->width(), rect.height());

	if(rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);

}

void QSqlSourceView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_pLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void QSqlSourceView::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::lightGray);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void QSqlSourceView::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(m_pLineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom())
    {
	   if (block.isVisible() && bottom >= event->rect().top())
	   {
		   QString number = QString::number(blockNumber + 1);
		   painter.setPen(Qt::black);
		   painter.drawText(0, top, m_pLineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
	   }
	   block = block.next();
	   top = bottom;
	   bottom = top + (int) blockBoundingRect(block).height();
	   ++blockNumber;
   }
}
