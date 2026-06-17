//
// Created by ebeuque on 16/06/2026.
//

#ifndef OPENDBVIEWER_DATABASEPROCESSHANDLER_H
#define OPENDBVIEWER_DATABASEPROCESSHANDLER_H

#include <QString>

class QSqlDisplayQueryModel;
class QSqlDisplayTableModel;

class DatabaseProcessHandler
{
public:
	DatabaseProcessHandler();
	virtual ~DatabaseProcessHandler();

	void setTableModel(QSqlDisplayTableModel* pTableModel);
	QSqlDisplayTableModel* getTableModel() const;

public:
	virtual void notifyQueryResult(const QString& szQuery, bool bSuccess, int iResultCount, const QString& szQueryResult);
	virtual void notifyQueryModel(QSqlDisplayQueryModel* pQueryModel);

	virtual void notifyQueriesFinished(bool bSuccess, bool bLastQueryHasResults);

public:
	QSqlDisplayTableModel* m_pTableModel;
};

#endif //OPENDBVIEWER_DATABASEPROCESSHANDLER_H