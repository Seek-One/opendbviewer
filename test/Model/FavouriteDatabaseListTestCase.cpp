/*
 * FavouriteDatabaseListTestCase.cpp
 *
 *  Created on: 9 juil. 2018
 *      Author: mlegarrec
 */

#include "src/Model/FavouriteDatabaseList.h"
#include "FavouriteDatabaseListTestCase.h"

void FavouriteDatabaseListTestCase::test_appendDatabaseModel()
{
	//Test taille liste
	FavouriteDatabaseList listDatabase;

	//Insertion element vide
	DatabaseModel emptydatabase;
	listDatabase.append(emptydatabase);
	QVERIFY(listDatabase.at(0)==emptydatabase);

	//insertion élément(s) plein(s)
	DatabaseModel database("database0", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database1", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database2", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database3", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database4", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database5", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database6", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database7", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database8", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database9", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database10", "sqlite");
	listDatabase.append(database);
	database = DatabaseModel("database11", "sqlite");
	listDatabase.append(database);

	QVERIFY(listDatabase.size() <= 10);
	for (int i=0; i<10; i++) {
		QVERIFY(listDatabase.at(i) != emptydatabase);
	}
}

void FavouriteDatabaseListTestCase::test_appendListDatabaseModel()
{
	FavouriteDatabaseList listFavDatabase;
	QList<DatabaseModel> listInsertion;

	listFavDatabase.append(listInsertion);
	//Insertion empty list into list of favorites databases
	QVERIFY(listFavDatabase.size()<=10);

	DatabaseModel database("database0", "sqlite");
	listInsertion.append(database);
	database = DatabaseModel("database1", "sqlite");
	listInsertion.append(database);
	database = DatabaseModel("database2", "sqlite");
	listInsertion.append(database);
	database = DatabaseModel("database3", "sqlite");
	listInsertion.append(database);
	database = DatabaseModel("database4", "sqlite");
	listInsertion.append(database);
	//Insertion list into empty list of favorite databases
	listFavDatabase.append(listInsertion);
	//QVERIFY(!listFavDatabase.isEmpty());
	QVERIFY(listFavDatabase.size()<=10);

	//Insertion list with duplicates into list of favorite databases
	listFavDatabase.append(listInsertion);
	QVERIFY(listFavDatabase.size()<=5);
}

