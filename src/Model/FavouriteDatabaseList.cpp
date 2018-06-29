/*
 * FavouriteDatabaseList.cpp
 *
 *  Created on: 26 juin 2018
 *      Author: cdegeorgi
 */

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
	if(size() + 1 > MAX_ITEMS){
		removeFirst();
	}
	QList<DatabaseModel>::append(value);
}

void FavouriteDatabaseList::append(const QList<DatabaseModel>& value)
{
	if(size() + 1 > MAX_ITEMS){
		for(int i = 0 ; i < size() - MAX_ITEMS ; i++){
			removeFirst();
		}
	}
	QList<DatabaseModel>::append(value);
}
