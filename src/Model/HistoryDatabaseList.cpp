/*
 * HistoryDatabaseList.cpp
 *
 *  Created on: 26 juin 2018
 *      Author: cdegeorgi
 */
#include <QDebug>

#include "HistoryDatabaseList.h"

#define MAX_ITEMS 10

HistoryDatabaseList::HistoryDatabaseList()
{

}

HistoryDatabaseList::~HistoryDatabaseList()
{

}

void HistoryDatabaseList::append(const DatabaseModel& value)
{
	if (!isEmpty()) {
		removeAll(value);
	}
	if(size() + 1 > MAX_ITEMS) {
		removeFirst();
	}
	QList::append(value);
}

void HistoryDatabaseList::append(const QList<DatabaseModel>& value)
{
	for (int i=0; i!=value.size(); i++) {
		append(value.at(i));
	}
}
