#pragma once

#include "cocos2d.h"

class GameOverlay : public cocos2d::LayerColor
{
public:
	CREATE_FUNC(GameOverlay);
	virtual bool init() override;
protected:
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

class PauseOverlay : public GameOverlay
{
public:
	CREATE_FUNC(PauseOverlay);
	virtual bool init() override;
	
	std::function<void(void)> resumeCallback;
	std::function<void(void)> restartCallback;
	std::function<void(void)> exitCallback;
protected:
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
};

class DieOverlay : public GameOverlay
{
public:
	CREATE_FUNC(DieOverlay);
	virtual bool init() override;
	
	std::function<void(void)> restartCallback;
	std::function<void(void)> exitCallback;
protected:
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
};

class FinishOverlay : public GameOverlay
{
public:
	CREATE_FUNC(FinishOverlay);
	virtual bool init() override;
	
	std::function<void(void)> restartCallback;
	std::function<void(void)> nextLevelCallback;
	std::function<void(void)> exitCallback;
protected:
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
};
