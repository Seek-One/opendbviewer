/*
 * HistoryDatabaseList.h
 *
 *  Created on: 26 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_MODEL_HISTORYDATABASELIST_H_
#define SRC_MODEL_HISTORYDATABASELIST_H_

#include <QList>

#include "DatabaseModel.h"

class HistoryDatabaseList : public QList<DatabaseModel> {
public:
	HistoryDatabaseList();
	virtual ~HistoryDatabaseList();

	void append(const DatabaseModel& value);
	void append(const QList<DatabaseModel>& value);
};

#endif /* SRC_MODEL_HISTORYDATABASELIST_H_ */
