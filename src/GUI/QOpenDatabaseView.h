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

#include "Model/DatabaseModel.h"
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

	QPushButton* getSQLiteButton() const;
	QPushButton* getMySQLConnectButton() const;
	QPushButton* getPostgreSQLConnectButton() const;

	QPushButton* getSQLiteSelection() const;
	QPushButton* getMySQLSelection() const;
	QPushButton* getPostgreSQLSelection() const;

	QFileExplorerWidget* getFileExplorerWidget() const;

	QTreeWidget* getHistoryTreeWidget() const;
	QWidget* getHistoryInfoWidget() const;
	QLabel* getHistoryNameLabel() const;
	QLabel* getHistoryPathLabel() const;

	QWidget* makeNoSelectionTab(QWidget* pParent);
	QWidget* makeHistoryTab(QWidget* pParent);
	QWidget* makeHistoryInfo(QWidget* pParent);
	QWidget* makeExplorerTab(QWidget* pParent);
	QWidget* makeNewConnMenu(QWidget* pParent);
	QWidget* makeSQLiteTab(QWidget* pParent);
	QWidget* makeMySQLTab(QWidget* pParent);
	QWidget* makePostgreSQLTab(QWidget* pParent);

public slots:
	void dispatchClicked();
	void onHistoryItemClicked(QTreeWidgetItem *item, int column);
	void onHistoryTreeWidgetDoubleClicked(QTreeWidgetItem *item, int column);

signals :
	void clicked(DatabaseModel::DatabaseType type);
	void openHistorySQLiteDatabase(QString szPath);
	void openHistoryInfo(QString szPath);

private:
	QTabWidget* m_pConnectionTypeTabWidget;

	// Explorer
	QFileExplorerWidget *m_pFileExplorerWidget;

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

	//History Widgets
	QTreeWidget* m_pHistoryTreeWidget;
	QWidget* m_pHistoryInfoWidget;

	QLabel* m_pHistoryNameLabel;
	QLabel* m_pHistoryPathLabel;
	QLabel* m_pHistoryHostLabel;
	QLabel* m_pHistoryPortLabel;
	QLabel* m_pHistoryUsernameLabel;
};

#endif /* SRC_GUI_QOPENDATABASEVIEW_H_ */
