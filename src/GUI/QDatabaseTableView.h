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
#include <QTableView>
#include <QTextEdit>
#include <QTableView>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTreeView>

class QDatabaseTableView : public QWidget
{
	Q_OBJECT
public:
	QDatabaseTableView(QWidget* parent = 0);
	virtual ~QDatabaseTableView();
	QStandardItemModel* getStructureModel() const;
	QStandardItemModel* getDataResultsModel() const;
	QPushButton* getRefreshButton() const;
	QPushButton* getClearButton() const;
	QLineEdit* getFilterLine() const;
	QTextEdit* getConsoleTextEdit() const;
	QTextEdit* getCreationScriptTextEdit() const;



private:
	QWidget* makeDataTab();
	QWidget* makeStructureTab();
	QWidget* makeCreationScriptTab();

private:
	QStandardItemModel* m_pDataResultsModel;
	QStandardItemModel* m_pStructureModel;
	QPushButton *m_pRefreshButton;
	QPushButton *m_pClearButton;
	QLineEdit* m_pFilterLine;
	QTextEdit* m_pConsoleTextEdit;
	QTextEdit* m_pCreationScriptTextEdit;
};



#endif /* SRC_GUI_QDATABASETABLEVIEW_H_ */
