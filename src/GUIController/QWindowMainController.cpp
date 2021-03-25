/*
 * QWindowMainController.cpp
 *
 *  Created on: 9 juin 2016
 *      Author: echopin
 */

#include <QFile>
#include <QTextStream>

#include "Database/DatabaseController.h"
#include "Database/DatabaseControllerMysql.h"
#include "Database/DatabaseControllerPostgreSQL.h"
#include "Database/DatabaseControllerSqlite.h"

#include "Global/ApplicationSettings.h"

#include "GUI/QDatabaseConnectionView.h"
#include "GUI/QAboutDialog.h"
#include "GUI/QExportParametersDialog.h"
#include "GUI/QOpenHistoryView.h"
#include "GUI/QProgressBarDialog.h"
#include "GUI/QWindowMain.h"

#include "GUIController/QDatabaseConnectionViewController.h"
#include "GUIController/QWindowMainController.h"

QWindowMainController::QWindowMainController()
{
	m_pMainWindow = NULL;
	m_pOpenDatabaseViewController = NULL;
	m_pOpenHistoryViewController = NULL;
}

QWindowMainController::~QWindowMainController()
{
	if(m_pOpenDatabaseViewController)
	{
		delete m_pOpenDatabaseViewController;
		m_pOpenDatabaseViewController = NULL;
	}
	if(m_pOpenHistoryViewController)
	{
		delete m_pOpenHistoryViewController;
		m_pOpenHistoryViewController = NULL;
	}
}

