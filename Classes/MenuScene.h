#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "Helpers.h"

class MenuScene : public cocos2d::Scene {
public:
	MenuScene();
	~MenuScene();
	
	virtual bool init() override;
	
	CREATE_FUNC(MenuScene);
private:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
};
