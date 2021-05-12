/*
 * ConfigDatabaseController.cpp
 *
 *  Created on: 15 avr. 2021
 *      Author: gsegard
 */

#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "Controller/ConfigDatabaseController.h"
#include "Settings/QSettingsManager.h"

ConfigDatabaseController::ConfigDatabaseController()
{

}

ConfigDatabaseController::~ConfigDatabaseController()
{

}

bool ConfigDatabaseController::initDatabasesList()
{
	QJsonDocument jsonDocument = parseToJsonDocument(QSettingsManager::getInstance().getDatabasesJsonFile());
	QJsonObject jsonObject = jsonDocument.object();
	QJsonArray jsonArray = jsonObject.value("databases").toArray();

	int iCounter = 0;
	QJsonArray::const_iterator iterJson;
	for(iterJson = jsonArray.constBegin(); iterJson != jsonArray.constEnd(); ++iterJson)
	{
		QString szDatabaseIdentifier = iterJson->toObject().value("identifier").toString();
		// Check if each file path exists: if not, the database is deleted from the json
		if(!QFileInfo::exists(szDatabaseIdentifier)){
			jsonArray.removeAt(iCounter);
			iCounter--;
		}else{
			QString szDatabaseName = iterJson->toObject().value("name").toString();
			int iID = szDatabaseName.split("_").last().toInt();
			ConfigDatabase configDatabase(szDatabaseIdentifier, iID);
			addDatabase(configDatabase);
		}
		iCounter++;
	}

	bool bGoOn = removeUnusedQueriesFiles();

	if(bGoOn)
	{
		int iNextId = 0;

		if(!jsonArray.isEmpty()){
			iNextId = jsonObject.value("next_id").toInt();
		}

		jsonObject.insert("next_id", iNextId);
		jsonObject.insert("databases", jsonArray);
		jsonDocument.setObject(jsonObject);
	}

	return writeToFile(QSettingsManager::getInstance().getDatabasesJsonFile(), jsonDocument.toJson());
}

bool ConfigDatabaseController::removeUnusedQueriesFiles()
{
	QDir dirQueriesFile;
	dirQueriesFile.setPath(QSettingsManager::getInstance().getDatabasesJsonDir());
	QStringList szListFiles = dirQueriesFile.entryList(QStringList() << "*.json", QDir::Files);

	bool bDatabaseExists = false;

	QStringList::const_iterator iterListFile;
	for(iterListFile = szListFiles.constBegin(); iterListFile != szListFiles.constEnd(); ++iterListFile)
	{
		// Split database name to get its ID
		int iDatabaseID = iterListFile->split("_").last().split(".").first().toInt();
		bDatabaseExists = false;

		ConfigDatabaseList::const_iterator iterConfig;
		for(iterConfig = m_listConfigDatabase.constBegin(); iterConfig != m_listConfigDatabase.constEnd(); ++iterConfig)
		{
			if(iterConfig->getDatabaseID() == iDatabaseID){
				bDatabaseExists = true;
			}
		}

		// The file is deleted if it is not used
		if(!bDatabaseExists){
			const QString szQueriesFile = (*iterListFile);
			QFile file(QSettingsManager::getInstance().getDatabasesJsonDir() + szQueriesFile);
			file.remove();
		}
	}
	return true;
}

bool ConfigDatabaseController::saveDatabasesList()
{
	QJsonDocument jsonDocument = parseToJsonDocument(QSettingsManager::getInstance().getDatabasesJsonFile());
	QJsonObject jsonObject = jsonDocument.object();
	QJsonArray jsonArray;
	int iNextID = 0;

	int iMaxDatabases = 15;
	int iDatabasesCounter = 0;

	ConfigDatabaseList::const_iterator iterConfig;
	for(iterConfig = m_listConfigDatabase.constBegin(); iterConfig != m_listConfigDatabase.constEnd(); ++iterConfig)
	{
		if(iDatabasesCounter < iMaxDatabases)
		{
			QJsonObject jsonNewObject;
			jsonNewObject.insert("identifier", iterConfig->getDatabaseIdentifier());
			jsonNewObject.insert("name", iterConfig->getDatabaseName());
			jsonArray.push_back(jsonNewObject);
			iNextID = qMax(iterConfig->getDatabaseID(), iNextID);
		}
		iDatabasesCounter++;
	}

	jsonObject.insert("next_id", iNextID + 1);
	jsonObject.insert("databases", jsonArray);
	jsonDocument.setObject(jsonObject);

	return writeToFile(QSettingsManager::getInstance().getDatabasesJsonFile(), jsonDocument.toJson());
}

