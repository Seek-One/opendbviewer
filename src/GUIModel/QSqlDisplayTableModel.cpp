/*
 * QSqlDisplayTableModel.cpp
 *
 *  Created on: 21 août 2018
 *      Author: mlegarrec
 */

#include <GUIModel/QSqlDisplayTableModel.h>

#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QWidget>

QSqlDisplayTableModel::QSqlDisplayTableModel(QObject* parent, QSqlDatabase db) : QSqlTableModel(parent, db)
{
}

QSqlDisplayTableModel::~QSqlDisplayTableModel()
{
}

QVariant QSqlDisplayTableModel::data(const QModelIndex &index, int role) const
{
	QVariant value = QSqlQueryModel::data(index, role);
	if (!index.isValid()) {
		return QVariant();
	}
	switch (role) {
	case Qt::FontRole:
		if (QSqlTableModel::data(index).isNull()) {
			QFont italicFont;
			italicFont.setItalic(true);
			return italicFont;
		}
		break;
	case Qt::ForegroundRole:
		if (QSqlTableModel::data(index).isNull()) {
			return QBrush(Qt::darkGray);
		}
		break;
	case Qt::DisplayRole:
		if (value.isNull()) {
			return QVariant("NULL");
		}
		break;
	default:
		break;
	}
	return QSqlTableModel::data(index, role);
}

bool QSqlDisplayTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool bSet = QSqlTableModel::setData(index, value, role);
	if (QSqlTableModel::editStrategy() == OnFieldChange && !bSet) {
		emit databaseError();
	}
	return bSet;
}

