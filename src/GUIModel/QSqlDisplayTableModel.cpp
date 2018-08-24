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
	QVariant value = QSqlTableModel::data(index, role);
	int iOccur = m_ErrorIndexVector.indexOf(index);
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
		if (iOccur != -1) {
			QFont boldFont;
			boldFont.setBold(true);
			return boldFont;
		}
		break;
	case Qt::ForegroundRole:
		if (QSqlTableModel::data(index).isNull()) {
			return QBrush(Qt::darkGray);
		}
		if (iOccur != -1) {
			return QBrush(Qt::red);
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
	return value;
}

bool QSqlDisplayTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool bSet = QSqlTableModel::setData(index, value, role);
	if (QSqlTableModel::editStrategy() == OnFieldChange){
		if(!bSet) {
			m_ErrorIndexVector.push_back(index);
			emit databaseError();
		} else {
			int iOccur = m_ErrorIndexVector.indexOf(index);
			if (iOccur != -1) {
				m_ErrorIndexVector.remove(iOccur);
			}
		}
	}
	return bSet;
}

