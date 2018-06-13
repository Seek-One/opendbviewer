/*
 * QFileExplorerWidget.cpp
 *
 *  Created on: 7 juin 2018
 *      Author: cdegeorgi
 */

#include <QHeaderView>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "GUIController/QOpenDatabaseViewController.h"
#include "Widget/QFileExplorerWidget.h"

QFileExplorerWidget::QFileExplorerWidget(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	this->setLayout(pMainLayout);


	dirModel = new QFileSystemModel(this);
	dirModel->setRootPath(QDir::currentPath());
	dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

	fileModel = new QFileSystemModel(this);
	fileModel->setRootPath(QDir::currentPath());
	fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    m_pFolderTreeView = new QTreeView();
    m_pFolderTreeView->setModel(dirModel);
    m_pFolderTreeView->setColumnHidden(1, true);
    m_pFolderTreeView->setColumnHidden(2, true);
    m_pFolderTreeView->setColumnHidden(3, true);

    m_pFileTreeView = new QTreeView();
    m_pFileTreeView->setModel(fileModel);
    m_pFileTreeView->setDragEnabled(true);
    m_pFileTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pFileTreeView->setItemsExpandable(false);
    m_pFileTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pFileTreeView->setRootIsDecorated(false);
    m_pFileTreeView->setAlternatingRowColors(true);

    QSplitter *pSplitter = new QSplitter();

    pSplitter->addWidget(m_pFolderTreeView);
    pSplitter->addWidget(m_pFileTreeView);

	pMainLayout->addWidget(pSplitter);

	connect(m_pFolderTreeView, SIGNAL(clicked(const QModelIndex)), this, SLOT(onTreeViewClicked(const QModelIndex)));
	connect(m_pFileTreeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(onTreeViewDoubleClicked(const QModelIndex)));
}

QFileExplorerWidget::~QFileExplorerWidget()
{

}

void QFileExplorerWidget::onTreeViewClicked(QModelIndex index)
{
	QString szPath = dirModel->fileInfo(index).absoluteFilePath();
    m_pFileTreeView->setRootIndex(fileModel->setRootPath(szPath));
}

void QFileExplorerWidget::onTreeViewDoubleClicked(QModelIndex index)
{
	QString szTypeSelectedFile = fileModel->type(index);

	if (szTypeSelectedFile == "Folder")
	{
		QString szFolderPath = fileModel->filePath(index);
		QString szPath = szFolderPath;

		m_pFileTreeView->setRootIndex(fileModel->setRootPath(szPath));
	}

	if (szTypeSelectedFile.contains("File"))
	{
		QString szFilePath = fileModel->filePath(index);
		if(szFilePath.endsWith(".sqlite") || szFilePath.endsWith(".db"))
		{
			emit openSelectedFile(szFilePath);
		}
	}
}
