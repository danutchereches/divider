#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "Level.h"
#include "Helpers.h"

class LevelSelectScene : public cocos2d::Scene {
public:
	LevelSelectScene();
	~LevelSelectScene();
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(LevelSelectScene);
	
	static Level LEVELS[];
private:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	cocos2d::Menu* mMenu;
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};
