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

class QDatabaseTableView : public QWidget
{
	Q_OBJECT
public:
	QDatabaseTableView(QWidget* parent = 0);
	virtual ~QDatabaseTableView();

	QStandardItemModel* getStructureModel() const;
	QTableView* getStructureTable() const;

private:
	QWidget* makeDataTab();
	QWidget* makeStructureTab();
	QWidget* makeCreationScriptTab();

private:
	QTableView* m_pStructureTable;
	QStandardItemModel* m_pStructureModel;
};



#endif /* SRC_GUI_QDATABASETABLEVIEW_H_ */
