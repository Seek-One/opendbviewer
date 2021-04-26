/*
 * QDatabaseWorksheetView.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASEWORKSHEETVIEW_H_
#define SRC_GUI_QDATABASEWORKSHEETVIEW_H_

#include <QComboBox>
#include <QMenu>
#include <QPushButton>
#include <QToolBar>
#include <QTextEdit>
#include <QStandardItemModel>

#include "QDatabaseTableView.h"
#include "QSqlSourceView.h"

class QQueryResultView;

class QDatabaseWorksheetView : public QWidget
{
	Q_OBJECT
public:
	QDatabaseWorksheetView(QWidget* parent = 0);
	virtual ~QDatabaseWorksheetView();
	QPushButton* getExecuteButton() const;
	QPushButton* getReformatButton() const;
	QPushButton* getClearTextButton() const;
	QPushButton* getImportButton() const;
	QPushButton* getExportButton() const;
	QSqlSourceView* getWorksheetTextEdit() const;
	QTextEdit* getWorksheetConsoleTextEdit() const;
	QPushButton* getRequestHistoryButton() const;
	QMenu* getRequestHistoryMenu() const;

	QTableView* getWorksheetTableView() const;

	void showTabData();
	void showTabConsole();

	void addRequestHistoryItem(const QString& szItemText);
	void clearRequestHistoryItems();

private:
	QToolBar* makeWorksheetToolbar();

private:
	QPushButton *m_pExecuteButton;
	QPushButton *m_pReformatButton;
	QPushButton *m_pClearTextButton;
	QPushButton* m_pRequestHistoryButton;
	QMenu* m_pRequestHistoryMenu;
	QSqlSourceView* m_pWorksheetTextEdit;
	QQueryResultView* m_pQueryResultView;
};

#endif /* SRC_GUI_QDATABASEWORKSHEETVIEW_H_ */
