/*
 * FavouriteDatabaseList.h
 *
 *  Created on: 26 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_MODEL_FAVOURITEDATABASELIST_H_
#define SRC_MODEL_FAVOURITEDATABASELIST_H_

#include <QList>

#include "DatabaseModel.h"

class FavouriteDatabaseList : public QList<DatabaseModel> {
public:
	FavouriteDatabaseList();
	virtual ~FavouriteDatabaseList();

	void append(const DatabaseModel& value);
	void append(const QList<DatabaseModel>& value);
};

#endif /* SRC_MODEL_FAVOURITEDATABASELIST_H_ */
