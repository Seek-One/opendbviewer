/*
 * QSqlDisplayQueryModel.cpp
 *
 *  Created on: 21 août 2018
 *      Author: mlegarrec
 */

#include <GUIModel/QSqlDisplayQueryModel.h>

#include <QColor>
#include <QFont>

QSqlDisplayQueryModel::QSqlDisplayQueryModel(QObject *parent)
{
}

QSqlDisplayQueryModel::~QSqlDisplayQueryModel()
{
}

QVariant QSqlDisplayQueryModel::data(const QModelIndex &item, int role) const
{
	QVariant value = QSqlQueryModel::data(item, role);
	if (!item.isValid()) {
		return QVariant();
	}

	if (role == Qt::FontRole) {
		if (QSqlQueryModel::data(this->index(item.row(), item.column())).isNull()) {
			QFont italicFont;
			italicFont.setItalic(true);
			return italicFont;
		}
	}

	if (role == Qt::ForegroundRole) {
		if (QSqlQueryModel::data(this->index(item.row(), item.column())).isNull()) {
			return QColor(Qt::darkGray);
		}
	}

	if (role == Qt::DisplayRole && value.isNull()) {
		return QVariant("NULL");
	}
	return value;
}
