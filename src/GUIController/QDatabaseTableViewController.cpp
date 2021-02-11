/*
 * QDatabaseTableViewController.cpp
 *
 *  Created on: 14 juin 2016
 *      Author: echopin
 */


#include <QApplication>
#include <QPalette>
#include <QTreeView>
#include <QTableView>
#include <QMessageBox>
#include <QSqlError>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QRadioButton>
#include <QDialogButtonBox>

#include "Database/DatabaseController.h"

#include "GUI/QDatabaseTableView.h"
#include "GUI/QExportParametersDialog.h"

#include "GUIController/QDatabaseTableViewController.h"

QDatabaseTableViewController::QDatabaseTableViewController()
{
	m_pDatabaseTableView = NULL;
	m_pDatabaseController = NULL;
	m_pDatabaseTableModel = NULL;
	m_szTableName = "";
}

QDatabaseTableViewController::~QDatabaseTableViewController()
{
	if (m_pDatabaseTableModel) {
		delete m_pDatabaseTableModel;
	}
}

void QDatabaseTableViewController::init(QDatabaseTableView* pDatabaseTableView, QString& szTableName, DatabaseController* pDatabaseController)
{
	m_pDatabaseTableView = pDatabaseTableView;
	m_szTableName = szTableName;
	m_pDatabaseController = pDatabaseController;
	m_pDatabaseTableModel = new QSqlDisplayTableModel(NULL, pDatabaseController->getSqlDatabase());

	connect(m_pDatabaseTableView->getRefreshButton(), SIGNAL(clicked()), this, SLOT(updateTableData()));
	connect(m_pDatabaseTableView->getClearButton(), SIGNAL(clicked()), this, SLOT(clearFilterField()));
	connect(m_pDatabaseTableView->getExportDataButton(), SIGNAL(clicked()), this, SLOT(exportData()));
	connect(m_pDatabaseTableModel, SIGNAL(databaseError()), this, SLOT(displayError()));
}

bool QDatabaseTableViewController::loadDatabaseTableInfos()
{
	bool bRes;

	// Load table structure
	bRes = m_pDatabaseController->loadTableDescription(m_szTableName, onDbLoadTableDescription, this);
	showQueryInformation();
	m_pDatabaseTableView->getStructureTreeView()->header()->resizeSections(QHeaderView::ResizeToContents);

	// Load table data
	bRes = loadDatabaseTableData() && bRes;
	
	// Load table creation script (if any)
	bRes = m_pDatabaseController->loadTableCreationScript(m_szTableName, onDbLoadTableCreationScript, this) && bRes;
	showQueryInformation();

	return bRes;
}

bool QDatabaseTableViewController::loadDatabaseTableData()
{
	bool bRes;
	int iDefaultSize = 20;
	QString szFilter = m_pDatabaseTableView->getFilterLine()->text();
	bRes = m_pDatabaseController->loadTableData(m_szTableName, szFilter, m_pDatabaseTableModel); //Load the data

	m_pDatabaseTableView->getDataTableView()->setModel(m_pDatabaseTableModel);
	m_pDatabaseTableView->getDataTableView()->setShowGrid(false);
	m_pDatabaseTableView->getDataTableView()->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
	m_pDatabaseTableView->getDataTableView()->horizontalHeader()->setStretchLastSection(true);
	m_pDatabaseTableView->getDataTableView()->verticalHeader()->setDefaultSectionSize(iDefaultSize);
	m_pDatabaseTableView->getDataTableView()->verticalHeader()->setHidden(true);
	m_pDatabaseTableView->getDataTableView()->sortByColumn(0, Qt::AscendingOrder);
	showQueryInformation();

	return bRes;
}

void QDatabaseTableViewController::updateTableData()
{
	if(loadDatabaseTableData()){
		m_pDatabaseTableView->showTabData();
	}else{
		m_pDatabaseTableView->showTabConsole();
	}
}

void QDatabaseTableViewController::clearFilterField()
{
	m_pDatabaseTableView->getFilterLine()->clear();
	updateTableData();
}

