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
	QPushButton* getFileSelectionButton() const;
	QLineEdit* getFilePathField() const;

	// MySQL
	QLineEdit* getHostField() const;
	QLineEdit* getPortField() const;
	QLineEdit* getUsernameField() const;
	QLineEdit* getPasswordField() const;
	QLineEdit* getDatabaseField() const;

	QPushButton* getCancelButton() const;
	QPushButton* getOKButton() const;

private:
	QWidget* makeSQLiteTab(QWidget* pParent);
	QWidget* makeMySQLTab(QWidget* pParent);

private:
	QTabWidget* m_pConnectionTypeTabWidget;

	// SQLite
	QLineEdit* m_pFilePathField;
	QPushButton* m_pFileSelectionButton;

	// MySQL
	QLineEdit* m_pHostField;
	QLineEdit* m_pPortField;
	QLineEdit* m_pUsernameField;
	QLineEdit* m_pPasswordField;
	QLineEdit* m_pDatabaseField;

	// Validation button
	QPushButton* m_pOKButton;
	QPushButton* m_pCancelButton;
};

#endif /* SRC_GUI_QOPENDATABASEDIALOG_H_ */
