#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "LevelSelectScene.h"
#include "Helpers.h"

class LevelSelectScene : public cocos2d::Scene {
public:
	LevelSelectScene();
	~LevelSelectScene();
	
	virtual bool init() override;
	
	CREATE_FUNC(LevelSelectScene);
private:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};
