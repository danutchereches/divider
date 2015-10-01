#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "LevelSelectScene.h"
#include "Helpers.h"

class IntroScene : public cocos2d::Scene {
public:
	IntroScene();
	~IntroScene();
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(IntroScene);
private:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	bool mIsGameServicesAvailable;
	
	void load(float dt);
	void update(float dt);
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};
