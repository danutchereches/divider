#include "GameScene.h"

int GameScene::NUMBER_POOL_SIZE = 21;
int GameScene::NUMBER_POOL[] = {10, 12, 14, 15, 18, 20, 21, 25, 27, 28, 32, 35, 36, 42, 45, 48, 50, 54, 56, 64, 68};
int GameScene::DIVISORS_SIZE = 7;
int GameScene::DIVISORS[] = {3, 4, 5, 6, 7, 8, 9};

cocos2d::Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = cocos2d::Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(44, 44, 44, 255)))
	{
		return false;
	}
	
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	
	mIsGameServicesAvailable = false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::log("size %f, %f", mScreenSize.width, mScreenSize.height);
	cocos2d::log("visible size %f, %f", mVisibleSize.width, mVisibleSize.height);
	cocos2d::log("offset %f, %f", mOrigin.x, mOrigin.y);
	
	mGameLayer = cocos2d::Layer::create();
	mGameLayer->ignoreAnchorPointForPosition(false);
	mGameLayer->setPosition(cocos2d::Vec2(0, 20));
	mGameLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	mGameLayer->setContentSize(cocos2d::Size(mScreenSize.width, mScreenSize.height - 20));
	this->addChild(mGameLayer, 100);
	
	mUILayer = cocos2d::Layer::create();
	this->addChild(mUILayer, 200);
	
	cocos2d::LayerColor* bottomBar = cocos2d::LayerColor::create(cocos2d::Color4B(200, 100, 100, 255));
	bottomBar->ignoreAnchorPointForPosition(false);
	bottomBar->setPosition(mOrigin.x, mOrigin.y);
	bottomBar->setAnchorPoint(cocos2d::Vec2::ZERO);
	bottomBar->setContentSize(cocos2d::Size(mVisibleSize.width, 20));
	mUILayer->addChild(bottomBar);
	
	cocos2d::Menu* bottomMenu = cocos2d::Menu::create();
	bottomMenu->ignoreAnchorPointForPosition(false);
	bottomMenu->setPosition(cocos2d::Vec2::ZERO);
	bottomMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	bottomMenu->setContentSize(bottomBar->getContentSize());
	bottomBar->addChild(bottomMenu);
	
	float dw = bottomBar->getContentSize().width / DIVISORS_SIZE;
	std::function<void(cocos2d::Ref*)> callback = [this](cocos2d::Ref* node) {
		cocos2d::MenuItem* menuItem = dynamic_cast<cocos2d::MenuItem*>(node);
		if (menuItem == nullptr)
			return;
		
		updateDivisor(menuItem->getTag());
	//	cocos2d::Label* divisor = dynamic_cast<cocos2d::Label*>(menuItem->getChildren().front());
	//	if (divisor == nullptr)
	//		return;
	};
	for (int i = 0; i < DIVISORS_SIZE; i++)
	{
		cocos2d::Label* label = cocos2d::Label::createWithTTF(cocos2d::__String::createWithFormat("%d", DIVISORS[i])->_string, "fonts/default.ttf", 12);
		cocos2d::MenuItemLabel* divisor = cocos2d::MenuItemLabel::create(label, callback);
		label->setPosition(cocos2d::Vec2(dw/2, bottomBar->getContentSize().height/2));
		label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		divisor->setPosition(i * dw, 0);
		divisor->setContentSize(cocos2d::Size(dw, bottomBar->getContentSize().height));
		divisor->setAnchorPoint(cocos2d::Vec2::ZERO);
		divisor->setTag(DIVISORS[i]);
		bottomMenu->addChild(divisor);
	}
	
	updateDivisor(6);
	
	mBallZOrder = 999999;
	
	checkNumbers();
	initPools();
	
	// Register Touch Event
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	auto listener2 = cocos2d::EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	
	auto listener3 = cocos2d::EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, CC_CALLBACK_0(GameScene::onComeToForeground, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener3, this);
	
	auto listener4 = cocos2d::EventListenerCustom::create(EVENT_COME_TO_BACKGROUND, CC_CALLBACK_0(GameScene::onComeToBackground, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener4, this);
	
	this->schedule(schedule_selector(GameScene::update), 0.02f);
	this->schedule(schedule_selector(GameScene::updateSlow), 1.5f);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		std::vector<cocos2d::plugin::PluginParam*> params;
		params.push_back(new cocos2d::plugin::PluginParam("game"));
		AppDelegate::pluginAnalytics->callFuncWithParam("logPageView", params);
	}
	
	return true;
}

