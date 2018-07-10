/*
 * FavouriteDatabaseListTestCase.h
 *
 *  Created on: 9 juil. 2018
 *      Author: mlegarrec
 */

#ifndef TEST_MODEL_FAVOURITEDATABASELISTTESTCASE_H_
#define TEST_MODEL_FAVOURITEDATABASELISTTESTCASE_H_

#include <QtTest>

class FavouriteDatabaseListTestCase : public QObject {
	Q_OBJECT
private slots:
	void test_appendDatabaseModel();
	void test_appendListDatabaseModel();
};

#endif /* TEST_MODEL_FAVOURITEDATABASELISTTESTCASE_H_ */
