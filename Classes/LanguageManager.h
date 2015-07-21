#ifndef __LANGUAGEMANAGER_H__
#define __LANGUAGEMANAGER_H__

#include "cocos2d.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"

class LanguageManager {
public:
	static void init();
	static void init(cocos2d::LanguageType);
	
	static LanguageManager* getInstance();
	const std::string getStringForKey(std::string key);
	static const std::string getString(std::string key);
private:
	LanguageManager(const std::string& fileName);
	
	rapidjson::Document mDocument;
	static LanguageManager* mInstance;
};
#endif // __LANGUAGEMANAGER_H__
