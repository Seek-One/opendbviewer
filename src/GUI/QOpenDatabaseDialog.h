/*
 * QOpenDatabaseDialog.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QOPENDATABASEDIALOG_H_
#define SRC_GUI_QOPENDATABASEDIALOG_H_

#include <QDialog>

class QPushButton;
class QLineEdit;
class QTabWidget;

class QOpenDatabaseDialog : public QDialog
{
	Q_OBJECT
public:
	QOpenDatabaseDialog(QWidget* parent = 0);
	virtual ~QOpenDatabaseDialog();

	QTabWidget* getConnectionTypeTabWidget() const;

	// SQLite
	QPushButton* getSQLiteFileSelectionButton() const;
	QLineEdit* getSQLiteFilePathField() const;

	// MySQL
	QLineEdit* getMySQLHostField() const;
	QLineEdit* getMySQLPortField() const;
	QLineEdit* getMySQLUsernameField() const;
	QLineEdit* getMySQLPasswordField() const;
	QLineEdit* getMySQLDatabaseField() const;

	QPushButton* getCancelButton() const;
	QPushButton* getOKButton() const;

private:
	QWidget* makeSQLiteTab(QWidget* pParent);
	QWidget* makeMySQLTab(QWidget* pParent);

private:
	QTabWidget* m_pConnectionTypeTabWidget;

	// SQLite
	QLineEdit* m_pSQLiteFilePathField;
	QPushButton* m_pSQLiteFileSelectionButton;

	// MySQL
	QLineEdit* m_pMySQLHostField;
	QLineEdit* m_pMySQLPortField;
	QLineEdit* m_pMySQLUsernameField;
	QLineEdit* m_pMySQLPasswordField;
	QLineEdit* m_pMySQLDatabaseField;

	// Validation button
	QPushButton* m_pOKButton;
	QPushButton* m_pCancelButton;
};

#endif /* SRC_GUI_QOPENDATABASEDIALOG_H_ */
