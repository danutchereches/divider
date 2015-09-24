#include "GameOverlay.h"

bool GameOverlay::init()
{
	if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 180)))
		return false;
	
	
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameOverlay::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameOverlay::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameOverlay::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	auto listener2 = cocos2d::EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(GameOverlay::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(GameOverlay::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	
	return true;
}

bool GameOverlay::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("GO You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	event->stopPropagation();
	
	return true;
}

void GameOverlay::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("GO You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	event->stopPropagation();
}

void GameOverlay::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("GO You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	event->stopPropagation();
}

void GameOverlay::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("GO button press %d", (int) keyCode);
	
	event->stopPropagation();
}

void GameOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("GO button release %d GO", (int) keyCode);
	
	event->stopPropagation();
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		
	}
}


bool PauseOverlay::init()
{
	if (!GameOverlay::init())
		return false;
	
	cocos2d::Label* title = cocos2d::Label::createWithTTF("PAUSED", "fonts/default.ttf", 10);
	title->setPosition(getContentSize().width/2, getContentSize().height/2);
	addChild(title);
	
	resumeCallback = nullptr;
	restartCallback = nullptr;
	exitCallback = nullptr;
	
	return true;
}

void PauseOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		event->stopPropagation();
		
		if (resumeCallback != nullptr)
			resumeCallback();
		
		removeFromParentAndCleanup(true);
	}
	else
	{
		GameOverlay::onKeyReleased(keyCode, event);
	}
}

bool DieOverlay::init()
{
	if (!GameOverlay::init())
		return false;
	
	cocos2d::Label* title = cocos2d::Label::createWithTTF("DEAD", "fonts/default.ttf", 10);
	title->setPosition(getContentSize().width/2, getContentSize().height/2);
	addChild(title);
	
	restartCallback = nullptr;
	exitCallback = nullptr;
	
	return true;
}

void DieOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		event->stopPropagation();
		
		if (exitCallback != nullptr)
			exitCallback();
	}
	else
	{
		GameOverlay::onKeyReleased(keyCode, event);
	}
}

bool FinishOverlay::init()
{
	if (!GameOverlay::init())
		return false;
	
	cocos2d::Label* title = cocos2d::Label::createWithTTF("LEVEL FINISHED!", "fonts/default.ttf", 10);
	title->setPosition(getContentSize().width/2, getContentSize().height/2);
	addChild(title);
	
	restartCallback = nullptr;
	nextLevelCallback = nullptr;
	exitCallback = nullptr;
	
	return true;
}

void FinishOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		event->stopPropagation();
		
		if (exitCallback != nullptr)
			exitCallback();
	}
	else
	{
		GameOverlay::onKeyReleased(keyCode, event);
	}
}
