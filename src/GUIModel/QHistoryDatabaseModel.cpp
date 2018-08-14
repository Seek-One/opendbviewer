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
		DatabaseModel::DatabaseType type = databaseModel.getDatabaseType();
		QString qFinalText, qElidedText;
	  	int iWidth = m_iWidth, iCorrect = 5; //iCorrect correct a side effet on the QTreeView
	  	QFontMetrics metrics(m_font);

		switch(type) {
		case DatabaseModel::SQLiteType:
			qElidedText = databaseModel.getDatabasePath().section('/', 0, -2);
		  	qElidedText = metrics.elidedText(qElidedText, Qt::ElideMiddle, iWidth-iCorrect);
			qFinalText = "<b>" + databaseModel.getDatabaseName() + "</b><br/>" + qElidedText;
			break;
		case DatabaseModel::MySQLType:
			qFinalText = "<b>" + databaseModel.getDatabaseName() + "</b><br/>" + databaseModel.getDatabaseHost() + "&nbsp;&nbsp;&nbsp;" + databaseModel.getDatabaseUsername();
			break;
		case DatabaseModel::PostgreSQLType:
			qFinalText = "<b>" + databaseModel.getDatabaseName() + "</b><br/>" + databaseModel.getDatabaseHost() + "&nbsp;&nbsp;&nbsp;" + databaseModel.getDatabaseUsername();
			break;
		default:
			break;
		}
		return qFinalText;
	}
	return QVariant();
}

void QHistoryDatabaseModel::obtainInfoDesign(int iWidth, const QFont& font)
{
	m_iWidth = iWidth;
	m_font = font;
}
