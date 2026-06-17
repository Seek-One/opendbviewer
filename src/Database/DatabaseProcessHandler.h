//
// Created by ebeuque on 16/06/2026.
//

#ifndef OPENDBVIEWER_DATABASEPROCESSHANDLER_H
#define OPENDBVIEWER_DATABASEPROCESSERHANDLER_H

#include <QString>

class QSqlDisplayQueryModel;

class DatabaseProcessHandler
{
public:
	DatabaseProcessHandler();
	virtual ~DatabaseProcessHandler();

public:
	virtual void notifyQueryResult(const QString& szQuery, bool bSuccess, const QString& szQueryResult);
	virtual void notifyQueryModel(QSqlDisplayQueryModel* pQueryModel);

	virtual void notifyQueriesFinished(bool bSuccess, bool bLastQueryHasResults);
};

#endif //OPENDBVIEWER_DATABASECONTROLLERHANDLER_H