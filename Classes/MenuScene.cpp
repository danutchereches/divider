#include "MenuScene.h"

MenuScene::MenuScene()
{
	cocos2d::log("menu scene constructed");
}

MenuScene::~MenuScene()
{
	cocos2d::log("menu scene destructed");
}

bool MenuScene::init()
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
		cocos2d::Director::getInstance()->pushScene(GameMode2Scene::create());
	});
	mode2Btn->setPosition(cocos2d::Vec2(mVisibleSize.width/2, mVisibleSize.height * 0.5f));
	menu->addChild(mode2Btn);
	
	auto exitBtn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("EXIT", "fonts/default.ttf", 12),
			[] (cocos2d::Ref* btn) {
		AppDelegate::closeApp();
	});
	exitBtn->setPosition(cocos2d::Vec2(mVisibleSize.width/2, mVisibleSize.height * 0.35f));
	menu->addChild(exitBtn);

	return true;
}
