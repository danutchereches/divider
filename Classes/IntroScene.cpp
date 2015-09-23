#include "IntroScene.h"

IntroScene::IntroScene()
{
	cocos2d::log("intro scene constructed");
}

IntroScene::~IntroScene()
{
	cocos2d::log("intro scene destructed");
}

bool IntroScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::LayerColor* bg = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 30, 255));
	this->addChild(bg);
	
	auto logo = cocos2d::Label::createWithTTF("DIVIDER", "fonts/default.ttf", 22);
	logo->setColor(cocos2d::Color3B(120, 211, 73));
	logo->setPosition(mScreenSize.width/2, mScreenSize.height * 0.9f);
	this->addChild(logo);
	
	auto menu = cocos2d::Menu::create();
	menu->ignoreAnchorPointForPosition(false);
	menu->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y));
	menu->setAnchorPoint(cocos2d::Vec2::ZERO);
	menu->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height));
	this->addChild(menu);
	
	auto mode1Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("mode 1", "fonts/default.ttf", 12),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(GameMode1Scene::create());
	});
	mode1Btn->setPosition(cocos2d::Vec2(mVisibleSize.width/2, mVisibleSize.height * 0.65f));
	menu->addChild(mode1Btn);
	
	auto mode2Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("mode 2", "fonts/default.ttf", 12),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(GameMode2InfiniteScene::create());
	});
	mode2Btn->setPosition(cocos2d::Vec2(mVisibleSize.width/2, mVisibleSize.height * 0.5f));
	menu->addChild(mode2Btn);
	
	auto mode3Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("mode 3", "fonts/default.ttf", 12),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(LevelSelectScene::create());
	});
	mode3Btn->setPosition(cocos2d::Vec2(mVisibleSize.width/2, mVisibleSize.height * 0.35f));
	menu->addChild(mode3Btn);
	
	auto exitBtn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("EXIT", "fonts/default.ttf", 10),
			[] (cocos2d::Ref* btn) {
		AppDelegate::closeApp();
	});
	exitBtn->setPosition(cocos2d::Vec2(mVisibleSize.width/2, mVisibleSize.height * 0.15f));
	menu->addChild(exitBtn);
	
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(IntroScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(IntroScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	if (AppDelegate::pluginAnalytics != nullptr)
		AppDelegate::pluginAnalytics->logPageView("intro");
	
	return true;
}

void IntroScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void IntroScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (AppDelegate::pluginAnalytics != nullptr)
			AppDelegate::pluginAnalytics->logEvent("click_back_btn");
		
		AppDelegate::closeApp();
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
