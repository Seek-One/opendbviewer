/*
 * QDatabaseWorksheetView.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include "QDatabaseWorksheetView.h"

QDatabaseWorksheetView::QDatabaseWorksheetView(QWidget* parent)
		: QWidget(parent)
{
	QVBoxLayout *pWorksheetViewLayout = new QVBoxLayout;
	setLayout(pWorksheetViewLayout);

	QToolBar *pWorksheetToolbar = makeWorksheetToolbar();
	pWorksheetViewLayout->addWidget(pWorksheetToolbar);

	QTextEdit *pWorksheetTextEdit = new QTextEdit();
	pWorksheetViewLayout->addWidget(pWorksheetTextEdit);

	//Creation of the worksheet view
	QTableView *pWorksheetTableView = new QTableView();
	pWorksheetViewLayout->addWidget(pWorksheetTableView);
}


QDatabaseWorksheetView::~QDatabaseWorksheetView()
{

}

QToolBar* QDatabaseWorksheetView::makeWorksheetToolbar()
{
	QToolBar *pWorksheetToolbar = new QToolBar();

	QPushButton *pButton1 = new QPushButton("Execute");
	pWorksheetToolbar->addWidget(pButton1);

	QPushButton *pButton2 = new QPushButton("Reformat");
	pWorksheetToolbar->addWidget(pButton2);

	QPushButton *pButton3 = new QPushButton("Clear");
	pWorksheetToolbar->addWidget(pButton3);

	return pWorksheetToolbar;
}
