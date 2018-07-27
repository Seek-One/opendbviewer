/*
 * HTMLDelegate.h
 *
 *  Created on: 26 juil. 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUIMODEL_HTMLDELEGATE_H_
#define SRC_GUIMODEL_HTMLDELEGATE_H_

#include <QStyledItemDelegate>

class HTMLDelegate : public QStyledItemDelegate
{
public:
	HTMLDelegate();
	virtual ~HTMLDelegate();

protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif /* SRC_GUIMODEL_HTMLDELEGATE_H_ */
