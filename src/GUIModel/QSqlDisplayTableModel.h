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
	Q_OBJECT
public:
	explicit QSqlDisplayTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
	virtual ~QSqlDisplayTableModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

signals:
	void databaseError();

private:
	QVector<QModelIndex> m_ErrorIndexVector;
};

#endif /* SRC_GUIMODEL_QSQLDISPLAYTABLEMODEL_H_ */
