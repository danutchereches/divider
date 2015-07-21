#ifndef __LOADER_H__
#define __LOADER_H__

#include "cocos2d.h"
#include "AppDelegate.h"
#include "Objects.h"

enum class Currencies;

class Loader
{
public:
	static void loadEverything();
	
	static void loadTextures();
	static void loadSounds();
	static void loadAnimations();
	static void loadObjects();
private:
	static cocos2d::SpriteFrameCache* mFrameCache;
};

#endif // __LOADER_H__
