#include "GameScene.h"

int GameScene::NUMBER_POOL_SIZE = 21;
int GameScene::NUMBER_POOL[] = {10, 12, 14, 15, 18, 20, 21, 25, 27, 28, 32, 35, 36, 42, 45, 48, 50, 54, 56, 64, 68};
int GameScene::DIVISORS[] = {2, 5, 3, 4, 6, 7, 8, 9};

bool GameScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	
	mIsGameServicesAvailable = false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::log("size %f, %f", mScreenSize.width, mScreenSize.height);
	cocos2d::log("visible size %f, %f", mVisibleSize.width, mVisibleSize.height);
	cocos2d::log("offset %f, %f", mOrigin.x, mOrigin.y);
	
	cocos2d::LayerColor* bg = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 30, 255));;
	this->addChild(bg, 100);
	
	mGameLayer = cocos2d::Node::create();
//	mGameLayer->ignoreAnchorPointForPosition(false);
	mGameLayer->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y + 20));
	mGameLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	mGameLayer->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height - 20));
	bg->addChild(mGameLayer, 100);
	
	mUILayer = cocos2d::Layer::create();
	mUILayer->ignoreAnchorPointForPosition(false);
	mUILayer->setPosition(mOrigin);
	mUILayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	mUILayer->setContentSize(mVisibleSize);
	this->addChild(mUILayer, 200);
	
	mScoreView = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 10);
	mScoreView->setPosition(cocos2d::Vec2(2, mUILayer->getContentSize().height - 2));
	mScoreView->setAnchorPoint(cocos2d::Vec2(0, 1));
	mUILayer->addChild(mScoreView);
	
	mBallZOrder = 999999;
	mSpawnTimer = 0;
	
	mDivisorMin = 0;
	mDivisorMax = sizeof(DIVISORS) / sizeof(int);
	
	mScore = 0;
	updateScore();
	
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
	
	this->schedule(schedule_selector(GameScene::update));
	this->schedule(schedule_selector(GameScene::updateSlow), 1.0f);
	
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
		for (int j = 0; j < mDivisorMax; j++)
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
	for (int i = mDivisorMin; i < mDivisorMax; i++)
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
	mSpawnTimer += dt;
	
	if (mSpawnTimer >= mSpawnInterval)
	{
		mSpawnTimer = 0;
		
		spawnBall();
	}
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
}

void GameScene::updateScore()
{
	mScoreView->setString(cocos2d::__String::createWithFormat("%d", mScore)->_string);
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
				if (mCurrentDivisor && ball->getNumber() % mCurrentDivisor == 0)
					divideBall(ball);
				else
					missBall(ball);
				
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

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	
}

bool GameMode1Scene::init()
{
	if (!GameScene::init())
		return false;
	
	mDivisorMin = 1;
	
	cocos2d::LayerColor* bottomBar = cocos2d::LayerColor::create(cocos2d::Color4B(200, 100, 100, 255));
	bottomBar->ignoreAnchorPointForPosition(false);
	bottomBar->setPosition(0, 0);
	bottomBar->setAnchorPoint(cocos2d::Vec2::ZERO);
	bottomBar->setContentSize(cocos2d::Size(mUILayer->getContentSize().width, 20));
	bottomBar->setTag(301);
	mUILayer->addChild(bottomBar);
	
	cocos2d::Menu* bottomMenu = cocos2d::Menu::create();
	bottomMenu->ignoreAnchorPointForPosition(false);
	bottomMenu->setPosition(cocos2d::Vec2::ZERO);
	bottomMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	bottomMenu->setContentSize(bottomBar->getContentSize());
	bottomBar->addChild(bottomMenu);
	
	float dw = bottomBar->getContentSize().width / (mDivisorMax - mDivisorMin);
	std::function<void(cocos2d::Ref*)> callback = [this](cocos2d::Ref* node) {
		cocos2d::MenuItem* menuItem = dynamic_cast<cocos2d::MenuItem*>(node);
		if (menuItem == nullptr)
			return;
		
		updateDivisor(menuItem->getTag());
	//	cocos2d::Label* divisor = dynamic_cast<cocos2d::Label*>(menuItem->getChildren().front());
	//	if (divisor == nullptr)
	//		return;
	};
	for (int i = mDivisorMin; i < mDivisorMax; i++)
	{
		cocos2d::Label* label = cocos2d::Label::createWithTTF(cocos2d::__String::createWithFormat("%d", DIVISORS[i])->_string, "fonts/default.ttf", 12);
		cocos2d::MenuItemLabel* divisor = cocos2d::MenuItemLabel::create(label, callback);
		label->setPosition(cocos2d::Vec2(dw/2, bottomBar->getContentSize().height/2));
		label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		divisor->setPosition((i - mDivisorMin) * dw, 0);
		divisor->setContentSize(cocos2d::Size(dw, bottomBar->getContentSize().height));
		divisor->setAnchorPoint(cocos2d::Vec2::ZERO);
		divisor->setTag(DIVISORS[i]);
		bottomMenu->addChild(divisor);
	}
	
	mSpawnInterval = 1.8f;
	
	updateDivisor(6);
	
	return true;
}

