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
#include <QStackedWidget>

#include "Model/DatabaseModel.h"
#include "Widget/QFileExplorerWidget.h"
#include "GUIModel/QHistoryDatabaseItem.h"

class QOpenHistoryView : public QWidget
{
	Q_OBJECT
public:
	QOpenHistoryView(QWidget* pParent);
	virtual ~QOpenHistoryView();

	QTreeView* getHistoryTreeView() const;
	QStackedWidget* getHistoryInfoStackedWidget() const;
	QLabel* getHistoryNameLabel() const;
	QLabel* getHistoryPathLabel() const;
	QLabel* getHistoryHostLabel() const;
	QLabel* getHistoryPortLabel() const;
	QLabel* getHistoryUsernameLabel() const;

	void makeHistoryInfo(DatabaseModel::DatabaseType type);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool eventFilter(QObject* pObject, QEvent* pEvent);

public slots:
	void onHistoryItemClicked(const QModelIndex&);
	void onHistoryItemDoubleClicked(const QModelIndex&);

signals:
	void callHistoryDatabase(const QModelIndex&);
	void callHistoryInfo(const QModelIndex&);
	void improveDesign(int iWidth);

private:
	QStackedWidget* m_pHistoryInfoStackedWidget;
	QTreeView* m_pHistoryTreeView;
	QLabel* m_pHistoryNameLabel;
	QLabel* m_pHistoryPathLabel;
	QLabel* m_pHistoryHostLabel;
	QLabel* m_pHistoryPortLabel;
	QLabel* m_pHistoryUsernameLabel;
};

#endif /* SRC_GUI_QOPENHISTORYVIEW_H_ */
