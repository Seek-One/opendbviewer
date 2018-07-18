/*
 * HistoryDatabaseListTestCase.cpp
 *
 *  Created on: 9 juil. 2018
 *      Author: mlegarrec
 */

#include <Model/HistoryDatabaseList.h>
#include "HistoryDatabaseListTestCase.h"

void HistoryDatabaseListTestCase::test_appendDatabaseModel()
{
	HistoryDatabaseList listDatabase;

	//Test about the size of the list
	for(int i = 0; i < 10; ++i){
		listDatabase.append(DatabaseModel(QString::number(i), DatabaseModel::SQLiteType));
		QVERIFY(listDatabase.size() == (i + 1));
	}
	listDatabase.append(DatabaseModel("10", DatabaseModel::SQLiteType));
	QVERIFY(listDatabase.size() == 10);

	//Clear the list
	listDatabase.clear();

	//Test on duplicates
	listDatabase.append(DatabaseModel("", DatabaseModel::SQLiteType));
	QVERIFY(listDatabase.size() == 1);
	listDatabase.append(DatabaseModel("", DatabaseModel::SQLiteType));
	QVERIFY(listDatabase.size() == 1);
}

void HistoryDatabaseListTestCase::test_appendListDatabaseModel()
{
	HistoryDatabaseList listHistDatabase;
	QList<DatabaseModel> listInsertion;

	listHistDatabase.append(listInsertion);
	//Insertion empty list into list of favorites databases
	QVERIFY(listHistDatabase.size()<=10);

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
	listHistDatabase.append(listInsertion);
	//QVERIFY(!listFavDatabase.isEmpty());
	QVERIFY(listHistDatabase.size()<=10);

	//Insertion list with duplicates into list of favorite databases
	listHistDatabase.append(listInsertion);
	QVERIFY(listHistDatabase.size()<=5);
}