void GameMode1Scene::updateDivisor(int d)
{
	mCurrentDivisor = d;
	
	cocos2d::Node* menu = mUILayer->getChildByTag(301)->getChildren().front();//UberHack!
	cocos2d::Vector<cocos2d::Node*> items = menu->getChildren();
	if (items.size() != (mDivisorMax - mDivisorMin))
	{
		cocos2d::log("ERROR: wrong number of items in divisors menu. (%d vs %d required)", (int) items.size(), (mDivisorMax - mDivisorMin));
		AppDelegate::closeApp();
		return;
	}
	
	for (auto it = items.begin(); it != items.end(); it++)
	{
		cocos2d::Node* item = (*it);
		cocos2d::Label* label = dynamic_cast<cocos2d::Label*>(item->getChildren().front());
		if (mCurrentDivisor == (*it)->getTag())
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

void GameMode1Scene::spawnBall()
{
	Ball* ball = mBallPool.obtainPoolItem();
	ball->setPosition(rand() % (int) mScreenSize.width , rand() % (int) mScreenSize.height);
	ball->setNumber(NUMBER_POOL[rand() % NUMBER_POOL_SIZE]);
	ball->setColor(cocos2d::Color3B(55+rand() % 200, 55+rand() % 200, 55+rand() % 200));
	ball->setVisible(true);
	ball->setLocalZOrder(mBallZOrder--);
	ball->runAction(BallAction::create(CC_CALLBACK_0(GameMode1Scene::ballPopCallback, this, ball)));
	mBalls.pushBack(ball);
}

void GameMode1Scene::divideBall(Ball* ball)
{
	mBallPool.recyclePoolItem(ball);
	mBalls.eraseObject(ball);
	
	mScore++;
	updateScore();
}

void GameMode1Scene::missBall(Ball* ball)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
	ball->setScale(ball->getScale() + 0.1f);
}

void GameMode1Scene::ballPopCallback(Ball* ball)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
	cocos2d::Director::getInstance()->replaceScene(clone());
}

GameScene* GameMode1Scene::clone() const
{
	return GameMode1Scene::create();
}

bool GameMode2Scene::init()
{
	if (!GameScene::init())
		return false;
	
	cocos2d::Node* bottomBar = cocos2d::Node::create();
	bottomBar->setPosition(0, 0);
	bottomBar->setAnchorPoint(cocos2d::Vec2::ZERO);
	bottomBar->setContentSize(cocos2d::Size(mUILayer->getContentSize().width, 14));
	bottomBar->setTag(301);
	mUILayer->addChild(bottomBar);
	
	cocos2d::Sprite* barBg = cocos2d::Sprite::createWithSpriteFrameName("bottom_bar");
	barBg->setPosition(bottomBar->getContentSize().width/2, 1.8f);
	barBg->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	bottomBar->addChild(barBg);
	
	mCurrentDivisorLabel = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 9);
	mCurrentDivisorLabel->setPosition(bottomBar->getContentSize().width * 0.478f, bottomBar->getContentSize().height/2);
	bottomBar->addChild(mCurrentDivisorLabel);
	
	mCurrentDivisor = -1;
	
	return true;
}

void GameMode2Scene::update(float dt)
{
	GameScene::update(dt);
	
	mGameLayer->setPositionY(mGameLayer->getPositionY() + dt * mBallSpeed);
}

void GameMode2Scene::updateSlow(float dt)
{
	GameScene::updateSlow(dt);
	
	if (mBalls.size() > 0)
	{
		while (!mBalls.empty())
		{
			Ball* ball = mBalls.front();
			if (ball->getPositionY() > -mGameLayer->getPositionY() + mScreenSize.height)
			{
				mBallPool.recyclePoolItem(ball);
				mBalls.eraseObject(ball);
				
				if (ball->getNumber() % mCurrentDivisor == 0)
					missBall(ball, false);
			}
			else
				break;
		}
	}
}

