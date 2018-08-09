/*
 * DatabaseCallback.h
 *
 *  Created on: 6 sept. 2017
 *      Author: ebeuque
 */

#ifndef SRC_DATABASE_DATABASECALLBACK_H_
#define SRC_DATABASE_DATABASECALLBACK_H_

#include <QString>

#include "Model/DatabaseModel.h"

enum DatabaseQueryStep {
	DBQueryStepStart,
	DBQueryStepRow,
	DBQueryStepEnd,
};

typedef void (*DbLoadTableCB)(const QString& szTable, void* user_data);
typedef void (*DbLoadTableDescription)(const QStringList& listRowHeader, const QStringList& listRowData, DatabaseQueryStep step, void* user_data);
typedef void (*DbLoadTableCreationScript)(const QString& szCreationScriptString, void* user_data);

#endif /* SRC_DATABASE_DATABASECALLBACK_H_ */
