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

	int iVal = 0;
	foreach(const QJsonValue& val, jsonArray)
	{
		QString szDatabaseIdentifier = val.toObject().value("identifier").toString();
		// Check if each file path exists: if not, the database is deleted from the json
		if(!QFileInfo::exists(szDatabaseIdentifier)){
			jsonArray.removeAt(iVal);
			iVal--;
		}else{
			QString szDatabaseName = val.toObject().value("name").toString();
			int iID = szDatabaseName.split("_").last().toInt();
			ConfigDatabase configDatabase(szDatabaseIdentifier, iID);
			addDatabase(configDatabase);
		}
		iVal++;
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

	foreach(const QString& szDatabaseName, szListFiles)
	{
		// Split database name to get its ID
		int iDatabaseID = szDatabaseName.split("_").last().split(".").first().toInt();
		bDatabaseExists = false;

		foreach(const ConfigDatabase& configDatabase, m_listConfigDatabase)
		{
			if(configDatabase.getDatabaseID() == iDatabaseID){
				bDatabaseExists = true;
			}
		}

		// The file is deleted if it is not used
		if(!bDatabaseExists){
			QFile file(QSettingsManager::getInstance().getDatabasesJsonDir() + szDatabaseName);
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
	foreach(const ConfigDatabase& configDatabase, m_listConfigDatabase)
	{
		if(iDatabasesCounter < iMaxDatabases)
		{
			QJsonObject jsonNewObject;
			jsonNewObject.insert("identifier", configDatabase.getDatabaseIdentifier());
			jsonNewObject.insert("name", configDatabase.getDatabaseName());
			jsonArray.push_back(jsonNewObject);
			iNextID = qMax(configDatabase.getDatabaseID(), iNextID);
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

bool ConfigDatabaseController::initDatabaseQueries(const QString& szName)
{
	QString szFilePath = QSettingsManager::getInstance().getDatabasesJsonDir() + szName + ".json";
	QJsonObject newDatabaseJsonObject;
	QJsonArray newDatabaseJsonArray;
	newDatabaseJsonObject.insert("queries", newDatabaseJsonArray);
	QJsonDocument newDatabaseJsonDocument;
	newDatabaseJsonDocument.setObject(newDatabaseJsonObject);

	return writeToFile(szFilePath, newDatabaseJsonDocument.toJson());
}

void ConfigDatabaseController::loadDatabaseQueries(const QString& szName, QStringList& listQueries)
{
	QString szFilePath = QSettingsManager::getInstance().getDatabasesJsonDir() + szName + ".json";
	QJsonDocument jsonDocument = parseToJsonDocument(szFilePath);
	QJsonArray jsonArray = jsonDocument.object().find("queries").value().toArray();

	listQueries.clear();
	foreach(const QJsonValue& val, jsonArray)
	{
		listQueries.append(val.toString());
	}
}

void ConfigDatabaseController::addDatabaseQuery(const QString& szQuery, QStringList& listQueries)
{
	listQueries.push_front(szQuery);
}

bool ConfigDatabaseController::saveDatabaseQueries(const QString& szName, const QStringList& szListQueries)
{
	QString szFilePath = QSettingsManager::getInstance().getDatabasesJsonDir() + szName + ".json";
	QJsonDocument jsonDocument = parseToJsonDocument(szFilePath);
	QJsonArray jsonArray;

	foreach(const QString& szQuery, szListQueries)
	{
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
	foreach(const ConfigDatabase& configDatabase, m_listConfigDatabase)
	{
		if(QString::compare(configDatabase.getDatabaseIdentifier(), szDatabaseIdentifier) == 0){
			return configDatabase.getDatabaseName();
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
