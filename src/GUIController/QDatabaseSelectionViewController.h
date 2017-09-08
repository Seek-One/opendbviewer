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
class QDatabaseSelectionView;
class DatabaseController;
class QDatabaseConnectionViewController;

class QDatabaseSelectionViewController : public QObject
{
	Q_OBJECT
public:
	QDatabaseSelectionViewController();
	virtual ~QDatabaseSelectionViewController();

	void init(QWindowMain* pMainWindow, QDatabaseSelectionView* pDatabaseSelectionView);
	QString getFileName() const;

private:
	QStringList makeDatabaseInfoList();

public slots:
	void openFileDialog();
	void closeSelectionWindow();
	void loadDatabase();

private:
	QDatabaseSelectionView* m_pDatabaseSelectionView;
	QString m_fileName;
	QWindowMain* m_pMainWindow;
	DatabaseController* m_pDatabaseController;
	QDatabaseConnectionViewController* m_pDatabaseConnectionViewController;
};

#endif /* SRC_GUICONTROLLER_QDATABASESELECTIONVIEWCONTROLLER_H_ */
