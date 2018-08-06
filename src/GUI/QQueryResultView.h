/*
 * QQueryResultView.h
 *
 *  Created on: 8 sept. 2017
 *      Author: ebeuque
 */

#ifndef SRC_GUI_QQUERYRESULTVIEW_H_
#define SRC_GUI_QQUERYRESULTVIEW_H_

#include <QTabWidget>

class QTableView;
class QTextEdit;
class QStandardItemModel;

class QQueryResultView : public QTabWidget {
	Q_OBJECT
public:
	QQueryResultView(QWidget* pParent = NULL);
	virtual ~QQueryResultView();

	QTableView* getDataTableView() const;
	QStandardItemModel* getDataResultsModel() const;
	QTextEdit* getConsoleTextEdit() const;;

	void showTabData();
	void showTabConsole();

private:
	QTableView* m_pDataTableView;
	QTextEdit* m_pConsoleTextEdit;
	QStandardItemModel* m_pDataResultsModel;
};

#endif /* SRC_GUI_QQUERYRESULTVIEW_H_ */
