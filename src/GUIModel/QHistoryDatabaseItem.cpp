/*
 * QHistoryDatabaseItem.cpp
 *
 *  Created on: 25 juil. 2018
 *      Author: mlegarrec
 */

#include "QHistoryDatabaseItem.h"

QHistoryDatabaseItem::QHistoryDatabaseItem(const DatabaseModel& databaseModel)
{
	m_databaseModel = databaseModel;
}

QHistoryDatabaseItem::~QHistoryDatabaseItem()
{

}

const DatabaseModel& QHistoryDatabaseItem::getDatabaseModel() const
{
	return m_databaseModel;
}
