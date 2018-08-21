/*
 * QSqlDisplayTableModel.cpp
 *
 *  Created on: 21 août 2018
 *      Author: mlegarrec
 */

#include <GUIModel/QSqlDisplayTableModel.h>

#include <QColor>
#include <QFont>

QSqlDisplayTableModel::QSqlDisplayTableModel(QObject* parent, QSqlDatabase db) : QSqlTableModel(parent, db)
{
}

QSqlDisplayTableModel::~QSqlDisplayTableModel()
{
}

QVariant QSqlDisplayTableModel::data(const QModelIndex &item, int role) const
{
	QVariant value = QSqlQueryModel::data(item, role);
	if (!item.isValid()) {
		return QVariant();
	}

	if (role == Qt::FontRole) {
			if (QSqlTableModel::data(this->index(item.row(), item.column())).isNull()) {
				QFont italicFont;
				italicFont.setItalic(true);
				return italicFont;
			}
		}

	if (role == Qt::ForegroundRole) {
		if (QSqlTableModel::data(this->index(item.row(), item.column())).isNull()) {
			return QColor(Qt::darkGray);
		}
	}

	if (role == Qt::DisplayRole && value.isNull()) {
		return QVariant("NULL");
	}
	return QSqlTableModel::data(item, role);
}

