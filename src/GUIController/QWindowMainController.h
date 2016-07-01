/*
 * QWindowMainController.h
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_
#define SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_

#include "QWindowMainController.h"
#include "GUI/QWindowMain.h"
#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QDatabaseSelectionView.h"

#include <QMessageBox>
#include <QDialog>
#include <QFileDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QObject>

class QWindowMain;

class QWindowMainController : public QObject
{
	Q_OBJECT
public:
	QWindowMainController();
	virtual ~QWindowMainController();

	void init(QWindowMain* pMainWindow);


private:
	QWidget* makeTab();

public slots:
	void newDatabaseConnection();
	void about();
	void closeConnectionTab(const int& index);
	//void openFileSelectionWindow();

private:
	QWindowMain* m_pMainWindow;
};

#endif /* SRC_GUICONTROLLER_QWINDOWMAINCONTROLLER_H_ */
