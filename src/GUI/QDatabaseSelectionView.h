/*
 * QDatabaseSelectionView.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUI_QDATABASESELECTIONVIEW_H_
#define SRC_GUI_QDATABASESELECTIONVIEW_H_

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QToolBar>
#include <QLabel>
#include <QAction>
#include <QLineEdit>

class QDatabaseSelectionView : public QWidget
{
	Q_OBJECT
public:
	QDatabaseSelectionView(QWidget* parent = 0);
	virtual ~QDatabaseSelectionView();
	QPushButton* getFileSelectionButton() const;
	QPushButton* getCancelButton() const;
	QPushButton* getOKButton() const;
	QLineEdit* getFilePathField() const;
	QLineEdit* getHostField() const;
	QLineEdit* getPortField() const;
	QLineEdit* getUsernameField() const;
	QLineEdit* getPasswordField() const;
	QLineEdit* getDatabaseField() const;
	QTabWidget* getFileSelectionTabWidget() const;

private:
	QWidget* makeSqliteTab();
	QWidget* makeMySqlTab();

private:
	QPushButton* m_pFileSelectionButton;
	QPushButton* m_pOKButton;
	QPushButton* m_pCancelButton;
	QLineEdit* m_pFilePathField;
	QLineEdit* m_pHostField;
	QLineEdit* m_pPortField;
	QLineEdit* m_pUsernameField;
	QLineEdit* m_pPasswordField;
	QLineEdit* m_pDatabaseField;
	QTabWidget* m_pFileSelectionTabWidget;
};

#endif /* SRC_GUI_QDATABASESELECTIONVIEW_H_ */
