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

#include "Global/ApplicationSettings.h"
#include "GUIController/QOpenDatabaseViewController.h"
#include "Settings/QSettingsManager.h"
#include "Widget/QFileExplorerWidget.h"

QFileExplorerWidget::QFileExplorerWidget(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	this->setLayout(pMainLayout);

	dirModel = new QFileSystemModel(this);
	dirModel->setRootPath(ApplicationSettings::getCurrentExplorerPath());
	dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	QString szHomeUserPath = dirModel->rootPath();

    m_pFolderTreeView = new QTreeView();
    m_pFolderTreeView->setModel(dirModel);
    m_pFolderTreeView->setColumnHidden(1, true);
    m_pFolderTreeView->setColumnHidden(2, true);
    m_pFolderTreeView->setColumnHidden(3, true);
    m_pFolderTreeView->expandAll();
    m_pFolderTreeView->setCurrentIndex(dirModel->index(ApplicationSettings::getCurrentExplorerPath()));


    m_pFileExplorerDropAreaWidget = makeFileExplorerDropAreaWidget(this, szHomeUserPath);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

    	m_pFileTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

#else

    	m_pFileTreeView->header()->setResizeMode(QHeaderView::ResizeToContents);

#endif

    QSplitter *pSplitter = new QSplitter();

    pSplitter->addWidget(m_pFolderTreeView);
    pSplitter->addWidget(m_pFileExplorerDropAreaWidget);

    pSplitter->setStretchFactor(1, 2);

	pMainLayout->addWidget(pSplitter);

	m_pOpenDatabaseButton = new QPushButton(tr("Open"));
	pMainLayout->addWidget(m_pOpenDatabaseButton, 1, Qt::AlignRight);

	connect(m_pFolderTreeView, SIGNAL(clicked(const QModelIndex)), this, SLOT(onFolderTreeViewClicked(const QModelIndex)));
	connect(m_pFileTreeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(onFileTreeViewDoubleClicked(const QModelIndex)));
	connect(m_pOpenDatabaseButton, SIGNAL(clicked()), this, SLOT(onOpenDatabaseButtonClicked()));
}

QFileExplorerWidget::~QFileExplorerWidget()
{

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

		ApplicationSettings::setCurrentExplorerPath(szFolderPath);
	    QSettingsManager::getInstance().saveConfigSettings();

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

void QFileExplorerWidget::onFolderTreeViewClicked(QModelIndex index)
{
	QString szPath = dirModel->fileInfo(index).absoluteFilePath();
    m_pFileTreeView->setRootIndex(fileModel->setRootPath(szPath));

    ApplicationSettings::setCurrentExplorerPath(szPath);
    QSettingsManager::getInstance().saveConfigSettings();
}

void QFileExplorerWidget::onOpenDatabaseButtonClicked()
{
	QModelIndexList indexList = m_pFileTreeView->selectionModel()->selectedRows();

	QList<QModelIndex>::iterator iter;
	QString szFilePath;
	for(iter = indexList.begin(); iter != indexList.end(); ++iter)
	{
		szFilePath = fileModel->filePath(*iter);
		if(szFilePath.endsWith(".sqlite") || szFilePath.endsWith(".db")){
			emit openDatabase(szFilePath);
		}
	}
}

QWidget* QFileExplorerWidget::makeFileExplorerDropAreaWidget(QWidget* pParent, QString szHomeUserPath)
{
	QWidget* pMainWidget = new QWidget(pParent);
	QVBoxLayout* pMainLayout = new QVBoxLayout();
	pMainWidget->setLayout(pMainLayout);
	pMainLayout->setContentsMargins(0, 0, 0, 0);

	fileModel = new QFileSystemModel(this);
	fileModel->setRootPath(QDir::currentPath());
	QStringList szfileTreeViewFilter = QStringList() << "*.db" << "*.sdb" << "*.sqlite" << "*.db3" << "*.s3db" << "*.sqlite3" << "*.sl3" << "*.db2" << "*.s2db" << "*.sqlite2" << "*.sl2";
	fileModel->setNameFilterDisables(false);
	fileModel->setNameFilters(szfileTreeViewFilter);

	m_pFileTreeView = new QTreeView();
	m_pFileTreeView->setModel(fileModel);
	m_pFileTreeView->setDragEnabled(true);
	m_pFileTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_pFileTreeView->setItemsExpandable(false);
	m_pFileTreeView->setRootIsDecorated(false);
	m_pFileTreeView->setAlternatingRowColors(true);
	m_pFileTreeView->setRootIndex(fileModel->setRootPath(szHomeUserPath));

	pMainLayout->addWidget(m_pFileTreeView, 3);

	return pMainWidget;
}
