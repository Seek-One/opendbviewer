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
#include <QComboBox>

#include "Model/DatabaseModel.h"

class QLineEdit;
class QTabWidget;

class QOpenDatabaseView : public QWidget
{
	Q_OBJECT

public:
	QOpenDatabaseView(QWidget* parent);
	virtual ~QOpenDatabaseView();

	// SQLite
	QPushButton* getSQLiteFileSelectionButton() const;
	QLineEdit* getSQLiteFilePathField() const;

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

	//Common
	QPushButton* getValidationButton() const;
	QComboBox* getComboBoxSelection() const;

	void openSQLiteMenu();
	void openMySQLMenu();
	void openPostgreSQLMenu();
	void hideMenus();

public slots:
	void dispatchClicked();

signals :
	void clicked(DatabaseModel::DatabaseType type);

private:
	QWidget* makeSQLiteWidget();
	QWidget* makeMySQLWidget();
	QWidget* makePostgreSQLWidget();

private:
	QWidget* m_pSQLiteWidget;
	QWidget* m_pMySQLWidget;
	QWidget* m_pPSQLWidget;
	QComboBox* m_pComboBoxSelection;

	// SQLite
	QLineEdit* m_pSQLiteFilePathField;
	QPushButton* m_pSQLiteFileSelectionButton;

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
	QPushButton* m_pValidationButton;
};

#endif /* SRC_GUI_QOPENDATABASEVIEW_H_ */