void GameMode2Scene::updateDivisor(int d)
{
	mCurrentDivisor = d;
	mCurrentDivisorLabel->setString(cocos2d::__String::createWithFormat("%d", mCurrentDivisor)->_string);
}

void GameMode2Scene::spawnBall()
{
	Ball* ball = mBallPool.obtainPoolItem();
	ball->setPosition(rand() % (int) mGameLayer->getContentSize().width, -mGameLayer->getPositionY());
	ball->setNumber(NUMBER_POOL[rand() % NUMBER_POOL_SIZE]);
	ball->setColor(cocos2d::Color3B(55+rand() % 200, 55+rand() % 200, 55+rand() % 200));
	ball->setVisible(true);
	ball->setLocalZOrder(mBallZOrder--);
	ball->setScale(0.5f);
	mBalls.pushBack(ball);
}

void GameMode2Scene::divideBall(Ball* ball)
{
	mBallPool.recyclePoolItem(ball);
	mBalls.eraseObject(ball);
}

void GameMode2Scene::missBall(Ball* ball)
{
	missBall(ball, true);
}

bool GameMode2InfiniteScene::init()
{
	if (!GameMode2Scene::init())
		return false;
	
	cocos2d::Node* bottomBar = mUILayer->getChildByTag(301);
	
	mPreviousDivisorLabel = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 7);
	mPreviousDivisorLabel->setPosition(bottomBar->getContentSize().width * 0.2f, bottomBar->getContentSize().height/2);
	bottomBar->addChild(mPreviousDivisorLabel);
	
	mNextDivisorLabel = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 7);
	mNextDivisorLabel->setPosition(bottomBar->getContentSize().width * 0.8f, bottomBar->getContentSize().height/2);
	mNextDivisorLabel->setVisible(false);
	bottomBar->addChild(mNextDivisorLabel);
	
	mWaveTimer = 0;
	mWaveLength = 30.0f;
	
	mWaveNumber = cocos2d::UserDefault::getInstance()->getBoolForKey("skip_tutorial", false) ? 1 : 0;
	mCurrentDivisor = -1;
	mNextDivisor = -1;
	
	setDivisorRange();
	
	return true;
}

void GameMode2InfiniteScene::update(float dt)
{
	if (mWaveTimer == 0) // if starting new wave, wait for all balls to go out of the screen
	{
		if (mBalls.size() == 0)
		{
			startWave();
			
			mSpawnTimer = mSpawnInterval;
			mWaveTimer += dt;
		}
		else //stall
		{
			mSpawnTimer -= mSpawnInterval;
		}
	}
	else
	{
		mWaveTimer += dt;
	}
	
	GameMode2Scene::update(dt);
	
	if (mWaveTimer >= mWaveLength)
	{
		mWaveTimer = 0;
		
		endWave();
	}
}

void GameMode2InfiniteScene::updateSlow(float dt)
{
	GameMode2Scene::updateSlow(dt);
	
}

void GameMode2InfiniteScene::startWave()
{
	if (mWaveNumber == 0)
		mWaveLength = 20;
	else
		mWaveLength = 30;
	
	if (mWaveNumber == 0)       // difficulty for wave 0
	{
		mBallSpeed = 8.0f;
		mSpawnInterval = 4.0f;
	}
	else if (mWaveNumber <= 1)  // difficulty for wave 1
	{
		mBallSpeed = 10.0f;
		mSpawnInterval = 3.0f;
	}
	else if (mWaveNumber <= 3)  // difficulty for wave 2, 3
	{
		mBallSpeed = 15.0f;
		mSpawnInterval = 2.5f;
	}
	else if (mWaveNumber <= 6)  // difficulty for wave 4, 5, 6
	{
		mBallSpeed = 20.0f;
		mSpawnInterval = 2.0f;
	}
	else if (mWaveNumber <= 10) // difficulty for wave 7, 8, 9, 10
	{
		mBallSpeed = 25.0f;
		mSpawnInterval = 1.5f;
	}
	else                        // difficulty for wave 11+
	{
		mBallSpeed = 30.0f;
		mSpawnInterval = 1.2f;
	}
	
	if (mNextDivisor == -1) // first wave
	{
		mNextDivisor = DIVISORS[mDivisorMin + rand() % (mDivisorMax - mDivisorMin)];
	}
	
	mNextDivisorLabel->setVisible(false);
	updateDivisor(mNextDivisor);
	
	if (mWaveNumber > 1)
	{
		mScore += 5;
		updateScore();
	}
}

