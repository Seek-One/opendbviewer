/*
 * QHistoryDatabaseItem.h
 *
 *  Created on: 25 juil. 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUIMODEL_QHISTORYDATABASEITEM_H_
#define SRC_GUIMODEL_QHISTORYDATABASEITEM_H_

#include "Model/DatabaseModel.h"

#include <QStandardItem>

class QHistoryDatabaseItem : public QStandardItem
{
public:
	QHistoryDatabaseItem(const DatabaseModel& databaseModel);
	virtual ~QHistoryDatabaseItem();

	const DatabaseModel& getDatabaseModel() const;

//signals:
//	void itemClicked();

private:
	DatabaseModel m_databaseModel;
};

#endif /* SRC_GUIMODEL_QHISTORYDATABASEITEM_H_ */
