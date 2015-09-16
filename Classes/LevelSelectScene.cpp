#include "LevelSelectScene.h"

LevelSelectScene::LevelSelectScene()
{
	cocos2d::log("level select scene constructed");
}

LevelSelectScene::~LevelSelectScene()
{
	cocos2d::log("level select scene destructed");
}

bool LevelSelectScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::LayerColor* bg = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 30, 255));
	this->addChild(bg);
	
	auto menu = cocos2d::Menu::create();
	menu->ignoreAnchorPointForPosition(false);
	menu->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y));
	menu->setAnchorPoint(cocos2d::Vec2::ZERO);
	menu->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height));
	this->addChild(menu);
	
	cocos2d::MenuItem* menuItem;
	float width = (mVisibleSize.width - 20)/3;
	float height = (mVisibleSize.height - 40) / 4;
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			int levelNr = x * 4 + y + 1;
			
			menuItem = cocos2d::MenuItem::create();
			menuItem->setPosition(cocos2d::Vec2(10 + width * x, mVisibleSize.height - 20 - y * height));
			menuItem->setContentSize(cocos2d::Size(width, height));
			menuItem->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
			menu->addChild(menuItem);
			
			cocos2d::Label* label = cocos2d::Label::createWithTTF(cocos2d::__String::createWithFormat("level %d", levelNr)->_string, "fonts/default.ttf", 6);
			label->setPosition(width/2, height * 0.65f);
			menuItem->addChild(label);
			
			for (int i = 0; i < 3; i++)
			{
				cocos2d::Sprite* star = cocos2d::Sprite::createWithSpriteFrameName("star_w");
				star->setPosition(width / 4 * (i+1), 8);
				menuItem->addChild(star);
			}
			
			if (levelNr <= 1)
			{
				menuItem->setCallback([levelNr] (cocos2d::Ref* btn) {
					cocos2d::Director::getInstance()->pushScene(GameMode2LevelScene::create(levelNr));
				});
			}
			else
			{
				label->setColor(cocos2d::Color3B::GRAY);
			}
		}
	}
	
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(LevelSelectScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(LevelSelectScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void LevelSelectScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void LevelSelectScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (AppDelegate::pluginAnalytics != nullptr)
			AppDelegate::pluginAnalytics->logEvent("click_back_btn");
		
		cocos2d::Director::getInstance()->popScene();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_DOWN)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->decreaseVolume();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_UP)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->increaseVolume();
	}
}
