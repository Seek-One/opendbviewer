/*
 * QDatabaseSelectionViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QDATABASESELECTIONVIEWCONTROLLER_H_
#define SRC_GUICONTROLLER_QDATABASESELECTIONVIEWCONTROLLER_H_

#include <QObject>
#include <QString>

class QWindowMain;
class QOpenDatabaseDialog;
class DatabaseController;
class QDatabaseConnectionViewController;

class QDatabaseSelectionViewController : public QObject
{
	Q_OBJECT
public:
	QDatabaseSelectionViewController();
	virtual ~QDatabaseSelectionViewController();

	void init(QWindowMain* pMainWindow, QOpenDatabaseDialog* pOpenDatabaseDialog);
	QString getFileName() const;

private:
	QStringList makeDatabaseInfoList();

public slots:
	void openFileDialog();
	void closeSelectionWindow();
	void loadDatabase();

private:
	QOpenDatabaseDialog* m_pOpenDatabaseDialog;
	QString m_fileName;
	QWindowMain* m_pMainWindow;
	DatabaseController* m_pDatabaseController;
	QDatabaseConnectionViewController* m_pDatabaseConnectionViewController;
};

#endif /* SRC_GUICONTROLLER_QDATABASESELECTIONVIEWCONTROLLER_H_ */
