/*
 * QDatabaseTableView.h
 *
 *  Created on: 8 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASETABLEVIEW_H_
#define SRC_GUI_QDATABASETABLEVIEW_H_

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QDebug>

#include "QQueryResultView.h"

class QTreeView;
class QTableView;

class QDatabaseTableView : public QWidget
{
	Q_OBJECT
public:
	QDatabaseTableView(QWidget* parent = 0);
	virtual ~QDatabaseTableView();

	// Structure tab
	QTreeView* getStructureTreeView() const;
	QStandardItemModel* getStructureModel() const;

	// Data tab
	QTableView* getDataTableView() const;
	QPushButton* getRefreshButton() const;
	QPushButton* getClearButton() const;
	QLineEdit* getFilterLine() const;
	QTextEdit* getConsoleTextEdit() const;
	void showTabData();
	void showTabConsole();

	// Creation script tab
	QTextEdit* getCreationScriptTextEdit() const;

private:
	QWidget* makeDataTab();
	QWidget* makeStructureTab();
	QWidget* makeCreationScriptTab();

private slots:
	void filterLineTextChanged(const QString& szText);

private:
	// Structure tab
	QStandardItemModel* m_pStructureModel;
	QTreeView* m_pStructureTreeView;

	// Data tab
	QPushButton *m_pRefreshButton;
	QPushButton *m_pClearButton;
	QLineEdit* m_pFilterLine;
	QQueryResultView *m_pQueryResultView;

	// Creation script tab
	QTextEdit* m_pCreationScriptTextEdit;
};



#endif /* SRC_GUI_QDATABASETABLEVIEW_H_ */