void GameScene::checkNumbers()
{
	cocos2d::log("Calculating number of divisors for all numbers:");
	for (int i = 0; i < NUMBER_POOL_SIZE; i++)
	{
		int m = 0;
		for (int j = 0; j < DIVISORS_SIZE; j++)
			if (NUMBER_POOL[i] % DIVISORS[j] == 0)
				m++;
		
		if (m == 0)
		{
			cocos2d::log("ERROR: number %d doesn't have any numbers in divisors!", NUMBER_POOL[i]);
			AppDelegate::closeApp();
			return;
		}
		
		cocos2d::log(" -- %d has %d divisors", NUMBER_POOL[i], m);
	}
	
	cocos2d::log("\nCounting numbers in pool for each divisor:");
	for (int i = 0; i < DIVISORS_SIZE; i++)
	{
		int m = 0;
		for (int j = 0; j < NUMBER_POOL_SIZE; j++)
			if (NUMBER_POOL[j] % DIVISORS[i] == 0)
				m++;
		
		cocos2d::log(" -- %d can divide %d numbers from pool", DIVISORS[i], m);
	}
}

void GameScene::initPools()
{
	mBallPool.init(100, mGameLayer);
}

void GameScene::update(float dt)
{
//	timeFromLastMoney += dt;
	
//	if (timeFromLastMoney >= 0.5f)
//	{
//		timeFromLastMoney = 0;
//		
//
//	}
}

void GameScene::updateSlow(float dt)
{
	if (AppDelegate::pluginGameServices->isSignedIn() != mIsGameServicesAvailable)
	{
		mIsGameServicesAvailable = AppDelegate::pluginGameServices->isSignedIn();
		
		if (mIsGameServicesAvailable)
		{
		//	if (mScore > 0)
		//		AppDelegate::pluginGameServices->publishScore(mScore);
			
		//	if (mScore >= 10000)
		//		AppDelegate::pluginGameServices->unlockAchievement(1);
		}
	}
	
	Ball* ball = mBallPool.obtainPoolItem();
	ball->setScale(0.2f);
	ball->setAnchorPoint(cocos2d::Vec2::ZERO);
	ball->setPosition(rand() % (int) mScreenSize.width , rand() % (int) mScreenSize.height);
	ball->setNumber(NUMBER_POOL[rand() % NUMBER_POOL_SIZE]);
	ball->setColor(cocos2d::Color3B(55+rand() % 200, 55+rand() % 200, 55+rand() % 200));
	ball->setVisible(true);
	ball->setLocalZOrder(mBallZOrder--);
	ball->runAction(BallAction::create());
	mBalls.pushBack(ball);
}

void GameScene::updateDivisor(int d)
{
	mSelectedDivisor = d;
	
	cocos2d::Node* menu = mUILayer->getChildren().front()->getChildren().front();//UberHack!
	cocos2d::Vector<cocos2d::Node*> items = menu->getChildren();
	if (items.size() != DIVISORS_SIZE)
	{
		cocos2d::log("ERROR: wrong number of items in divisors menu. (%d vs %d required)", (int) items.size(), DIVISORS_SIZE);
		AppDelegate::closeApp();
		return;
	}
	
	for (auto it = items.begin(); it != items.end(); it++)
	{
		cocos2d::Node* item = (*it);
		cocos2d::Label* label = dynamic_cast<cocos2d::Label*>(item->getChildren().front());
		if (mSelectedDivisor == (*it)->getTag())
		{
			label->setScale(1.0f);
			label->setColor(cocos2d::Color3B::WHITE);
		}
		else
		{
			label->setScale(0.7f);
			label->setColor(cocos2d::Color3B::GRAY);
		}
	}
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (!mBalls.empty())
	{
		for (auto it = mBalls.begin(); it != mBalls.end(); it++)
		{
			Ball* ball = *it;
			cocos2d::Vec2 local = touch->getLocation() - mGameLayer->getPosition();
			cocos2d::Rect r = ball->getBoundingBox();
		//	r.origin += mGameLayer->getPosition(); //for squares
			
		//	if (r.containsPoint(local)) //for squares
			if (local.distance(ball->getPosition()) <= r.size.width/2)
			{
				if (ball->getNumber() % mSelectedDivisor == 0)
				{
					mBallPool.recyclePoolItem(ball);
					mBalls.eraseObject(ball);
				}
				else
				{
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
					ball->setScale(ball->getScale() + 0.1f);
				}
				
				break;
			}
		}
	}
	
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	
}
