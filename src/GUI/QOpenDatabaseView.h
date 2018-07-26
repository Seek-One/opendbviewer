/*
 * QOpenDatabaseView.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QOPENDATABASEVIEW_H_
#define SRC_GUI_QOPENDATABASEVIEW_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

#include "Model/DatabaseModel.h"
#include "Widget/QDropAreaWidget.h"

class QLineEdit;
class QTabWidget;

class QOpenDatabaseView : public QWidget
{
	Q_OBJECT

public:
	QOpenDatabaseView(QWidget* parent = 0);
	virtual ~QOpenDatabaseView();

	QStackedWidget* getStackedMenuConnWidget() const;

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

	QPushButton* getSQLiteButton() const;
	QPushButton* getMySQLConnectButton() const;
	QPushButton* getPostgreSQLConnectButton() const;

	//Menu
	QPushButton* getSQLiteSelection() const;
	QPushButton* getMySQLSelection() const;
	QPushButton* getPostgreSQLSelection() const;

	QWidget* makeNewConnMenu(QWidget* pParent);
	QWidget* makeSQLiteTab(QWidget* pParent);
	QWidget* makeMySQLTab(QWidget* pParent);
	QWidget* makePostgreSQLTab(QWidget* pParent);

	void openNewConnMenuTab();
	void openSQLiteTab();
	void openMySQLTab();
	void openPostgreSQLTab();

public slots:
	void dispatchClicked();

signals :
	void clicked(DatabaseModel::DatabaseType type);

private:
	QStackedWidget* m_pStackedMenuConnWidget;
	QWidget* m_pNewConnWidget;
	QWidget* m_pSQLiteWidget;
	QWidget* m_pMySQLWidget;
	QWidget* m_pPSQLWidget;

	// SQLite
	QLineEdit* m_pSQLiteFilePathField;
	QPushButton* m_pSQLiteFileSelectionButton;
	QDropAreaWidget* m_pDropAreaWidget;

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
	QPushButton* m_pSQLiteButton;
	QPushButton* m_pMySQLConnectButton;
	QPushButton* m_pPostgreSQLConnectButton;

	//Selection Button
	QPushButton* m_pSQLiteSelection;
	QPushButton* m_pMySQLSelection;
	QPushButton* m_pPSQLSelection;
};

#endif /* SRC_GUI_QOPENDATABASEVIEW_H_ */
