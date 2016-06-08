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
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>

class QDatabaseTableView : public QWidget
{
public:
	QDatabaseTableView(QWidget* parent = 0);
	virtual ~QDatabaseTableView();

private:
	QWidget* makeDataTab();
	QWidget* makeStructureTab();

private:
	QWidget *m_pTableTab2;

};



#endif /* SRC_GUI_QDATABASETABLEVIEW_H_ */
