/*
 * QSqlDisplayQueryModel.h
 *
 *  Created on: 21 août 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUIMODEL_QSQLDISPLAYQUERYMODEL_H_
#define SRC_GUIMODEL_QSQLDISPLAYQUERYMODEL_H_

#include <QSqlQueryModel>

class QSqlDisplayQueryModel : public QSqlQueryModel
{
public:
	QSqlDisplayQueryModel(QObject *parent = 0);
	virtual ~QSqlDisplayQueryModel();

	QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};

#endif /* SRC_GUIMODEL_QSQLDISPLAYQUERYMODEL_H_ */
