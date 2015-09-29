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
	
	cocos2d::Texture2D::TexParams texParams;
	texParams.magFilter = GL_LINEAR;
	texParams.minFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	
	cocos2d::Sprite* bg = cocos2d::Sprite::create("bg.png", cocos2d::Rect(mOrigin.x, mOrigin.y, mVisibleSize.width, mVisibleSize.height));
	bg->getTexture()->setTexParameters(texParams);
	bg->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height/2));
	addChild(bg);
	
	auto logo = cocos2d::Sprite::create("logo.png");
	logo->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height * 0.8f));
	this->addChild(logo);
	
	auto loading = cocos2d::Label::createWithTTF("Loading .. ", "fonts/semibold.otf", 6);
	loading->setColor(cocos2d::Color3B::WHITE);
	loading->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height * 0.45f));
	loading->setTag(13);
	this->addChild(loading);
	
	schedule(schedule_selector(IntroScene::load), 0);
	
	return true;
}

void IntroScene::load(float dt)
{
	Loader::loadEverything();
	
	unschedule(schedule_selector(IntroScene::load));
	
	this->removeChildByTag(13);
	
	auto menu = cocos2d::Menu::create();
	menu->ignoreAnchorPointForPosition(false);
	menu->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y));
	menu->setAnchorPoint(cocos2d::Vec2::ZERO);
	menu->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height));
	this->addChild(menu, 2);
	
	auto mode1Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("?", "fonts/default.otf", 6),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(GameMode1Scene::create());
	});
	mode1Btn->setAnchorPoint(cocos2d::Vec2(0, 1));
	mode1Btn->setPosition(2, mVisibleSize.height - 2);
	menu->addChild(mode1Btn);
	
	auto mode2Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("?", "fonts/default.otf", 6),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(GameMode2InfiniteScene::create());
	});
	mode2Btn->setAnchorPoint(cocos2d::Vec2(1, 1));
	mode2Btn->setPosition(mVisibleSize.width - 2, mVisibleSize.height - 2);
	menu->addChild(mode2Btn);
	
	cocos2d::Sprite* btnBg = cocos2d::Sprite::createWithSpriteFrameName("big_play_bg");
	btnBg->setPosition(mOrigin.x + mVisibleSize.width * 0.5f, mOrigin.y + mVisibleSize.height * 0.4f);
	this->addChild(btnBg, 1);
	
	auto mode3Btn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("big_play_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(LevelSelectScene::create());
	});
	mode3Btn->setPosition(mVisibleSize.width * 0.5f, mVisibleSize.height * 0.4f);
	menu->addChild(mode3Btn);
	/*
	auto exitBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("back_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		AppDelegate::closeApp();
	});
	exitBtn->setPosition(9, 9);
	menu->addChild(exitBtn);
	*/
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(IntroScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(IntroScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	if (AppDelegate::pluginAnalytics != nullptr)
		AppDelegate::pluginAnalytics->logPageView("intro");
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
