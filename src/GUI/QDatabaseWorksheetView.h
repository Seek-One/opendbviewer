/*
 * QDatabaseWorksheetView.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASEWORKSHEETVIEW_H_
#define SRC_GUI_QDATABASEWORKSHEETVIEW_H_

#include "QDatabaseTableView.h"

#include <QPushButton>
#include <QToolBar>
#include <QTextEdit>
#include <QStandardItemModel>

class QDatabaseWorksheetView : public QWidget
{
public:
	QDatabaseWorksheetView(QWidget* parent = 0);
	virtual ~QDatabaseWorksheetView();
	QPushButton* getExecuteButton() const;
	QPushButton* getReformatButton() const;
	QPushButton* getClearTextButton() const;
	QTextEdit* getWorksheetTextEdit() const;
	QTextEdit* getWorksheetConsoleTextEdit() const;
	QStandardItemModel* getWorksheetResultsModel() const;

private:
	QToolBar* makeWorksheetToolbar();

private:
	QPushButton *m_pExecuteButton;
	QPushButton *m_pReformatButton;
	QPushButton *m_pClearTextButton;
	QTextEdit* m_pWorksheetTextEdit;
	QTextEdit* m_pWorksheetConsoleTextEdit;
	QStandardItemModel* m_pWorksheetResultsModel;
};

#endif /* SRC_GUI_QDATABASEWORKSHEETVIEW_H_ */
