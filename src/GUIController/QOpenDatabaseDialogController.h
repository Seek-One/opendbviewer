/*
 * QDatabaseSelectionViewController.h
 *
 *  Created on: 10 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QOPENDATABASEDIALOGCONTROLLER_H_
#define SRC_GUICONTROLLER_QOPENDATABASEDIALOGCONTROLLER_H_

#include <QObject>
#include <QString>

class QWindowMain;
class QOpenDatabaseDialog;
class DatabaseController;
class QDatabaseConnectionViewController;

class QOpenDatabaseDialogController : public QObject
{
	Q_OBJECT
public:
	QOpenDatabaseDialogController();
	virtual ~QOpenDatabaseDialogController();

	void init(QWindowMain* pMainWindow, QOpenDatabaseDialog* pOpenDatabaseDialog);
	QString getFileUrl() const;

private:
	QStringList makeMySQLDatabaseInfoList();
	QStringList makePostgreSQLDatabaseInfoList();

public slots:
	void openFileDialog();
	void openFile(const QString& szFileUrl);
	void closeOpenDatabaseDialog();
	void loadDatabase();

private:
	QOpenDatabaseDialog* m_pOpenDatabaseDialog;
	QString m_szFileUrl;
	QWindowMain* m_pMainWindow;
};

#endif /* SRC_GUICONTROLLER_QOPENDATABASEDIALOGCONTROLLER_H_ */
