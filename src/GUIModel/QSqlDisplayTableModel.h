/*
 * QSqlDisplayTableModel.h
 *
 *  Created on: 21 août 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUIMODEL_QSQLDISPLAYTABLEMODEL_H_
#define SRC_GUIMODEL_QSQLDISPLAYTABLEMODEL_H_

#include <QSqlTableModel>

class QSqlDisplayTableModel : public QSqlTableModel
{
public:
	explicit QSqlDisplayTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
	virtual ~QSqlDisplayTableModel();

	QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};

#endif /* SRC_GUIMODEL_QSQLDISPLAYTABLEMODEL_H_ */
