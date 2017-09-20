/*
 * QQueryResultView.h
 *
 *  Created on: 8 sept. 2017
 *      Author: ebeuque
 */

#ifndef SRC_GUI_QQUERYRESULTVIEW_H_
#define SRC_GUI_QQUERYRESULTVIEW_H_

#include <QTabWidget>

class QTreeView;
class QTextEdit;
class QStandardItemModel;

class QQueryResultView : public QTabWidget {
	Q_OBJECT
public:
	QQueryResultView(QWidget* pParent = NULL);
	virtual ~QQueryResultView();

	QTreeView* getDataTreeView() const;
	QStandardItemModel* getDataResultsModel() const;
	QTextEdit* getConsoleTextEdit() const;;

	void showTabData();
	void showTabConsole();

private:
	QTreeView* m_pDataTreeView;
	QTextEdit* m_pConsoleTextEdit;
	QStandardItemModel* m_pDataResultsModel;
};

#endif /* SRC_GUI_QQUERYRESULTVIEW_H_ */
