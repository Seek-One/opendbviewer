/*
 * QOpenDatabaseView.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QOPENDATABASEVIEW_H_
#define SRC_GUI_QOPENDATABASEVIEW_H_

#include <QWidget>

#include "Widget/QDropAreaWidget.h"
#include "Widget/QFileExplorerWidget.h"

class QPushButton;
class QLineEdit;
class QTabWidget;

class QOpenDatabaseView : public QWidget
{
	Q_OBJECT
public:
	QOpenDatabaseView(QWidget* parent = 0);
	virtual ~QOpenDatabaseView();

	QTabWidget* getConnectionTypeTabWidget() const;

	// SQLite
	QPushButton* getSQLiteFileSelectionButton() const;
	QLineEdit* getSQLiteFilePathField() const;

	QDropAreaWidget* getDropAreaWidget() const;

	// MySQL
	QLineEdit* getMySQLHostField() const;
	QLineEdit* getMySQLPortField() const;
	QLineEdit* getMySQLUsernameField() const;
	QLineEdit* getMySQLPasswordField() const;
	QLineEdit* getMySQLDatabaseField() const;

	// PostgreSQL
	QLineEdit* getPSQLHostField() const;
	QLineEdit* getPSQLPortField() const;
	QLineEdit* getPSQLUsernameField() const;
	QLineEdit* getPSQLPasswordField() const;
	QLineEdit* getPSQLDatabaseField() const;

	QPushButton* getCancelButton() const;
	QPushButton* getOKButton() const;

	QFileExplorerWidget* getFileExplorerWidget() const;

private:
	QWidget* makeSQLiteTab(QWidget* pParent);
	QWidget* makeMySQLTab(QWidget* pParent);
	QWidget* makePostgreSQLTab(QWidget* pParent);


private:
	QTabWidget* m_pConnectionTypeTabWidget;

	// SQLite
	QLineEdit* m_pSQLiteFilePathField;
	QPushButton* m_pSQLiteFileSelectionButton;
	QDropAreaWidget* m_pDropAreaWidget;
	QFileExplorerWidget *m_pFileExplorerWidget;

	// MySQL
	QLineEdit* m_pMySQLHostField;
	QLineEdit* m_pMySQLPortField;
	QLineEdit* m_pMySQLUsernameField;
	QLineEdit* m_pMySQLPasswordField;
	QLineEdit* m_pMySQLDatabaseField;

	// PostgreSQL
	QLineEdit* m_pPSQLHostField;
	QLineEdit* m_pPSQLPortField;
	QLineEdit* m_pPSQLUsernameField;
	QLineEdit* m_pPSQLPasswordField;
	QLineEdit* m_pPSQLDatabaseField;

	// Validation button
	QPushButton* m_pOKButton;
	QPushButton* m_pCancelButton;

};

#endif /* SRC_GUI_QOPENDATABASEVIEW_H_ */
