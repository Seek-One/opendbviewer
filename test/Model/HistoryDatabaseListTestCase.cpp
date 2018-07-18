/*
 * FavouriteDatabaseListTestCase.cpp
 *
 *  Created on: 9 juil. 2018
 *      Author: mlegarrec
 */

#include "Model/FavouriteDatabaseList.h"
#include "FavouriteDatabaseListTestCase.h"

void FavouriteDatabaseListTestCase::test_appendDatabaseModel()
{
	FavouriteDatabaseList listDatabase;

	//Test about the size of the list
	for(int i = 0; i < 10; ++i){
		listDatabase.append(DatabaseModel(QString::number(i), i));
		QVERIFY(listDatabase.size() == (i + 1));
	}
	listDatabase.append(DatabaseModel("10", 10));
	QVERIFY(listDatabase.size() == 10);

	//Clear the list
	listDatabase.clear();

	//Test on duplicates
	listDatabase.append(DatabaseModel("", 1));
	QVERIFY(listDatabase.size() == 1);
	listDatabase.append(DatabaseModel("", 1));
	QVERIFY(listDatabase.size() == 1);
}

void FavouriteDatabaseListTestCase::test_appendListDatabaseModel()
{
	FavouriteDatabaseList listFavDatabase;
	QList<DatabaseModel> listInsertion;

	listFavDatabase.append(listInsertion);
	//Insertion empty list into list of favorites databases
	QVERIFY(listFavDatabase.size()<=10);

	DatabaseModel database("database0", DatabaseModel::SQLiteType);
	listInsertion.append(database);
	database = DatabaseModel("database1", DatabaseModel::SQLiteType);
	listInsertion.append(database);
	database = DatabaseModel("database2", DatabaseModel::SQLiteType);
	listInsertion.append(database);
	database = DatabaseModel("database3", DatabaseModel::SQLiteType);
	listInsertion.append(database);
	database = DatabaseModel("database4", DatabaseModel::SQLiteType);
	listInsertion.append(database);
	//Insertion list into empty list of favorite databases
	listFavDatabase.append(listInsertion);
	//QVERIFY(!listFavDatabase.isEmpty());
	QVERIFY(listFavDatabase.size()<=10);

	//Insertion list with duplicates into list of favorite databases
	listFavDatabase.append(listInsertion);
	QVERIFY(listFavDatabase.size()<=5);
}