void ConfigDatabaseController::addDatabase(const ConfigDatabase& configDatabase)
{
	m_listConfigDatabase.push_front(configDatabase);
}

void ConfigDatabaseController::moveDatabaseFirstInList(const ConfigDatabase& configDatabase, int iIndex)
{
	m_listConfigDatabase.removeAt(iIndex);
	m_listConfigDatabase.push_front(configDatabase);
}

bool ConfigDatabaseController::initQueries(const QString& szName)
{
	QString szFilePath = QString("%1%2.json").arg(QSettingsManager::getInstance().getDatabasesJsonDir(), szName);
	QJsonObject newDatabaseJsonObject;
	QJsonArray newDatabaseJsonArray;
	newDatabaseJsonObject.insert("queries", newDatabaseJsonArray);
	QJsonDocument newDatabaseJsonDocument;
	newDatabaseJsonDocument.setObject(newDatabaseJsonObject);

	return writeToFile(szFilePath, newDatabaseJsonDocument.toJson());
}

bool ConfigDatabaseController::loadQueries(const QString& szName, QStringList& szListQueries)
{
	QString szFilePath = QString("%1%2.json").arg(QSettingsManager::getInstance().getDatabasesJsonDir(), szName);
	QJsonDocument jsonDocument = parseToJsonDocument(szFilePath);
	QJsonArray jsonArray = jsonDocument.object().find("queries").value().toArray();

	szListQueries.clear();

	QJsonArray::const_iterator iterJson;
	for(iterJson = jsonArray.constBegin(); iterJson != jsonArray.constEnd(); ++iterJson)
	{
		szListQueries.append(iterJson->toString());
	}

	return !szListQueries.isEmpty();
}

void ConfigDatabaseController::addQuery(const QString& szQuery, QStringList& szListQueries)
{
	szListQueries.push_front(szQuery);
}

bool ConfigDatabaseController::removeQuery(const QString& szQuery, QStringList& szListQueries)
{
	return szListQueries.removeOne(szQuery);
}

bool ConfigDatabaseController::saveQueries(const QString& szName, const QStringList& szListQueries)
{
	QString szFilePath = QString("%1%2.json").arg(QSettingsManager::getInstance().getDatabasesJsonDir(), szName);
	QJsonDocument jsonDocument = parseToJsonDocument(szFilePath);
	QJsonArray jsonArray;

	QStringList::const_iterator iterListQuery;
	for(iterListQuery = szListQueries.constBegin(); iterListQuery != szListQueries.constEnd(); ++iterListQuery)
	{
		const QString szQuery = (*iterListQuery);
		jsonArray.append(szQuery);
	}

	QJsonObject jsonObject;
	jsonObject.insert("queries", jsonArray);
	jsonDocument.setObject(jsonObject);

	return writeToFile(szFilePath, jsonDocument.toJson());
}

bool ConfigDatabaseController::writeToFile(const QString& szFilePath, const QByteArray& szText)
{
	QFile file(szFilePath);
	bool bGoOn = (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate));

	if(bGoOn){
		file.write(szText);
	}
	file.close();
	return bGoOn;
}

QJsonDocument ConfigDatabaseController::parseToJsonDocument(const QString& szFile)
{
	QJsonDocument resJsonDoc;

	QFile fileDatabases(szFile);
	bool bGoOn = (fileDatabases.open(QFile::ReadOnly));

	if(bGoOn)
	{
		QByteArray jsonFile = fileDatabases.readAll();
		QJsonParseError jsonError;
		resJsonDoc = QJsonDocument::fromJson(jsonFile, &jsonError);
		if (resJsonDoc.isNull()) {
			qDebug("[Config Database Controller] Parse failed");
		}
	}
	fileDatabases.close();
	return resJsonDoc;
}

QString ConfigDatabaseController::getDatabaseName(const QString& szDatabaseIdentifier) const
{
	ConfigDatabaseList::const_iterator iterConfig;
	for(iterConfig = m_listConfigDatabase.constBegin(); iterConfig != m_listConfigDatabase.constEnd(); ++iterConfig)
	{
		if(QString::compare(iterConfig->getDatabaseIdentifier(), szDatabaseIdentifier) == 0){
			return iterConfig->getDatabaseName();
		}
	}
	return "";
}

int ConfigDatabaseController::getNextID()
{
	QJsonDocument jsonDocument = parseToJsonDocument(QSettingsManager::getInstance().getDatabasesJsonFile());
	QJsonObject jsonObject = jsonDocument.object();
	return jsonObject.value("next_id").toInt();
}

const ConfigDatabaseList& ConfigDatabaseController::getDatabaseList() const
{
	return m_listConfigDatabase;
}
