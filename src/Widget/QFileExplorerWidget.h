/*
 * QFileExplorerWidget.h
 *
 *  Created on: 7 juin 2018
 *      Author: cdegeorgi
 */

#ifndef SRC_WIDGET_QFILEEXPLORERWIDGET_H_
#define SRC_WIDGET_QFILEEXPLORERWIDGET_H_

#include <QAbstractItemView>
#include <QFileSystemModel>
#include <QListView>
#include <QPushButton>
#include <QtCore>
#include <QtGui>
#include <QTreeView>
#include <QTreeWidget>
#include <QWidget>

#include "Widget/QDropAreaWidget.h"

class QFileExplorerWidget : public QWidget{
	Q_OBJECT

public:
	QFileExplorerWidget(QWidget* parent = 0);
	virtual ~QFileExplorerWidget();

	QDropAreaWidget* getDropArea();

public slots:
	void onFileTreeViewDoubleClicked(QModelIndex index);
	void onFolderTreeViewDoubleClicked(QModelIndex index);
	void onOpenDatabaseButtonClicked();

private:
	QWidget *makeFileExplorerDropAreaWidget(QWidget* pParent, QString szHomeUserPath);

private:
	QFileSystemModel *dirModel;
	QFileSystemModel *fileModel;

	QTreeView *m_pFolderTreeView;
	QTreeView *m_pFileTreeView;

	QDropAreaWidget *m_pDropArea;

	QWidget *m_pFileExplorerDropAreaWidget;

	QPushButton* m_pOpenDatabaseButton;

signals:
	void openDatabase(QString szFilePath);
	void openSelectedFile(QString szFilePath);
};

#endif /* SRC_WIDGET_QFILEEXPLORERWIDGET_H_ */
