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

	switch(role){
	case Qt::FontRole:
	{
		if (QSqlQueryModel::data(this->index(item.row(), item.column())).isNull()) {
			QFont italicFont;
			italicFont.setItalic(true);
			return italicFont;
		}
	}
	break;
	case Qt::ForegroundRole:
	{
		if (QSqlQueryModel::data(this->index(item.row(), item.column())).isNull()) {
			return QColor(Qt::darkGray);
		}
	}
	break;
	case Qt::DisplayRole:
	{
		if (value.isNull()) {
			return QVariant("NULL");
		}
	}
	break;
	case DataTypeRole : {
		QVariant value = QSqlQueryModel::data(item, Qt::DisplayRole);
		if (value.isNull()) {
			return DataTypeNull;
		}
	}
	break;
	default: break;
	}
	return value;
}