void QWindowMainController::init(QWindowMain* pMainWindow)
{
	m_pMainWindow = pMainWindow;
	m_pOpenDatabaseViewController = new QOpenDatabaseViewController(pMainWindow, this);
	m_pOpenHistoryViewController = new QOpenHistoryViewController(pMainWindow, this);

    //Top Menu Bar
    connect(m_pMainWindow->getAboutAction(), SIGNAL(triggered()), this, SLOT(about()));
    connect(m_pMainWindow->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(m_pMainWindow->getDatabaseConnectionTab(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeDatabaseConnectionTab(int)));

    connect(m_pMainWindow, SIGNAL(dragEnterTriggered(QDragEnterEvent*)), this, SLOT(dragEnterReceived(QDragEnterEvent*)));
    connect(m_pMainWindow, SIGNAL(dragLeaveTriggered(QDragLeaveEvent*)), this, SLOT(dragLeaveReceived(QDragLeaveEvent*)));
    connect(m_pMainWindow, SIGNAL(dropTriggered(QDropEvent*)), this, SLOT(dropReceived(QDropEvent*)));

    //File Explorer Buttons
	connect(m_pMainWindow->getFileExplorerWidget(), SIGNAL(openSelectedFile(const QString&)), this, SLOT(callSQLiteFile(const QString&)));
	connect(m_pMainWindow->getFileExplorerWidget(), SIGNAL(openDatabase(const QString&)), this, SLOT(callSQLiteFile(const QString&)));

	//Main ToolBar Slots
	connect(m_pMainWindow->getViewsAction(), SIGNAL(triggered()), this, SLOT(openViews()));
	connect(m_pMainWindow->getHistAction(), SIGNAL(triggered()), this, SLOT(openHist()));
	connect(m_pMainWindow->getExplorerAction(), SIGNAL(triggered()), this, SLOT(openExplorer()));
	connect(m_pMainWindow->getNewConnAction(), SIGNAL(triggered()), this, SLOT(openMenuConn()));
}

void QWindowMainController::closeDatabaseConnectionTab(const int& index)
{
	QWidget* pTabItem = m_pMainWindow->getDatabaseConnectionTab()->widget(index);
	if(pTabItem){
		m_pMainWindow->getDatabaseConnectionTab()->removeTab(index);
		delete pTabItem;
	}
	if (m_pMainWindow->getDatabaseConnectionTab()->count() == 0) {
		m_pMainWindow->getStackedDatabaseWidget()->setCurrentWidget(m_pMainWindow->getNoSelectionWidget());
	}
}

void QWindowMainController::about()
{
	QAboutDialog aboutDialog(m_pMainWindow);
	aboutDialog.exec();
}

void QWindowMainController::openViews()
{
	m_pMainWindow->showViewsTab();
}

void QWindowMainController::openHist()
{
	m_pMainWindow->showHistoryTab();
}

void QWindowMainController::openExplorer()
{
	m_pMainWindow->showExplorerTab();
}

void QWindowMainController::openMenuConn()
{
	m_pMainWindow->showMenuConnTab();
}

void QWindowMainController::callHistoryDatabaseLoading(const DatabaseModel& databaseModel)
{
	m_pOpenDatabaseViewController->loadHistoryDatabase(databaseModel);
}

void QWindowMainController::callSQLiteFile(const QString& szFileUrl)
{
	m_pOpenDatabaseViewController->openSQLiteFile(szFileUrl);
}

void QWindowMainController::callInitHistoryList()
{
	m_pOpenHistoryViewController->initHistoryList();
}

bool QWindowMainController::saveSQLResultsToCSV(QSqlQueryModel* model, QWidget* parent, Qt::Orientation orientation, QString& szErrorMsg)
{
	QString szFilePath;

	QString szFieldSeparator;
	QString szStringSeparator;
	QString szLineBreakSeparator;
	bool bIncludesHeaders = true;

	bool bGoOn = true;

	// Open the parameters dialog
	QExportParametersDialog dialogExportParams(parent);
	int iRes = dialogExportParams.exec();
	if(iRes == QDialog::Accepted)
	{
		szFieldSeparator = dialogExportParams.getTextFieldSeparator();
		szStringSeparator = dialogExportParams.getTextStringSeparator();
		szLineBreakSeparator = dialogExportParams.getTextLineBreakSeparator();
		szFilePath = dialogExportParams.getFilePath();
		bIncludesHeaders = dialogExportParams.isIncludesHeaders();
	}else{
		return true;
	}

	// Check if the path is not empty
	if(bGoOn)
	{
		if(szFilePath.replace(" ", "").isEmpty()) {
			szErrorMsg = tr("The file path is empty.");
			bGoOn = false;
		}
	}

	// Check if extension is file is not a directory
	if(bGoOn) {
		QFileInfo info(szFilePath);
		if(info.isDir())
		{
			szErrorMsg = tr("The file path is a directory.");
			bGoOn = false;
		}
	}

	if(bGoOn)
	{
		QProgressBarDialog* pProgressBarDialog = new QProgressBarDialog(parent, tr("Loading: exporting data"));
		pProgressBarDialog->show();

		connect(pProgressBarDialog, SIGNAL(rejected()), pProgressBarDialog, SLOT(setCancel()));

		// Perform the file write
		QFile fileToWrite(szFilePath);
		bool bGoOn = (fileToWrite.open(QFile::WriteOnly | QFile::Text));
		if(bGoOn)
		{
			QTextStream fileTextStream(&fileToWrite);
			QModelIndex index;

			// Load all the query model datas
			while(model->canFetchMore()){
				model->fetchMore();
			}

			int iColumnCount = model->columnCount();
			int iRowCount = model->rowCount();
			int iMaxData  = iRowCount * iColumnCount;

			if(!bIncludesHeaders){
				iMaxData -= iColumnCount;
			}

			pProgressBarDialog->setMaximumData(iMaxData);

			int iRange = iMaxData/100;
			int iDataHeaders = 0;

			if(bIncludesHeaders)
			{
				for(int i = 0; i < iColumnCount; i++)
				{
					fileTextStream << szStringSeparator;
					fileTextStream << model->headerData(i, orientation).toString();
					fileTextStream << szStringSeparator;
					if(i < iColumnCount-1){
						fileTextStream << szFieldSeparator;
					}
					iDataHeaders++;
				}
				fileTextStream << szLineBreakSeparator;
			}

			pProgressBarDialog->setData(iDataHeaders);

			int iCurrentData;

			for(int i = 0; i < iRowCount; i++)
			{
				for(int j = 0; j < iColumnCount; j++)
				{
					index = model->index(i, j);

					int iTypeRole = -1;
					QVariant typeRole = model->data(index, DataTypeRole);
					if(!typeRole.isNull()){
						iTypeRole = typeRole.toInt();
					}

					QString szDisplayText;
					if(iTypeRole != DataTypeNull){
						szDisplayText = model->data(index, Qt::DisplayRole).toString();
					}
					fileTextStream << szStringSeparator;
					// Escape quotes if the string separator is quotes
					if(szStringSeparator == "\""){
						szDisplayText.replace(szFieldSeparator, "\"\""+szFieldSeparator+"\"\"");
					}
					fileTextStream << szDisplayText;
					fileTextStream << szStringSeparator;
					if(j < iColumnCount-1){
						fileTextStream << szFieldSeparator;
					}

					iCurrentData = j + i*iColumnCount + iDataHeaders;

					if(iCurrentData % iRange == 0){
						pProgressBarDialog->setData(iCurrentData);
					}

					if(iCurrentData == iMaxData){
						pProgressBarDialog->setData(iCurrentData);
					}
				}

				if(i < iRowCount-1){
					fileTextStream << szLineBreakSeparator;
				}

				QApplication::processEvents();
				if(pProgressBarDialog->isCancel()) {
					fileToWrite.remove();
					break;
				}
			}
			pProgressBarDialog->close();
			fileToWrite.flush();
			fileToWrite.close();
		}
	}
	return bGoOn;
}

void QWindowMainController::dragEnterReceived(QDragEnterEvent* pEvent)
{
	const QMimeData* pMimeData = pEvent->mimeData();
	if(pMimeData){
		QList<QUrl> listUrls = pMimeData->urls();
		QList<QUrl>::iterator iter;
		QString szFileName;
		for(iter = listUrls.begin(); iter != listUrls.end(); ++iter){
			szFileName = (*iter).path();
			if(szFileName.endsWith(".sqlite") || szFileName.endsWith(".db")){
				pEvent->acceptProposedAction();
				m_pMainWindow->enableBlurEffect(true);
				m_pMainWindow->getDropArea()->startPaint();
				break;
			}
		}
	}
}

void QWindowMainController::dragLeaveReceived(QDragLeaveEvent* pEvent)
{
	m_pMainWindow->enableBlurEffect(false);
	m_pMainWindow->getDropArea()->stopPaint();
}

void QWindowMainController::dropReceived(QDropEvent* pEvent)
{
	m_pMainWindow->enableBlurEffect(false);
	const QMimeData* pMimeData = pEvent->mimeData();
	if(pMimeData){
		QList<QUrl> listUrls = pMimeData->urls();
		QList<QUrl>::iterator iter;
		QString szPath;
		for(iter = listUrls.begin(); iter != listUrls.end(); ++iter){
			szPath = (*iter).path();
			if(szPath.endsWith(".sqlite") || szPath.endsWith(".db")){
				callSQLiteFile(szPath);
			}
		}
	}
	m_pMainWindow->getDropArea()->stopPaint();
}
