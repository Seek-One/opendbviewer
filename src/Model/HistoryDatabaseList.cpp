/*
 * FavouriteDatabaseList.cpp
 *
 *  Created on: 26 juin 2018
 *      Author: cdegeorgi
 */
#include <QDebug>

#include "FavouriteDatabaseList.h"

#define MAX_ITEMS 10

FavouriteDatabaseList::FavouriteDatabaseList()
{

}

FavouriteDatabaseList::~FavouriteDatabaseList()
{

}

void FavouriteDatabaseList::append(const DatabaseModel& value)
{
	if (!isEmpty()) removeAll(value);
	if(size() + 1 > MAX_ITEMS) removeFirst();
	QList::append(value);
}

void FavouriteDatabaseList::append(const QList<DatabaseModel>& value)
{
	for (int i=0; i!=value.size(); i++) {
		append(value.at(i));
	}
}
