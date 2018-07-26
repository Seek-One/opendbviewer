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

}

QHistoryDatabaseModel::~QHistoryDatabaseModel()
{

}

QVariant QHistoryDatabaseModel::data(const QModelIndex &index, int role) const
{
	if(role == Qt::DisplayRole){
		QHistoryDatabaseItem* pItem = dynamic_cast<QHistoryDatabaseItem*>(itemFromIndex(index));
		DatabaseModel databaseModel = pItem->getDatabaseModel();

		switch(databaseModel.getDatabaseType()) {
		case DatabaseModel::SQLiteType:
		  	QString qElidedText, qFinalText;

			qElidedText = databaseModel.getDatabasePath().section('/', 0, -2);
			qFinalText = databaseModel.getDatabaseName() + "\n" + qElidedText;

			return qFinalText;
		//TODO other case (for now MySQL and PostgreSQL type)
		}
	}
	return QVariant();
}
