#include "LanguageManager.h"

LanguageManager* LanguageManager::mInstance = nullptr;

void LanguageManager::init()
{
	init(cocos2d::Application::getInstance()->getCurrentLanguage());
}

void LanguageManager::init(cocos2d::LanguageType lang)
{
	std::string fileName;
	// detect current language
	switch (lang)
	{
		case cocos2d::LanguageType::ENGLISH:
			fileName = "en.json";
			break;
		case cocos2d::LanguageType::ROMANIAN:
			fileName = "ro.json";
			break;
		default:
			cocos2d::log("Unknown language. Use english");
			fileName = "en.json";
			break;
	};
	
	mInstance = new LanguageManager(fileName);
}

LanguageManager::LanguageManager(const std::string& fileName)
{
	// below we open, read and parse language data file with rapidjson library
	std::string clearContent = cocos2d::FileUtils::getInstance()->getStringFromFile("lang/" + fileName);
	
	mDocument.Parse<0>(clearContent.c_str());
	if (mDocument.HasParseError())
	{
		cocos2d::log("Language file parsing error!");
		return;
	}
}

LanguageManager* LanguageManager::getInstance()
{
	return mInstance;
}

const std::string LanguageManager::getStringForKey(std::string key)
{
	if (!mDocument.IsObject())
		return key;
	
	if (mDocument[key.c_str()].IsString())
		return mDocument[key.c_str()].GetString();
	else
		return key;
}

const std::string LanguageManager::getString(std::string key)
{
	return mInstance->getStringForKey(key);
}
