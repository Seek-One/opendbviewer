//
// Created by ebeuque on 16/06/2026.
//

#include "DatabaseProcessHandler.h"

DatabaseProcessHandler::DatabaseProcessHandler()
{
	m_pTableModel = nullptr;
}

DatabaseProcessHandler::~DatabaseProcessHandler()
{

}

void DatabaseProcessHandler::setTableModel(QSqlDisplayTableModel* pTableModel)
{
	m_pTableModel = pTableModel;
}

QSqlDisplayTableModel* DatabaseProcessHandler::getTableModel() const
{
	return m_pTableModel;
}

void DatabaseProcessHandler::notifyQueryResult(const QString& szQuery, bool bSuccess, int iResultCount, const QString& szQueryResult)
{

}

void DatabaseProcessHandler::notifyQueryModel(QSqlDisplayQueryModel* pQueryModel)
{

}

void DatabaseProcessHandler::notifyQueriesFinished(bool bSuccess, bool bLastQueryHasResults)
{

}