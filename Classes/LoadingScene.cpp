#include "LoadingScene.h"

LoadingScene::LoadingScene()
{
	cocos2d::log("loading scene constructed");
}

LoadingScene::~LoadingScene()
{
	cocos2d::log("loading scene destructed");
}

bool LoadingScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	// Register Touch Event
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	cocos2d::LayerColor* bg = cocos2d::LayerColor::create(cocos2d::Color4B(44, 44, 44, 255));
	this->addChild(bg);
	
	auto logo = cocos2d::Label::createWithTTF("logo", "fonts/default.ttf", 36);
	logo->setColor(cocos2d::Color3B(120, 211, 73));
	logo->setPosition(mScreenSize.width/2, mScreenSize.height * 0.7f);
	this->addChild(logo);
	
	auto loading = cocos2d::Label::createWithTTF("Loading .. ", "fonts/default.ttf", 6);
	loading->setColor(cocos2d::Color3B::WHITE);
	loading->setPosition(mScreenSize.width/2, mScreenSize.height * 0.3f);
	this->addChild(loading);
	
	schedule(schedule_selector(LoadingScene::load), 0);
	
	return true;
}

void LoadingScene::onEnter()
{
	cocos2d::Scene::onEnter();
	
}

void LoadingScene::load(float dt)
{
	Loader::loadEverything();
	
	unschedule(schedule_selector(LoadingScene::load));

	auto scene = GameScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(scene);
}
