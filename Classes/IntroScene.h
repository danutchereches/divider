#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "Helpers.h"

class IntroScene : public cocos2d::Scene {
public:
	IntroScene();
	~IntroScene();
	
	virtual bool init() override;
	
	CREATE_FUNC(IntroScene);
private:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
};