void GameMode2InfiniteScene::endWave()
{
	if (mWaveNumber == 0)
		cocos2d::UserDefault::getInstance()->setBoolForKey("skip_tutorial", true);
	
	mWaveNumber++;
	
	setDivisorRange();
	
	pick_number:
	int d = DIVISORS[mDivisorMin + rand() % (mDivisorMax - mDivisorMin)];;
	if ((d == mCurrentDivisor || d == mPreviousDivisor) && (mDivisorMax - mDivisorMin) > 2)
		goto pick_number;
	
	mNextDivisor = d;
	mNextDivisorLabel->setVisible(true);
	mNextDivisorLabel->setString(cocos2d::__String::createWithFormat("%d", d)->_string);
	
	cocos2d::log("selected divisor %d", d);
}

void GameMode2InfiniteScene::setDivisorRange()
{
	if (mWaveNumber == 0)       // difficulty for wave 0
	{
		mDivisorMin = 0;
		mDivisorMax = 1;
	}
	else if (mWaveNumber <= 1)  // difficulty for wave 1
	{
		mDivisorMin = 0;
		mDivisorMax = 3;
	}
	else if (mWaveNumber <= 3)  // difficulty for wave 2, 3
	{
		mDivisorMin = 1;
		mDivisorMax = 4;
	}
	else if (mWaveNumber <= 6)  // difficulty for wave 4, 5, 6
	{
		mDivisorMin = 2;
		mDivisorMax = 5;
	}
	else if (mWaveNumber <= 10) // difficulty for wave 7, 8, 9, 10
	{
		mDivisorMin = 2;
		mDivisorMax = 7;
	}
	else                        // difficulty for wave 11+
	{
		mDivisorMin = 2;
		mDivisorMax = 8;
	}
}

void GameMode2InfiniteScene::updateDivisor(int d)
{
	if (mBalls.size() > 0)
		cocos2d::log("WARNING: Screen is not empty! Divisor shouldn't change while balls are still on screen!");
	
	mPreviousDivisor = mCurrentDivisor;
	
	if (mPreviousDivisor > 0)
		mPreviousDivisorLabel->setString(cocos2d::__String::createWithFormat("%d", mPreviousDivisor)->_string);
	mNextDivisorLabel->setVisible(false);
	
	GameMode2Scene::updateDivisor(d);
}

void GameMode2InfiniteScene::divideBall(Ball* ball)
{
	GameMode2Scene::divideBall(ball);
	
	mScore++;
	updateScore();
}

void GameMode2InfiniteScene::missBall(Ball* ball, bool manual)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
	cocos2d::Director::getInstance()->replaceScene(clone());
}

GameScene* GameMode2InfiniteScene::clone() const
{
	return GameMode2InfiniteScene::create();
}

GameMode2LevelScene* GameMode2LevelScene::create(int level)
{
	GameMode2LevelScene* scene = new (std::nothrow) GameMode2LevelScene();
	
	if (scene && scene->initWithLevelNumber(level))
	{
		scene->autorelease();
		return scene;
	}
	else
	{
		delete scene;
		scene = nullptr;
		return nullptr;
	}
}

bool GameMode2LevelScene::initWithLevelNumber(int level)
{
	if (!GameMode2Scene::init())
		return false;
	
	mLevelNumber = level;
	
	switch (mLevelNumber)
	{
		case 1:
			mBallSpeed = 8.0f;
			mSpawnInterval = 0.5f;
			mCurrentDivisor = 2;
			break;
		case 2:
			mBallSpeed = 9.0f;
			mSpawnInterval = 0.45f;
			mCurrentDivisor = 5;
			break;
		case 3:
			mBallSpeed = 10.0f;
			mSpawnInterval = 0.4f;
			mCurrentDivisor = 3;
			break;
		default:
			return false;
	}
	
	updateDivisor(mCurrentDivisor);
	
	return true;
}

void GameMode2LevelScene::update(float dt)
{
	GameMode2Scene::update(dt);
	
}

void GameMode2LevelScene::updateSlow(float dt)
{
	GameMode2Scene::updateSlow(dt);
	
}

void GameMode2LevelScene::divideBall(Ball* ball)
{
	GameMode2Scene::divideBall(ball);
	
	mScore++;
	updateScore();
}

void GameMode2LevelScene::missBall(Ball* ball, bool manual)
{
	if (manual)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
		cocos2d::Director::getInstance()->replaceScene(clone());
	}
	else
	{
		//TODO: maybe affect score
	}
}

GameScene* GameMode2LevelScene::clone() const
{
	return GameMode2LevelScene::create(mLevelNumber);
}
