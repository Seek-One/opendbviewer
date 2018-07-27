/*
 * QHistoryDatabaseModel.h
 *
 *  Created on: 25 juil. 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUIMODEL_QHISTORYDATABASEMODEL_H_
#define SRC_GUIMODEL_QHISTORYDATABASEMODEL_H_

#include <QStandardItemModel>
#include <QFontMetrics>

class QHistoryDatabaseModel : public QStandardItemModel {
public:
	QHistoryDatabaseModel();
	virtual ~QHistoryDatabaseModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	void obtainInfoDesign(int iWidth, const QFont& font);

private:
	QFont m_font;
	int m_iWidth;
};

#endif /* SRC_GUIMODEL_QHISTORYDATABASEMODEL_H_ */
