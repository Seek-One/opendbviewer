/*
 * QOpenHistoryView.h
 *
 *  Created on: 23 juil. 2018
 *      Author: mlegarrec
 */

#ifndef SRC_GUI_QOPENHISTORYVIEW_H_
#define SRC_GUI_QOPENHISTORYVIEW_H_

#include <QWidget>
#include <QLabel>
#include <QTreeWidget>

#include "Model/DatabaseModel.h"
#include "Widget/QDropAreaWidget.h"
#include "Widget/QFileExplorerWidget.h"
#include "GUIModel/QHistoryDatabaseItem.h"

class QOpenHistoryView : public QWidget
{
	Q_OBJECT
public:
	QOpenHistoryView(QWidget* pParent);
	virtual ~QOpenHistoryView();

	QTreeView* getHistoryTreeView() const;
	QWidget* getHistoryInfoWidget() const;
	QLabel* getHistoryNameLabel() const;
	QLabel* getHistoryPathLabel() const;

private:
	QWidget* makeHistoryInfo(QWidget* pParent);

public slots:
	void onHistoryItemClicked(const QModelIndex&);
	void onHistoryItemDoubleClicked(const QModelIndex&);

signals:
	void openHistorySQLiteDatabase(const QModelIndex& index);
	void openHistoryInfo(const QModelIndex& index);

private:
	QTreeView* m_pHistoryTreeView;
	QWidget* m_pHistoryInfoWidget;

	QLabel* m_pHistoryNameLabel;
	QLabel* m_pHistoryPathLabel;
	QLabel* m_pHistoryHostLabel;
	QLabel* m_pHistoryPortLabel;
	QLabel* m_pHistoryUsernameLabel;
};

#endif /* SRC_GUI_QOPENHISTORYVIEW_H_ */
