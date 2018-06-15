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
#include <QtCore>
#include <QtGui>
#include <QTreeView>
#include <QTreeWidget>
#include <QWidget>

class QFileExplorerWidget : public QWidget{
	Q_OBJECT

public:
	QFileExplorerWidget(QWidget* parent = 0);
	virtual ~QFileExplorerWidget();

public slots:
	void onFolderTreeViewDoubleClicked(QModelIndex index);
	void onFileTreeViewDoubleClicked(QModelIndex index);

	signals:
	void openSelectedFile(QString szFilePath);

private:
	QFileSystemModel *dirModel;
	QFileSystemModel *fileModel;

	QTreeView *m_pFolderTreeView;
	QTreeView *m_pFileTreeView;

};

#endif /* SRC_WIDGET_QFILEEXPLORERWIDGET_H_ */