void QDatabaseTableViewController::exportData()
{
	bool bGoOn = true;
	QString szErrorMessage = "";

	QString szFilePath;

	QString szFieldSeparator;
	QString szStringSeparator;
	QString szLineBreakSeparator;
	bool bIncludesHeaders = true;

	// Open the parameters dialog
	QExportParametersDialog dialogExportParams;
	int iRes = dialogExportParams.exec();
	if(iRes == QDialog::Accepted)
	{
		szFieldSeparator = dialogExportParams.getTextFieldSeparator();
		szStringSeparator = dialogExportParams.getTextStringSeparator();
		szLineBreakSeparator = dialogExportParams.getTextLineBreakSeparator();
		szFilePath = dialogExportParams.getFilePath();
		bIncludesHeaders = dialogExportParams.isIncludesHeaders();

		// Check if the path is not empty
		if(szFilePath.replace(" ", "").isEmpty()) {
			bGoOn = false;
			szErrorMessage = tr("The file path is empty.");
		}

		// Check if extension is file is not a directory
		if(bGoOn)
		{
			QFileInfo info(szFilePath);
			if(info.isDir())
			{
				bGoOn = false;
				szErrorMessage = tr("The file path is a directory.");
			}
		}

		// Perform the file write
		if(bGoOn)
		{
			QFile fileToWrite(szFilePath);
			bGoOn = (fileToWrite.open(QFile::WriteOnly | QFile::Text));
			if(bGoOn)
			{
				QTextStream fileTextStream(&fileToWrite);
				QModelIndex index;

				int iColumnCount = m_pDatabaseTableModel->columnCount();

				if(bIncludesHeaders)
				{
					for(int i = 0; i < iColumnCount; i++)
					{
						fileTextStream << szStringSeparator;
						fileTextStream << m_pDatabaseTableModel->headerData(i, m_pDatabaseTableView->getStructureTreeView()->header()->orientation(), 0).toString();
						fileTextStream << szStringSeparator;
						if(i<iColumnCount-1){
							fileTextStream << szFieldSeparator;
						}
					}

					fileTextStream << szLineBreakSeparator;
				}

				for(int i = 0; i < m_pDatabaseTableModel->rowCount(); i++)
				{
					for(int j = 0; j < iColumnCount; j++)
					{
						index = m_pDatabaseTableModel->index(i, j);
						
						int iTypeRole = -1;
						QVariant typeRole = m_pDatabaseTableModel->data(index, DataTypeRole);
						if(!typeRole.isNull()){
							iTypeRole = typeRole.toInt();
						}

						QString szDisplayText;
						if(iTypeRole != DataTypeNull){
							szDisplayText = m_pDatabaseTableModel->data(index, Qt::DisplayRole).toString();
						}
						fileTextStream << szStringSeparator;
						fileTextStream << getEscapedText(szDisplayText, szFieldSeparator, szStringSeparator);
						fileTextStream << szStringSeparator;
						fileTextStream << szFieldSeparator;
					}
					fileTextStream << szLineBreakSeparator;
				}
				fileToWrite.flush();
				fileToWrite.close();
			}
			else{
				szErrorMessage = tr("The file cannot be open for writing.");
			}
		}

		if(!bGoOn){
			QMessageBox::critical(m_pDatabaseTableView, tr("Error"), tr("Unable to export the data into the file:") + "<br/>" + szErrorMessage);
		}
	}
}

QString QDatabaseTableViewController::getEscapedText(const QString& szData, const QString& szFieldSeparator, const QString& szStringSeparator) const
{
	if(szStringSeparator == "\""){
		QString szNewText = szData;
		szNewText.replace(szFieldSeparator, "\"\""+szFieldSeparator+"\"\"");
		return szNewText;
	}
	return szData;
}

void QDatabaseTableViewController::displayError()
{
	QMessageBox::warning(m_pDatabaseTableView, tr("Error"), tr("Error: <br/><b>%1</b>").arg(m_pDatabaseTableModel->lastError().text()));
}

QList<QStandardItem*> QDatabaseTableViewController::makeStandardItemListFromStringList(const QList<QString>& szStringList)
{
	QList<QStandardItem*> listStandardItemList;
	QStandardItem* pStandardItem;
	QFont font;

	QPalette palette = QApplication::palette(m_pDatabaseTableView);
	QBrush nullbrush = palette.brush(QPalette::Disabled, QPalette::Text);

	QList<QString>::const_iterator iter = szStringList.begin();
	while(iter != szStringList.end())
	{
		//Getting an item from QList<QString> to add it to a QList<QStandardItem>
		if((*iter).isNull()){
			pStandardItem = new QStandardItem(QString("NULL"));
			pStandardItem->setData(DataTypeNull, DataTypeRole);
			font = pStandardItem->font();
			font.setItalic(true);
			pStandardItem->setFont(font);
			pStandardItem->setForeground(nullbrush);
		} else {
			pStandardItem = new QStandardItem(*iter);
		}
		pStandardItem->setEditable(true);
		listStandardItemList.append(pStandardItem);
		iter++;
	}
	return listStandardItemList;
}

void QDatabaseTableViewController::showQueryInformation()
{
	QString szQueryInformation = m_pDatabaseController->getQueryResultString();
	QTextCursor cursor(m_pDatabaseTableView->getConsoleTextEdit()->textCursor());//Creates a cursor
	cursor.movePosition(QTextCursor::Start);//Moves the cursor to the start
	m_pDatabaseTableView->getConsoleTextEdit()->setTextCursor(cursor);//Sets the cursor position
	m_pDatabaseTableView->getConsoleTextEdit()->insertPlainText(szQueryInformation);//insert text at the cursor position
}

void QDatabaseTableViewController::onDbLoadTableDescription(const QStringList& listRowHeader, const QStringList& listRowData, DatabaseQueryStep step, void* user_data)
{
	QDatabaseTableViewController* pDatabaseTableViewController = (QDatabaseTableViewController*)(user_data);

	if(step == DBQueryStepStart){
		pDatabaseTableViewController->m_pDatabaseTableView->getStructureModel()->setHorizontalHeaderLabels(listRowHeader);
	}
	//Creating a QList<QStandardItem> in order to append a row to the model
	if(step == DBQueryStepRow){
		QList<QStandardItem*> listRowDataItem;
		listRowDataItem = pDatabaseTableViewController->makeStandardItemListFromStringList(listRowData);
		//Adds a row to the table
		pDatabaseTableViewController->m_pDatabaseTableView->getStructureModel()->appendRow(listRowDataItem);
	}
	if(step == DBQueryStepEnd){
		pDatabaseTableViewController->m_pDatabaseTableView->getStructureTreeView()->header()->resizeSections(QHeaderView::ResizeToContents);
	}
}

void QDatabaseTableViewController::onDbLoadTableCreationScript(const QString& szCreationScriptString, void* user_data)
{
	QDatabaseTableViewController* pDatabaseTableViewController = (QDatabaseTableViewController*)(user_data);
	pDatabaseTableViewController->m_pDatabaseTableView->getCreationScriptTextEdit()->append(szCreationScriptString);
}
