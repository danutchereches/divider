#include "LevelSelectScene.h"

Level LevelSelectScene::LEVELS[] =
{
		Level(101, 2, 10.0f, 20, 30), // level 1
		Level(102, 5, 10.0f, 20, 30), // level 2
		Level(103, 3, 10.0f, 20, 40), // level 3
		Level(104, 4, 11.0f, 20, 30), // level 4
		Level(105, 6, 12.0f, 20, 40), // level 5
		Level(106, 3, 13.0f, 20, 50), // level 6
		Level(107, 5, 20.0f, 20, 60), // level 7
		Level(108, 8, 15.0f, 20, 30), // level 8
		Level(109, 9, 14.0f, 20, 30), // level 9
		Level(110, 7, 14.0f, 20, 30), // level 10
		Level(111, 6, 16.0f, 20, 60), // level 11
		Level(112, 4, 17.0f, 20, 50), // level 12
		Level(113, 7, 15.0f, 20, 50), // level 13
		Level(114, 9, 14.5f, 20, 50), // level 14
		Level(115, 7, 17.0f, 20, 80)  // level 15
};

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
	
	mMenu = cocos2d::Menu::create();
	mMenu->ignoreAnchorPointForPosition(false);
	mMenu->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y));
	mMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	mMenu->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height));
	this->addChild(mMenu);
	
	cocos2d::MenuItem* menuItem;
	float width = (mVisibleSize.width - 20) / 3;
	float height = (mVisibleSize.height - 40) / 5;
	int n = sizeof(LEVELS)/sizeof(Level);
	
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int levelNr = y * 3 + x;
			
			if (levelNr >= n)
				break;
			
			menuItem = cocos2d::MenuItem::create();
			menuItem->setPosition(cocos2d::Vec2(10 + width * x, mVisibleSize.height - 20 - y * height));
			menuItem->setContentSize(cocos2d::Size(width, height));
			menuItem->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
			menuItem->setTag(LEVELS[levelNr].getId());
			mMenu->addChild(menuItem);
			
			cocos2d::Label* label = cocos2d::Label::createWithTTF(helpers::String::format("level %d", levelNr+1), "fonts/default.ttf", 6);
			label->setPosition(width/2, height * 0.65f);
			menuItem->addChild(label);
			
			for (int i = 0; i < 3; i++)
			{
				cocos2d::Sprite* star = cocos2d::Sprite::createWithSpriteFrameName("star_b");
				star->setPosition(width / 4 * (i+1), 8);
				star->setTag(i+1);
				menuItem->addChild(star);
			}
			
			if (true) //TODO: read level progress
			{
				menuItem->setCallback([levelNr] (cocos2d::Ref* btn) {
					cocos2d::Director::getInstance()->pushScene(GameMode2LevelScene::create(&LEVELS[levelNr]));
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

void LevelSelectScene::onEnter()
{
	cocos2d::Scene::onEnter();
	
	cocos2d::Node* menuItem;
	cocos2d::Sprite* star;
	cocos2d::SpriteFrame* starW = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("star_w");
	int n = sizeof(LEVELS)/sizeof(Level);
	
	for (int i = 0; i < n; i++)
	{
		menuItem = mMenu->getChildByTag(LEVELS[i].getId());
		
		if (!menuItem)
			continue;
		
		if (LEVELS[i].getScore() >= LEVELS[i].getNrDivisible() * 0.5f)
		{
			star = dynamic_cast<cocos2d::Sprite*>(menuItem->getChildByTag(1));
			star->setSpriteFrame(starW);
		}
		
		if (LEVELS[i].getScore() >= LEVELS[i].getNrDivisible() * 0.75f)
		{
			star = dynamic_cast<cocos2d::Sprite*>(menuItem->getChildByTag(2));
			star->setSpriteFrame(starW);
		}
		
		if (LEVELS[i].getScore() >= LEVELS[i].getNrDivisible())
		{
			star = dynamic_cast<cocos2d::Sprite*>(menuItem->getChildByTag(3));
			star->setSpriteFrame(starW);
		}
	}
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
