/*
 * QHistoryDatabaseModel.cpp
 *
 *  Created on: 25 juil. 2018
 *      Author: mlegarrec
 */

#include "QHistoryDatabaseItem.h"
#include "QHistoryDatabaseModel.h"

QHistoryDatabaseModel::QHistoryDatabaseModel()
{
	m_iWidth = 0;
}

QHistoryDatabaseModel::~QHistoryDatabaseModel()
{

}

QVariant QHistoryDatabaseModel::data(const QModelIndex &index, int role) const
{
	if(role == Qt::DisplayRole){
		QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(itemFromIndex(index));
		DatabaseModel databaseModel = pItem->getDatabaseModel();
		pItem->setEditable(false);

		switch(databaseModel.getDatabaseType()) {
		case DatabaseModel::SQLiteType:
		  	QString qElidedText, qFinalText;
		  	int iWidth = m_iWidth;
			qElidedText = databaseModel.getDatabasePath().section('/', 0, -2);
		  	QFontMetrics metrics(m_font);
		  	qElidedText = metrics.elidedText(qElidedText, Qt::ElideMiddle, iWidth-5);

			qFinalText = "<b>" + databaseModel.getDatabaseName() + "</b><br/>" + qElidedText;

			return qFinalText;
		//TODO other case (for now MySQL and PostgreSQL type)
		}
	}
	return QVariant();
}

void QHistoryDatabaseModel::obtainInfoDesign(int iWidth, const QFont& font)
{
	m_iWidth = iWidth;
	m_font = font;
}
