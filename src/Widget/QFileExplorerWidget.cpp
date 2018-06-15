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
	dirModel->setRootPath(QDir::homePath());
	dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	QString szHomeUserPath = dirModel->rootPath();

	fileModel = new QFileSystemModel(this);
	fileModel->setRootPath(QDir::currentPath());
	fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    m_pFolderTreeView = new QTreeView();
    m_pFolderTreeView->setModel(dirModel);
    m_pFolderTreeView->setColumnHidden(1, true);
    m_pFolderTreeView->setColumnHidden(2, true);
    m_pFolderTreeView->setColumnHidden(3, true);
    m_pFolderTreeView->expandAll();

    m_pFileTreeView = new QTreeView();
    m_pFileTreeView->setModel(fileModel);
    m_pFileTreeView->setDragEnabled(true);
    m_pFileTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pFileTreeView->setItemsExpandable(false);
    m_pFileTreeView->setRootIsDecorated(false);
    m_pFileTreeView->setAlternatingRowColors(true);
    m_pFileTreeView->setRootIndex(fileModel->setRootPath(szHomeUserPath));

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

    	m_pFileTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

#else

    	m_pFileTreeView->header()->setResizeMode(QHeaderView::ResizeToContents);

#endif

    QSplitter *pSplitter = new QSplitter();

    pSplitter->addWidget(m_pFolderTreeView);
    pSplitter->addWidget(m_pFileTreeView);

	pMainLayout->addWidget(pSplitter);

	connect(m_pFolderTreeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(onFolderTreeViewDoubleClicked(const QModelIndex)));
	connect(m_pFileTreeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(onFileTreeViewDoubleClicked(const QModelIndex)));
}

QFileExplorerWidget::~QFileExplorerWidget()
{

}

void QFileExplorerWidget::onFolderTreeViewDoubleClicked(QModelIndex index)
{
	QString szPath = dirModel->fileInfo(index).absoluteFilePath();
    m_pFileTreeView->setRootIndex(fileModel->setRootPath(szPath));
}

void QFileExplorerWidget::onFileTreeViewDoubleClicked(QModelIndex index)
{
	if (fileModel->isDir(index))
	{
		QString szFolderPath = fileModel->filePath(index);
		m_pFileTreeView->setRootIndex(fileModel->setRootPath(szFolderPath));

		QModelIndex folderIndex = dirModel->index(szFolderPath);
		QModelIndex parentIndex = dirModel->parent(folderIndex);

		m_pFolderTreeView->expand(folderIndex);

		m_pFolderTreeView->setCurrentIndex(folderIndex);


		if(m_pFolderTreeView->isExpanded(parentIndex) == false)
		{
			m_pFolderTreeView->expand(parentIndex);
		}
	}

	else
	{
		QString szFilePath = fileModel->filePath(index);
		if(szFilePath.endsWith(".sqlite") || szFilePath.endsWith(".db"))
		{
			emit openSelectedFile(szFilePath);
		}
	}
}