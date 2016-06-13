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
#include <QFileDialog>
#include "GUI/QDatabaseSelectionView.h"

class QDatabaseSelectionView;

class QDatabaseSelectionViewController : public QObject
{
	Q_OBJECT
public:
	QDatabaseSelectionViewController();
	virtual ~QDatabaseSelectionViewController();

	void init(QDatabaseSelectionView* pDatabaseSelectionView);

public slots:
	void openFileDialog();
	void closeSelectionWindow();

private:
	QDatabaseSelectionView* m_pDatabaseSelectionView;
	QString m_pFileName;
};

#endif /* SRC_GUICONTROLLER_QDATABASESELECTIONVIEWCONTROLLER_H_ */
