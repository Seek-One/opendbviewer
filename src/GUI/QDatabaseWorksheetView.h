/*
 * QDatabaseWorksheetView.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASEWORKSHEETVIEW_H_
#define SRC_GUI_QDATABASEWORKSHEETVIEW_H_

#include "QDatabaseTableView.h"
#include "QSqlSourceView.h"

#include <QPushButton>
#include <QToolBar>
#include <QTextEdit>
#include <QStandardItemModel>

class QQueryResultView;

class QDatabaseWorksheetView : public QWidget
{
public:
	QDatabaseWorksheetView(QWidget* parent = 0);
	virtual ~QDatabaseWorksheetView();
	QPushButton* getExecuteButton() const;
	QPushButton* getReformatButton() const;
	QPushButton* getClearTextButton() const;
	QSqlSourceView* getWorksheetTextEdit() const;
	QTextEdit* getWorksheetConsoleTextEdit() const;
	QStandardItemModel* getWorksheetResultsModel() const;
	QTreeView* getWorksheetTreeView() const;

	void showTabData();
	void showTabConsole();

private:
	QToolBar* makeWorksheetToolbar();

private:
	QPushButton *m_pExecuteButton;
	QPushButton *m_pReformatButton;
	QPushButton *m_pClearTextButton;
	QSqlSourceView* m_pWorksheetTextEdit;
	QQueryResultView* m_pQueryResultView;
};

#endif /* SRC_GUI_QDATABASEWORKSHEETVIEW_H_ */
