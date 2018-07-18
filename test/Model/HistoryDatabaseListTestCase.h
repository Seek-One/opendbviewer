/*
 * HistoryDatabaseListTestCase.h
 *
 *  Created on: 9 juil. 2018
 *      Author: mlegarrec
 */

#ifndef TEST_MODEL_HISTORYDATABASELISTTESTCASE_H_
#define TEST_MODEL_HISTORYDATABASELISTTESTCASE_H_

#include <QtTest>

class HistoryDatabaseListTestCase : public QObject {
	Q_OBJECT
private slots:
	void test_appendDatabaseModel();
	void test_appendListDatabaseModel();
};

#endif /* TEST_MODEL_HISTORYDATABASELISTTESTCASE_H_ */
