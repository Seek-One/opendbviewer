/*
 * HTMLDelegate.cpp
 *
 *  Created on: 26 juil. 2018
 *      Author: mlegarrec
 */

#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

#include "HTMLDelegate.h"

HTMLDelegate::HTMLDelegate()
{
}

HTMLDelegate::~HTMLDelegate()
{
}

void HTMLDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem options = option;
	initStyleOption(&options, index);

	painter->save();

	QTextDocument doc;
	doc.setHtml(options.text);

	options.text = "";
	options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

	// shift text right to make icon visible
	QSize iconSize = options.icon.actualSize(options.rect.size());
	painter->translate(options.rect.left()+iconSize.width(), options.rect.top());
	QRect clip(0, 0, options.rect.width()+iconSize.width(), options.rect.height());

	painter->setClipRect(clip);
	QAbstractTextDocumentLayout::PaintContext ctx;
	ctx.clip = clip;
	doc.documentLayout()->draw(painter, ctx);

	painter->restore();
}

QSize HTMLDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem options = option;
    initStyleOption(&options, index);

    QTextDocument doc;
    doc.setHtml(options.text);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

